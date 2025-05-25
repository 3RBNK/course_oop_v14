#ifndef COURSE_CHECK_CORRECT_H
#define COURSE_CHECK_CORRECT_H

#include <QStringList>
#include "schedule.h"

class CheckCorrect  {
public:
    explicit CheckCorrect() = default;
    bool validate_schedule(const QList<QSharedPointer<Lesson>> &lessons);
    bool validate_users(const QList<User*> &users);
    QStringList errors() const;

private:
    QStringList m_errors;
};

#endif //COURSE_CHECK_CORRECT_H
