#include "../include/auth_system.h"
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

AuthSystem::AuthSystem(QObject *parent) : QObject(parent) {
    try {
        load_users_from_file("D:\\home_work\\oop\\course\\code_v1\\users\\users.json");
    } catch (const std::exception &e) {
        qDebug() << "Error load users:" << e.what();
    }
}


void AuthSystem::load_users_from_file(const QString &file_path) {
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        throw std::runtime_error("Не удалось открыть файл для чтения");
    }

    QJsonDocument doc = QJsonDocument::fromJson(file.readAll());
    file.close();

    if (!doc.isObject()) {
        throw std::runtime_error("Неверный формат JSON");
    }

    QJsonObject mainObj = doc.object();

    qDeleteAll(m_users);
    m_users.clear();

    if (mainObj.contains("Администраторы")) {
        QJsonArray admins = mainObj["Администраторы"].toArray();
        for (const auto &adminRef: admins) {
            QJsonObject admin = adminRef.toObject();
            auto *newAdmin = new User(
                    admin["id"].toInt(),
                    admin["name"].toString(),
                    admin["role"].toString(),
                    admin["login"].toString(),
                    admin["password"].toString()
            );
            m_users.append(newAdmin);
        }
    }


    if (mainObj.contains("Преподаватели")) {
        QJsonArray teachers = mainObj["Преподаватели"].toArray();
        for (const auto &teacherRef: teachers) {
            QJsonObject teacher = teacherRef.toObject();

            QStringList subjects;
            if (teacher.contains("subjects")) {
                for (const auto &subject: teacher["subjects"].toArray()) {
                    subjects.append(subject.toString());
                }
            }

            auto *newTeacher = new Teacher(
                    teacher["id"].toInt(),
                    teacher["name"].toString(),
                    teacher["role"].toString(),
                    teacher["login"].toString(),
                    teacher["password"].toString(),
                    subjects
            );
            m_users.append(newTeacher);
        }
    }

    if (mainObj.contains("Группы")) {
        QJsonArray groups = mainObj["Группы"].toArray();
        for (const auto &groupRef: groups) {
            QJsonObject groupObj = groupRef.toObject();

            QList<User *> group_students;
            if (groupObj.contains("студенты")) {
                QJsonArray students = groupObj["студенты"].toArray();
                for (const auto &studentRef: students) {
                    QJsonObject student = studentRef.toObject();
                    User *newStudent = new User(
                            student["id"].toInt(),
                            student["name"].toString(),
                            "student",
                            student["login"].toString(),
                            student["password"].toString()
                    );
                    group_students.append(newStudent);
                }
            }

            int group_id = groupObj.value("id").toInt();
            QString group_name = groupObj.value("name").toString();
            int group_course = groupObj.value("course").toInt();

            auto *new_group = new Group(
                    group_id,
                    group_name,
                    "group",
                    "",
                    "",
                    group_course,
                    group_students);

            m_users.append(new_group);
        }
    }
}

void AuthSystem::save_users_to_json(const QString &file_path) {
    QJsonObject mainObject;

    QJsonArray adminsArray;
    for (const auto *user: m_users) {
        if (user->role() == "admin") {
            QJsonObject adminObj;
            adminObj["id"] = user->id();
            adminObj["name"] = user->name();
            adminObj["role"] = user->role();
            adminObj["login"] = user->login();
            adminObj["password"] = user->password();
            adminsArray.append(adminObj);
        }
    }
    mainObject["Администраторы"] = adminsArray;

    QJsonArray teachersArray;
    for (const auto *user: m_users) {
        if (const auto *teacher = dynamic_cast<const Teacher *>(user)) {
            QJsonObject teacherObj;
            teacherObj["id"] = teacher->id();
            teacherObj["name"] = teacher->name();
            teacherObj["role"] = teacher->role();
            teacherObj["login"] = teacher->login();
            teacherObj["password"] = teacher->password();

            QJsonArray subjectsArray;
            for (const QString &subject: teacher->subjects()) {
                subjectsArray.append(subject);
            }
            teacherObj["subjects"] = subjectsArray;

            QFile readFile(file_path);
            if (readFile.open(QIODevice::ReadOnly)) {
                QJsonDocument existingDoc = QJsonDocument::fromJson(readFile.readAll());
                readFile.close();

                if (existingDoc.isObject()) {
                    auto teachers = existingDoc.object()["Преподаватели"].toArray();
                    for (const auto &t: teachers) {
                        if (t.toObject()["name"].toString() == teacher->name()) {
                            teacherObj["расписание"] = t.toObject()["расписание"];
                            break;
                        }
                    }
                }
            }

            teachersArray.append(teacherObj);
        }
    }
    mainObject["Преподаватели"] = teachersArray;


    QJsonArray groupsArray;
    QMap<int, QJsonObject> groupObjects;
    int row = 0;

    QList<Group *> groups;
    for (auto *user: m_users) {
        if (auto *group = dynamic_cast<Group *>(user)) {
            if (group->role() == "group") {
                groups.append(group);
            }
        }
    }

    QMap < int, Group * > id_group_map;
    for (auto *group: groups) {
        int id = group->id();

        if (id_group_map.contains(id)) {
            QList<User *> old_student = id_group_map[id]->students();
            QList<User *> new_student = group->students();
            old_student.append(new_student);
            id_group_map[id]->set_student(old_student);
        } else {
            id_group_map[id] = group;
        }
    }

    for (auto *group: id_group_map) {
        QJsonObject groupObj;
        groupObj["id"] = group->id();
        groupObj["name"] = group->name();
        groupObj["role"] = group->role();
        groupObj["course"] = group->course();

        QJsonArray group_students;
        for (auto student: group->students()) {
            QJsonObject group_student;
            group_student["id"] = student->id();
            group_student["name"] = student->name();
            group_student["role"] = "student";
            group_student["login"] = student->login();
            group_student["password"] = student->password();

            group_students.append(group_student);
        }

        groupObj["студенты"] = group_students;
        groupObj["расписание"] = QJsonArray();
        groupObjects.insert(row, groupObj);
        row++;
    }

    QFile readFile(file_path);
    if (readFile.open(QIODevice::ReadOnly)) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(readFile.readAll());
        readFile.close();

        if (existingDoc.isObject() && existingDoc.object().contains("Группы")) {
            auto existingGroups = existingDoc.object()["Группы"].toArray();
            for (const auto &existingGroup: existingGroups) {
                QString groupName = existingGroup.toObject()["name"].toString();
                for (auto &groupObj: groupObjects) {
                    if (groupObj["name"].toString() == groupName) {
                        groupObj["расписание"] = existingGroup.toObject()["расписание"];
                        break;
                    }
                }
            }
        }
    }

    for (const auto &groupObj: groupObjects) {
        groupsArray.append(groupObj);
    }

    mainObject["Группы"] = groupsArray;

    QFile writeFile(file_path);
    if (!writeFile.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Не удалось открыть файл для записи");
    }

    QJsonDocument doc(mainObject);
    writeFile.write(doc.toJson(QJsonDocument::Indented));
    writeFile.close();
}


void AuthSystem::add_user(User *user) {
    m_users.append(user);
}


QList<User *> AuthSystem::get_users() const {
    return m_users;
}

User *AuthSystem::login(const QString &login, const QString &password) {
    for (User *user: m_users) {
        if (auto *group = dynamic_cast<Group *>(user)) {
            for (User *student: group->students()) {
                if (student->login() == login && student->password() == password) {
                    return group;
                }
            }
        } else {
            if (user->login() == login && user->password() == password) {
                return user;
            }
        }
    }
    return nullptr;
}