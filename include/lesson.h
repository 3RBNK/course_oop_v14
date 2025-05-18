#ifndef CODE_V1_LESSON_H
#define CODE_V1_LESSON_H

#include <QObject>
#include <QDateTime>

#include "teacher.h"
#include "group.h"
#include "auditorium.h"

class Lesson {
public:
    Lesson(int id, const QString &subject, const QDateTime &time_slot);

    int lesson_id() const;
    QString subject() const;
    QDateTime time_slot() const;

    bool conflict_with(const Lesson &other) const;

private:
    int m_lesson_id;
    QString m_subject;
    QDateTime m_time_slot;
};


#endif //CODE_V1_LESSON_H
