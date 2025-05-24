#include "../include/auth_system.h"
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

AuthSystem::AuthSystem(QObject *parent) : QObject(parent) {
    try {
        load_users_from_file("D:\\home_work\\oop\\course\\code_v1\\users\\users.json");
    } catch (const std::exception& e) {
        qDebug() << "Ошибка загрузки пользователей:" << e.what();
    }
}


void AuthSystem::load_users_from_file(const QString& file_path) {
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
    
    // Очищаем текущий список пользователей
    qDeleteAll(m_users);
    m_users.clear();

    // Загружаем администраторов
    if (mainObj.contains("Администраторы")) {
        QJsonArray admins = mainObj["Администраторы"].toArray();
        for (const auto& adminRef : admins) {
            QJsonObject admin = adminRef.toObject();
            auto* newAdmin = new User(
                admin["user_id"].toInt(),
                admin["name"].toString(),
                admin["role"].toString(),
                admin["login"].toString(),
                admin["password"].toString()
            );
            m_users.append(newAdmin);
        }
    }

    // Загружаем преподавателей
    if (mainObj.contains("Преподаватели")) {
        QJsonArray teachers = mainObj["Преподаватели"].toArray();
        for (const auto& teacherRef : teachers) {
            QJsonObject teacher = teacherRef.toObject();
            
            // Собираем список предметов
            QStringList subjects;
            if (teacher.contains("subjects")) {
                for (const auto& subject : teacher["subjects"].toArray()) {
                    subjects.append(subject.toString());
                }
            }

            auto* newTeacher = new Teacher(
                teacher["user_id"].toInt(),
                teacher["name"].toString(),
                teacher["role"].toString(),
                teacher["login"].toString(),
                teacher["password"].toString(),
                teacher["teacher_id"].toInt(),
                subjects
            );
            m_users.append(newTeacher);
        }
    }

    // Загружаем группы и студентов
    if (mainObj.contains("Группы")) {
        QJsonArray groups = mainObj["Группы"].toArray();
        for (const auto& groupRef : groups) {
            QJsonObject groupObj = groupRef.toObject();
            
            // Создаем объект группы
            auto* newGroup = new Group(
                groupObj["user_id"].toInt(),
                groupObj["name"].toString(),
                groupObj["role"].toString(),
                groupObj["login"].toString(),
                groupObj["password"].toString(),
                groupObj["group_id"].toInt(),
                groupObj["course"].toInt()
            );
            m_users.append(newGroup);

            // Загружаем студентов группы
            if (groupObj.contains("студенты")) {
                QJsonArray students = groupObj["студенты"].toArray();
                for (const auto& studentRef : students) {
                    QJsonObject student = studentRef.toObject();
                    auto* newStudent = new Group(
                        student["user_id"].toInt(),
                        student["name"].toString(),
                        student["role"].toString(),
                        student["login"].toString(),
                        student["password"].toString(),
                        groupObj["group_id"].toInt(),
                        groupObj["course"].toInt()
                    );
                    m_users.append(newStudent);
                }
            }
        }
    }
}

void AuthSystem::save_to_json(const QString& filename) {
    QJsonObject mainObject;
    
    // Сохраняем администраторов
    QJsonArray adminsArray;
    for (const auto* user : m_users) {
        if (user->role() == "admin") {
            QJsonObject adminObj;
            adminObj["user_id"] = user->user_id();
            adminObj["name"] = user->name();
            adminObj["role"] = user->role();
            adminObj["login"] = user->login();
            adminObj["password"] = user->password();
            adminsArray.append(adminObj);
        }
    }
    mainObject["Администраторы"] = adminsArray;
    
    // Сохраняем преподавателей
    QJsonArray teachersArray;
    for (const auto* user : m_users) {
        if (const auto* teacher = dynamic_cast<const Teacher*>(user)) {
            QJsonObject teacherObj;
            teacherObj["user_id"] = teacher->user_id();
            teacherObj["name"] = teacher->name();
            teacherObj["role"] = teacher->role();
            teacherObj["login"] = teacher->login();
            teacherObj["password"] = teacher->password();
            teacherObj["teacher_id"] = teacher->teacher_id();
            
            // Сохраняем предметы
            QJsonArray subjectsArray;
            for (const QString& subject : teacher->subjects()) {
                subjectsArray.append(subject);
            }
            teacherObj["subjects"] = subjectsArray;
            
            // Сохраняем существующее расписание
            QFile readFile(filename);
            if (readFile.open(QIODevice::ReadOnly)) {
                QJsonDocument existingDoc = QJsonDocument::fromJson(readFile.readAll());
                readFile.close();
                
                if (existingDoc.isObject()) {
                    auto teachers = existingDoc.object()["Преподаватели"].toArray();
                    for (const auto& t : teachers) {
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
    
    // Сохраняем группы и студентов
    QJsonArray groupsArray;
    QMap<int, QJsonObject> groupObjects; // group_id -> group object
    
    // Сначала создаем объекты групп
    for (const auto* user : m_users) {
        if (const auto* group = dynamic_cast<const Group*>(user)) {
            if (group->role() == "group") {
                QJsonObject groupObj;
                groupObj["user_id"] = group->user_id();
                groupObj["name"] = group->name();
                groupObj["role"] = group->role();
                groupObj["login"] = group->login();
                groupObj["password"] = group->password();
                groupObj["group_id"] = group->group_id();
                groupObj["course"] = group->course();
                groupObj["студенты"] = QJsonArray();
                groupObj["расписание"] = QJsonArray();
                
                groupObjects[group->group_id()] = groupObj;
            }
        }
    }
    
    // Добавляем студентов в соответствующие группы
    for (const auto* user : m_users) {
        if (const auto* student = dynamic_cast<const Group*>(user)) {
            if (student->role() == "student" && groupObjects.contains(student->group_id())) {
                QJsonObject studentObj;
                studentObj["user_id"] = student->user_id();
                studentObj["name"] = student->name();
                studentObj["role"] = student->role();
                studentObj["login"] = student->login();
                studentObj["password"] = student->password();
                
                QJsonArray studentsArray = groupObjects[student->group_id()]["студенты"].toArray();
                studentsArray.append(studentObj);
                groupObjects[student->group_id()]["студенты"] = studentsArray;
            }
        }
    }
    
    // Сохраняем существующее расписание групп
    QFile readFile(filename);
    if (readFile.open(QIODevice::ReadOnly)) {
        QJsonDocument existingDoc = QJsonDocument::fromJson(readFile.readAll());
        readFile.close();
        
        if (existingDoc.isObject() && existingDoc.object().contains("Группы")) {
            auto existingGroups = existingDoc.object()["Группы"].toArray();
            for (const auto& existingGroup : existingGroups) {
                QString groupName = existingGroup.toObject()["name"].toString();
                for (auto& groupObj : groupObjects) {
                    if (groupObj["name"].toString() == groupName) {
                        groupObj["расписание"] = existingGroup.toObject()["расписание"];
                        break;
                    }
                }
            }
        }
    }
    
    // Преобразуем map в array
    for (const auto& groupObj : groupObjects) {
        groupsArray.append(groupObj);
    }
    
    mainObject["Группы"] = groupsArray;
    
    // Сохраняем в файл
    QFile writeFile(filename);
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


QList<User*> AuthSystem::get_users() const {
    return m_users;
}

User *AuthSystem::login(const QString &login, const QString &password) {
    for (User *user: m_users) {
        if (user->login() == login && user->password() == password) {
            return user;
        }
    }
    return nullptr;
}