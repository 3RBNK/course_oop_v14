#ifndef COURSE_AUTH_SYSTEM_H
#define COURSE_AUTH_SYSTEM_H

#include <QObject>
#include <QString>
#include <QList>


#include "user.h"
#include "group.h"
#include "teacher.h"

class AuthSystem: public QObject {
    Q_OBJECT
public:
    explicit AuthSystem(QObject *parent = nullptr);

    User* login(const QString &login, const QString &password);

    void add_user(User* user);
    QList<User*> get_users() const;
private:
    QList<User*> m_users;

    void load_users_from_file(const QString &file_path);
    void save_users_to_json(const QString& file_path);

    friend class AdminInterface;
};

#endif //COURSE_AUTH_SYSTEM_H
