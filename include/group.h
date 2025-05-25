#ifndef COURSE_GROUP_H
#define COURSE_GROUP_H

#include "user.h"
#include <QDateTime>
#include <QList>
#include <QStringList>

class Group : public User {
public:
    Group();
    Group(int id, const QString &name, const QString &role,
          const QString &login, const QString &password,
          int course, const QList<User*> &students);

    QList<User*> students() const;

    int course() const;
    void set_course(int course);

    void add_student(User *student);
    void delete_student(User *student);

    void set_student(const QList<User*> &students);

private:
    int m_course;
    QList<User*> m_students;
};

#endif //COURSE_GROUP_H