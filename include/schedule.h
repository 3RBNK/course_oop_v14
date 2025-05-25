#ifndef COURSE_SCHEDULE_H
#define COURSE_SCHEDULE_H

#include <QWidget>
#include <QList>
#include <QSharedPointer>
#include "lesson.h"

class Schedule : public QWidget {
Q_OBJECT

public:
    explicit Schedule(QWidget *parent = nullptr);

    void add_lesson(const QSharedPointer<Lesson> &lesson);
    void remove_lesson(int lesson_id);

    void load_from_json(const QString &file_path, const User &user);
    void save_to_json(const QString &file_path, const User &user);

    QList<QSharedPointer<Lesson>> lessons() const;

private:
    QList<QSharedPointer<Lesson>> m_lessons;
};

#endif // COURSE_SCHEDULE_H