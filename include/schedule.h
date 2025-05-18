#ifndef CODE_V1_SCHEDULE_H
#define CODE_V1_SCHEDULE_H

#include <QObject>
#include <QList>
#include <QSharedPointer>
#include "lesson.h"

class Schedule : public QObject {
Q_OBJECT

public:
    explicit Schedule(QObject *parent = nullptr);

    void add_lesson(const QSharedPointer<Lesson> &lesson);
    void remove_lesson(int lesson_id);
    bool check_conflicts() const;

    QList<QSharedPointer<Lesson>> lessons() const;

private:
    QList<QSharedPointer<Lesson>> m_lessons;
};

#endif // CODE_V1_SCHEDULE_H
