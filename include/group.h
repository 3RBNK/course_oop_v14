#ifndef CODE_V1_GROUP_H
#define CODE_V1_GROUP_H

#include "user.h"
#include <QDateTime>

class Group : public User {
public:
    Group();
    Group(int userId, const QString &name, const QString &role,
          const QString &login, const QString &password,
          int groupId, int course);

    int group_id() const;
    int course() const;

    void set_group_id(int id);
    void set_course(int course);

private:
    int m_group_id;
    int m_course;
};

#endif //CODE_V1_GROUP_H