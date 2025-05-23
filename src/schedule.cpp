#include "../include/schedule.h"
#include <iostream>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QDebug>


Schedule::Schedule(QWidget *parent)
        : QWidget(parent) {}

void Schedule::add_lesson(const QSharedPointer <Lesson> &lesson) {
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

        for (const QJsonValue &groupValue: groups) {
            QJsonObject groupObj = groupValue.toObject();
            QJsonArray members = groupObj.value("Одногруппники").toArray();

            bool userFound = false;
            for (const QJsonValue &memberValue: members) {
                QJsonObject memberObj = memberValue.toObject();
                if (memberObj.value("name").toString() == user.name()) {
                    userFound = true;
                    break;
                }
            }

            if (userFound) {
                QJsonArray schedule = groupObj.value("Расписание").toArray();
                for (const QJsonValue &lessonValue: schedule) {
                    QJsonObject lessonObj = lessonValue.toObject();

                    int lessonId = lessonObj.value("id").toInt();
                    QString subject = lessonObj.value("subject").toString();
                    QDateTime timeSlot = QDateTime::fromString(lessonObj.value("time_slot").toString(), Qt::ISODate);


                    QSharedPointer <Lesson> lesson = QSharedPointer<Lesson>::create(
                            lessonId, subject, Teacher(), QList < Group > (), timeSlot, Auditorium()
                    );

                    add_lesson(lesson);
                }
                break;
            }
        }
    } else if (user.role() == "teacher") {
        QJsonArray teachers = root.value("Преподаватели").toArray();

        for (const QJsonValue &teacherValue: teachers) {
            QJsonObject teacherObj = teacherValue.toObject();
            if (teacherObj.value("name").toString() == user.name()) {
                QJsonArray schedule = teacherObj.value("расписание").toArray();

                for (const QJsonValue &lessonValue: schedule) {
                    QJsonObject lessonObj = lessonValue.toObject();

                    int lessonId = lessonObj.value("id").toInt();
                    QString subject = lessonObj.value("subject").toString();
                    QDateTime timeSlot = QDateTime::fromString(lessonObj.value("time_slot").toString(), Qt::ISODate);

                    QSharedPointer <Lesson> lesson = QSharedPointer<Lesson>::create(
                            lessonId, subject, Teacher(), QList < Group > (), timeSlot, Auditorium()
                    );

                    add_lesson(lesson);
                }
                break;
            }
        }
    } else {
        qDebug() << "Роль" << user.role() << " не поддерживается";
    }
}

void Schedule::save_to_json(const QString &filePath, const User &user) {
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл для чтения:" << filePath;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Ошибка парсинга JSON:" << parseError.errorString();
        return;
    }

    QJsonObject rootObj = doc.object();

    QJsonArray lessonsArray;
    for (const auto &lesson: m_lessons) {
        QJsonObject lessonObj;
        lessonObj["subject"] = lesson->subject();
        lessonObj["datetime"] = lesson->time_slot().toString(Qt::ISODate);
        lessonObj["auditorium"] = lesson->auditorium().auditorium_info();
        lessonObj["teacher"] = lesson->teacher().teacher_info();
        lessonsArray.append(lessonObj);
    }

    if (user.role() == "student") {
        QJsonObject studentsObj = rootObj["Студенты"].toObject();
        QJsonArray groupsArray = studentsObj["Группы"].toArray();

        for (int i = 0; i < groupsArray.size(); ++i) {
            QJsonObject groupObj = groupsArray[i].toObject();
            QJsonArray studentsArray = groupObj["Одногруппники"].toArray();

            for (int j = 0; j < studentsArray.size(); ++j) {
                QJsonObject studentObj = studentsArray[j].toObject();
                if (studentObj["id"].toInt() == user.user_id()) {
                    studentObj["schedule"] = lessonsArray;
                    studentsArray[j] = studentObj;
                    groupObj["Одногруппники"] = studentsArray;
                    groupsArray[i] = groupObj;
                    break;
                }
            }
        }
        studentsObj["Группы"] = groupsArray;
        rootObj["Студенты"] = studentsObj;
    } else if (user.role() == "teacher") {
        QJsonArray teachersArray = rootObj["Преподаватели"].toArray();

        for (int i = 0; i < teachersArray.size(); ++i) {
            QJsonObject teacherObj = teachersArray[i].toObject();
            if (teacherObj["name"].toString() == user.name()) {
                teacherObj["schedule"] = lessonsArray;
                teachersArray[i] = teacherObj;
                break;
            }
        }
        rootObj["Преподаватели"] = teachersArray;
    }

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи:" << filePath;
        return;
    }

    QJsonDocument updatedDoc(rootObj);
    file.write(updatedDoc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Расписание успешно сохранено для пользователя:" << user.name();
}


QList<QSharedPointer < Lesson>>

Schedule::lessons() const {
    return m_lessons;
}