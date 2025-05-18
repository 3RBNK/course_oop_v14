#ifndef CODE_V1_LESSON_H
#define CODE_V1_LESSON_H

#include <QObject>
#include <QDateTime>
#include <QString>
#include <QList>

#include "teacher.h"
#include "group.h"
#include "auditorium.h"

class Lesson {
public:
    Lesson(int id,
           const QString &subject,
           const Teacher &teacher,
           const QList<Group> &groups,
           const QDateTime &time_slot,
           const Auditorium &auditorium);

    Lesson(const Lesson &other);

    int lesson_id() const;
    QString subject() const;
    QDateTime time_slot() const;

    Teacher teacher() const;
    QList<Group> groups() const;
    Auditorium auditorium() const;

    bool conflict_with(const Lesson &other) const;

private:
    int m_lesson_id;
    QString m_subject;
    Teacher m_teacher;
    QList<Group> m_groups;
    QDateTime m_time_slot;
    Auditorium m_auditorium;
};

#endif // CODE_V1_LESSON_H
