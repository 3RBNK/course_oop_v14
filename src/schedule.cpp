#include "../include/schedule.h"
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


Schedule::Schedule(QWidget *parent)
        : QWidget(parent) {}

void Schedule::add_lesson(const QSharedPointer<Lesson> &lesson) {
    m_lessons.append(lesson);
}

void Schedule::remove_lesson(int lesson_id) {
    for (int i = 0; i < m_lessons.size(); ++i) {
        if (m_lessons[i]->lesson_id() == lesson_id) {
            m_lessons.removeAt(i);
            break;
        }
    }
}

bool Schedule::check_conflicts() const {
    for (int i = 0; i < m_lessons.size(); ++i) {
        for (int j = i + 1; j < m_lessons.size(); ++j) {
            if (m_lessons[i]->conflict_with(*m_lessons[j])) {
                return true;
            }
        }
    }
    return false;
}


void Schedule::load_from_json(const QString &file_path, const User &user) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл:" << file.errorString();
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Некорректный JSON";
        return;
    }

    QJsonObject root = doc.object();

    if (user.role() == "student") {
        QJsonObject students = root.value("Студенты").toObject();
        QJsonArray groups = students.value("Группы").toArray();

        for (const QJsonValue &groupValue : groups) {
            QJsonObject groupObj = groupValue.toObject();
            QJsonArray members = groupObj.value("Одногруппники").toArray();

            bool userFound = false;
            for (const QJsonValue &memberValue : members) {
                QJsonObject memberObj = memberValue.toObject();
                if (memberObj.value("name").toString() == user.name()) {
                    userFound = true;
                    break;
                }
            }

            if (userFound) {
                QJsonArray schedule = groupObj.value("Расписание").toArray();
                for (const QJsonValue &lessonValue : schedule) {
                    QJsonObject lessonObj = lessonValue.toObject();

                    int lessonId = lessonObj.value("id").toInt();
                    QString subject = lessonObj.value("subject").toString();
                    QDateTime timeSlot = QDateTime::fromString(lessonObj.value("time_slot").toString(), Qt::ISODate);


                    QSharedPointer<Lesson> lesson = QSharedPointer<Lesson>::create(
                            lessonId, subject, Teacher(), QList<Group>(), timeSlot, Auditorium()
                    );

                    add_lesson(lesson);
                }
                break;
            }
        }
    }
    else if (user.role() == "teacher") {
        QJsonArray teachers = root.value("Преподаватели").toArray();

        for (const QJsonValue &teacherValue : teachers) {
            QJsonObject teacherObj = teacherValue.toObject();
            if (teacherObj.value("name").toString() == user.name()) {
                QJsonArray schedule = teacherObj.value("расписание").toArray();

                for (const QJsonValue &lessonValue : schedule) {
                    QJsonObject lessonObj = lessonValue.toObject();

                    int lessonId = lessonObj.value("id").toInt();
                    QString subject = lessonObj.value("subject").toString();
                    QDateTime timeSlot = QDateTime::fromString(lessonObj.value("time_slot").toString(), Qt::ISODate);

                    QSharedPointer<Lesson> lesson = QSharedPointer<Lesson>::create(
                            lessonId, subject, Teacher(), QList<Group>(), timeSlot, Auditorium()
                    );

                    add_lesson(lesson);
                }
                break;
            }
        }
    }
    else {
        qDebug() << "Роль" << user.role() << " не поддерживается";
    }
}

QList<QSharedPointer<Lesson>> Schedule::lessons() const {
    return m_lessons;
}
