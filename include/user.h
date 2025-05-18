#ifndef CODE_V1_USER_H
#define CODE_V1_USER_H

#include <QObject>
#include <QString>

class User : public QObject {
Q_OBJECT

public:
    explicit User(QObject *parent = nullptr);
    User(int userId, const QString &name, const QString &role, const QString &login, const QString &password,
         QObject *parent = nullptr);

    int user_id() const;
    QString name() const;
    QString role() const;

    void set_user_id(int id);
    void set_name(const QString &name);
    void set_role(const QString &role);

    QString get_role() const;

    void view_schedule() const;

    QString login() const;
    QString password() const;
private:
    int m_user_id;
    QString m_name;
    QString m_role;
    QString m_login;
    QString m_password;
};

#endif //CODE_V1_USER_H
