#ifndef COURSE_TEACHER_H
#define COURSE_TEACHER_H

#include "user.h"
#include <QDateTime>
#include <QString>
#include <QStringList>

class Teacher : public User {
public:
    Teacher();
    Teacher(int id, const QString &name, const QString &role,
            const QString &login, const QString &password,
            const QStringList &subjects);
    Teacher(const QString &name);

    QStringList subjects() const;

    void add_subject(const QString &subject);
    void set_subjects(const QStringList &subjects);

    QString teacher_info() const;

private:
    QStringList m_subjects;
};

#endif //COURSE_TEACHER_H