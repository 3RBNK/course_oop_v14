#ifndef CODE_V1_AUTH_SYSTEM_H
#define CODE_V1_AUTH_SYSTEM_H

#include <QObject>
#include <QString>
#include <QList>


#include "user.h"
#include "group.h"
#include "teacher.h"

class AdminInterface;

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
    void save_to_json(const QString& filename);


    friend class AdminInterface;
};

#endif //CODE_V1_AUTH_SYSTEM_H
