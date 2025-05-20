#include "../include/auth_system.h"
#include <QFile>
#include <QDebug>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>

AuthSystem::AuthSystem(QObject *parent) : QObject(parent) {
    load_users_from_file("D:\\home_work\\oop\\course\\code_v1\\users\\users.json");
}

void AuthSystem::load_users_from_file(const QString &file_path)
{
    QFile file(file_path);
    if (!file.open(QIODevice::ReadOnly)) {
        qWarning() << "Не удалось открыть файл:" << file_path;
        return;
    }

    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parse_error;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parse_error);

    if (parse_error.error != QJsonParseError::NoError) {
        qWarning() << "Ошибка парсинга JSON:" << parse_error.errorString();
        return;
    }

    QJsonObject root_obj = doc.object();


    QJsonObject students_obj = root_obj.value("Студенты").toObject();
    QJsonArray groups_array = students_obj.value("Группы").toArray();

    for (const QJsonValue &group_val : groups_array) {
        QJsonObject group_obj = group_val.toObject();
        QJsonArray students_array = group_obj.value("Одногруппники").toArray();

        for (const QJsonValue &student_val : students_array) {
            QJsonObject student_obj = student_val.toObject();

            int user_id = student_obj.value("id").toInt();
            QString name = student_obj.value("name").toString();
            QString login = student_obj.value("login").toString();
            QString password = student_obj.value("password").toString();

            User *user = new User(user_id, name, "student", login, password);
            add_user(user);
        }
    }


    QJsonArray teachers_array = root_obj.value("Преподаватели").toArray();
    int next_teacher_id = 1000;

    for (const QJsonValue &teacher_val : teachers_array) {
        QJsonObject teacher_obj = teacher_val.toObject();

        QString name = teacher_obj.value("name").toString();
        QString login = teacher_obj.value("login").toString();
        QString password = teacher_obj.value("password").toString();

        User *user = new User(next_teacher_id++, name, "teacher", login, password);
        add_user(user);
    }


    QJsonArray admins_array = root_obj.value("Администраторы").toArray();
    int next_admin_id = 2000;

    for (const QJsonValue &admin_val : admins_array) {
        QJsonObject admin_obj = admin_val.toObject();

        QString name = admin_obj.value("name").toString();
        QString login = admin_obj.value("login").toString();
        QString password = admin_obj.value("password").toString();

        User *user = new User(next_admin_id++, name, "admin", login, password);
        add_user(user);
    }

    qDebug() << "Пользователи успешно загружены";
}


void AuthSystem::add_user(User *user) {
    m_users.append(user);
}

User *AuthSystem::login(const QString &login, const QString &password) {
    for (User *user: m_users) {
        if (user->login() == login && user->password() == password) {
            return user;
        }
    }
    return nullptr;
}