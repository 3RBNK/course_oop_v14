#include "../include/schedule.h"
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


void Schedule::load_from_json(const QString &file_path, const User &user) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл:" << file.errorString();
        return;
    }

    m_lessons.clear();

    QByteArray data = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(data);
    if (doc.isNull() || !doc.isObject()) {
        qWarning() << "Некорректный JSON";
        return;
    }

    QJsonObject root = doc.object();

    if (user.role() == "group") {
        QJsonArray groupsArray = root.value("Группы").toArray();

        for (const QJsonValue &groupValue : groupsArray) {
            QJsonObject groupObj = groupValue.toObject();
            int group_id = groupObj.value("id").toInt();

            bool userFound = group_id == user.id();

            if (userFound) {
                QJsonArray schedule = groupObj.value("расписание").toArray();
                for (const QJsonValue &lessonValue : schedule) {
                    QJsonObject lessonObj = lessonValue.toObject();

                    int lessonId = lessonObj.value("lesson_id").toInt();
                    QString subject = lessonObj.value("subject").toString();
                    QStringList subjects = {subject};
                    QString teacherName = lessonObj.value("teacher_name").toString();
                    int teacher_id = lessonObj.value("teacher_id").toInt();
                    QDateTime timeSlot = QDateTime::fromString(lessonObj.value("time_slot").toString(), Qt::ISODate);

                    Auditorium auditorium(
                        lessonObj.value("auditorium_id").toInt(),
                        lessonObj.value("name").toString(),
                        lessonObj.value("auditorium_capacity").toInt()
                    );

                    Teacher teacher(teacher_id, teacherName, "teacher", "", "", subjects);

                    QSharedPointer<Lesson> lesson = QSharedPointer<Lesson>::create(
                        lessonId, subject, teacher, QList<Group>(), timeSlot, auditorium
                    );

                    add_lesson(lesson);
                }
                break;
            }
        }
    } else if (user.role() == "teacher") {
        QJsonArray teachers = root.value("Преподаватели").toArray();

        for (const QJsonValue &teacherValue : teachers) {
            QJsonObject teacherObj = teacherValue.toObject();
            if (teacherObj.value("login").toString() == user.login()) {
                QJsonArray schedule = teacherObj.value("расписание").toArray();

                for (const QJsonValue &lessonValue : schedule) {
                    QJsonObject lessonObj = lessonValue.toObject();

                    int lessonId = lessonObj.value("lesson_id").toInt();
                    QString subject = lessonObj.value("subject").toString();
                    QDateTime timeSlot = QDateTime::fromString(lessonObj.value("time_slot").toString(), Qt::ISODate);

                    Auditorium auditorium(
                        lessonObj.value("auditorium_id").toInt(),
                        lessonObj.value("name").toString(),
                        lessonObj.value("auditorium_capacity").toInt()
                    );

                    QList<Group> groups;
                    QJsonArray group_names = lessonObj.value("group_names").toArray();
                    for (const QJsonValue &group_name : group_names) {
                        Group group(0, group_name.toString(), "group", "", "", 0, {});
                        groups.append(group);
                    }

                    QString teacher_name = teacherObj.value("name").toString();
                    QStringList subjects = {};
                    for (const QJsonValue &subject: teacherObj.value("subjects").toArray()) {
                        subjects.append(subject.toString());
                    }
                    Teacher teacher = Teacher(0, teacher_name, "teacher", "", "", subjects);

                    QSharedPointer<Lesson> lesson = QSharedPointer<Lesson>::create(
                        lessonId, subject, teacher, groups, timeSlot, auditorium
                    );

                    add_lesson(lesson);
                }
                break;
            }
        }
    } else {
        qDebug() << "Role" << user.role() << "unsupported";
    }
}

void Schedule::save_to_json(const QString &file_path, const User &user) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл для чтения:" << file_path;
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

        lessonObj["lesson_id"] = lesson->lesson_id();
        lessonObj["subject"] = lesson->subject();
        lessonObj["time_slot"] = lesson->time_slot().toString(Qt::ISODate);
        lessonObj["auditorium_capacity"] = lesson->auditorium().capacity();
        lessonObj["auditorium_id"] = lesson->auditorium().room_id();
        lessonObj["auditorium_name"] = lesson->auditorium().name();

        if (user.role() == "group") {
            lessonObj["teacher_id"] = lesson->teacher().id();
            lessonObj["teacher_name"] = lesson->teacher().name();
        } else if (user.role() == "teacher") {
            QJsonArray group_names;
            for (auto group_name: lesson->groups()) {
                group_names.append(group_name.name());
            }
            lessonObj["group_names"] = group_names;
        }

        lessonsArray.append(lessonObj);
    }

    if (user.role() == "group") {
        QJsonArray groupsArray = rootObj["Группы"].toArray();

        for (int i = 0; i < groupsArray.size(); ++i) {
            QJsonObject groupObj = groupsArray[i].toObject();
            int group_id = groupObj.value("id").toInt();
            if (group_id == user.id()) {
                groupObj["расписание"] = lessonsArray;
                groupsArray[i] = groupObj;
                break;
            }
        }
        rootObj["Группы"] = groupsArray;
    } else if (user.role() == "teacher") {
        QJsonArray teachersArray = rootObj["Преподаватели"].toArray();

        for (int i = 0; i < teachersArray.size(); ++i) {
            QJsonObject teacherObj = teachersArray[i].toObject();
            if (teacherObj["name"].toString() == user.name()) {
                teacherObj["расписание"] = lessonsArray;
                teachersArray[i] = teacherObj;
                break;
            }
        }
        rootObj["Преподаватели"] = teachersArray;
    }

    if (!file.open(QIODevice::WriteOnly)) {
        qWarning() << "Не удалось открыть файл для записи:" << file_path;
        return;
    }

    QJsonDocument updatedDoc(rootObj);
    file.write(updatedDoc.toJson(QJsonDocument::Indented));
    file.close();

    qDebug() << "Schedule save for: " << user.name();
}

QList<QSharedPointer<Lesson>> Schedule::lessons() const {
    return m_lessons;
}