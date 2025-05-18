//
// Created by david on 18.05.2025.
//

#ifndef CODE_V1_SCHEDULE_H
#define CODE_V1_SCHEDULE_H

#include <QWidget>
#include <QTableWidget>
#include <QVBoxLayout>
#include "lesson.h"

class Schedule : public QWidget {
Q_OBJECT

public:
    explicit Schedule(QWidget *parent = nullptr);

    void add_lesson(const Lesson &lesson);
    void remove_lesson(int lesson_id);
    void draw_schedule();
    bool check_conflicts() const;

private:
    QList<Lesson> m_lessons;
    QTableWidget *m_table;

    QStringList m_days = {"Пн", "Вт", "Ср", "Чт", "Пт", "Сб", "Вс"};
    QStringList m_time_slots = {
            "08:15", "10:00", "11:45", "14:20", "16:05"
    };

    int find_day_column(const QDateTime &dt) const;
    int find_time_row(const QDateTime &dt) const;
};

#endif //CODE_V1_SCHEDULE_H
