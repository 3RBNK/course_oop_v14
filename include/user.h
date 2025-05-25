#ifndef COURSE_USER_H
#define COURSE_USER_H

#include <QString>

class User {
public:
    User();
    User(int user_id, const QString &name, const QString &role,
         const QString &login, const QString &password);
    virtual ~User() = default;

    int id() const;
    QString name() const;
    QString role() const;

    void set_id(int id);
    void set_name(const QString &name);
    void set_role(const QString &role);

    QString login() const;
    QString password() const;

    bool operator==(const User &other) const;

protected:
    int m_id;
    QString m_name;
    QString m_role;
    QString m_login;
    QString m_password;
};

#endif //COURSE_USER_H