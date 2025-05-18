#include "../include/auth_system.h"

AuthSystem::AuthSystem(QObject *parent) : QObject(parent) {}

void AuthSystem::add_user(User *user) {
    m_users.append(user);
}

User* AuthSystem::login(const QString &login, const QString &password) {
    for (User *user : m_users) {
        if (user->login() == login && user->password() == password) {
            return user;
        }
    }
    return nullptr;
}