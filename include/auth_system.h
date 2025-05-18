//
// Created by david on 18.05.2025.
//

#ifndef CODE_V1_AUTH_SYSTEM_H
#define CODE_V1_AUTH_SYSTEM_H

#include <QObject>
#include <QString>
#include <QList>

#include "user.h"


class AdminInterface;

class AuthSystem: public QObject {
    Q_OBJECT
public:
    explicit AuthSystem(QObject *parent = nullptr);
    User* login(const QString &login, const QString &password);
private:
    QList<User*> m_users;
    void add_user(User* user);
    friend class AdminInterface;
};


#endif //CODE_V1_AUTH_SYSTEM_H
