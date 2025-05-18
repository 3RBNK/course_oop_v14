#include "../include/schedule.h"


Schedule::Schedule(QWidget *parent) : QWidget(parent) {
    m_table = new QTableWidget(this);
    m_table->setColumnCount(m_days.size());
    m_table->setRowCount(m_time_slots.size());
    m_table->setHorizontalHeaderLabels(m_days);
    m_table->setVerticalHeaderLabels(m_time_slots);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    setLayout(layout);
}

void Schedule::add_lesson(const Lesson &lesson) {
    m_lessons.append(lesson);
}

void Schedule::remove_lesson(int lesson_id) {
    for (int i = 0; i < m_lessons.size(); ++i) {
        if (m_lessons[i].lesson_id() == lesson_id) {
            m_lessons.removeAt(i);
            break;
        }
    }
}

int Schedule::find_day_column(const QDateTime &dt) const {
    int dayOfWeek = dt.date().dayOfWeek();
    return dayOfWeek - 1;
}

int Schedule::find_time_row(const QDateTime &dt) const {
    QString timeStr = dt.time().toString("HH:mm");
    return m_time_slots.indexOf(timeStr);
}

void Schedule::draw_schedule() {
    m_table->clearContents();

    for (const Lesson &lesson : m_lessons) {
        int col = find_day_column(lesson.time_slot());
        int row = find_time_row(lesson.time_slot());

        if (row >= 0 && col >= 0) {
            QString cellText = QString("%1").arg(lesson.subject());
            QTableWidgetItem *item = new QTableWidgetItem(cellText);
            item->setTextAlignment(Qt::AlignCenter);
            m_table->setItem(row, col, item);
        }
    }
}

bool Schedule::check_conflicts() const {
    for (int i = 0; i < m_lessons.size(); ++i) {
        for (int j = i + 1; j < m_lessons.size(); ++j) {
            if (m_lessons[i].conflict_with(m_lessons[j])) {
                return true;
            }
        }
    }
    return false;
}

