#include "../include/auth_system.h"
#include <QDebug>


AuthSystem::AuthSystem(QObject *parent)
        : QObject(parent) {
}

void AuthSystem::add_user(User *user) {
    m_users.append(user);
}

User *AuthSystem::login(const QString &login, const QString &password) {
    for (User *user: qAsConst(m_users)) {
        if (user->login() == login && user->password() == password) {
            qDebug() << "Login successful for user:" << user->name();
            return user;
        }
    }

    qDebug() << "Login failed: User does not exist.";
    return nullptr;
}