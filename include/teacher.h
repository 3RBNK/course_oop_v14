#ifndef CODE_V1_TEACHER_H
#define CODE_V1_TEACHER_H

#include "user.h"
#include <QDateTime>
#include <QString>
#include <QStringList>

class Teacher : public User {
public:
    explicit Teacher(const QString& teacher_name);

    Teacher();
    Teacher(int user_id, const QString &name, const QString &role,
            const QString &login, const QString &password,
            int teacherId, const QStringList &subjects);

    int teacher_id() const;
    QStringList subjects() const;

    void set_teacher_id(int id);
    void add_subject(const QString &subject);
    void set_subjects(const QStringList &subjects);

    QString teacher_info() const;

private:
    int m_teacher_id;
    QString m_teacher_name;
    QStringList m_subjects;
};

#endif //CODE_V1_TEACHER_H