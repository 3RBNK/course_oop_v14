#include "../include/user.h"
#include <QDebug>


User::User()
        : m_user_id(0),
          m_name(""),
          m_role(""),
          m_login(""),
          m_password("")
{
}

User::User(int user_id, const QString &name, const QString &role,
           const QString &login, const QString &password)
        : m_user_id(user_id),
          m_name(name),
          m_role(role),
          m_login(login),
          m_password(password)
{
}

int User::user_id() const {
    return m_user_id;
}

QString User::name() const {
    return m_name;
}

QString User::role() const {
    return m_role;
}

void User::set_user_id(int id) {
    m_user_id = id;
}

void User::set_name(const QString &name) {
    m_name = name;
}

void User::set_role(const QString &role) {
    m_role = role;
}

QString User::login() const {
    return m_login;
}

QString User::password() const {
    return m_password;
}
