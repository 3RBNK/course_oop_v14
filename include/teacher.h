#ifndef CODE_V1_TEACHER_H
#define CODE_V1_TEACHER_H

#include "user.h"
#include <QDateTime>
#include <QString>

class Teacher : public User {
public:
    Teacher();
    Teacher(int userId, const QString &name, const QString &role,
            const QString &login, const QString &password,
            int teacherId, const QString &subject);

    int teacher_id() const;
    QString subject() const;

    void set_teacher_id(int id);
    void set_subject(const QString &subject);

    QString teacher_info() const;

private:
    int m_teacher_id;
    QString m_subject;
};

#endif //CODE_V1_TEACHER_H