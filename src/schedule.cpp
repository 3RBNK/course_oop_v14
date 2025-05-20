#include "../include/schedule.h"

Schedule::Schedule(QWidget *parent)
        : QWidget(parent) {}

void Schedule::add_lesson(const QSharedPointer<Lesson> &lesson) {
    m_lessons.append(lesson);
}

void Schedule::remove_lesson(int lesson_id) {
    for (int i = 0; i < m_lessons.size(); ++i) {
        if (m_lessons[i]->lesson_id() == lesson_id) {
            m_lessons.removeAt(i);
            break;
        }
    }
}

bool Schedule::check_conflicts() const {
    for (int i = 0; i < m_lessons.size(); ++i) {
        for (int j = i + 1; j < m_lessons.size(); ++j) {
            if (m_lessons[i]->conflict_with(*m_lessons[j])) {
                return true;
            }
        }
    }
    return false;
}

QList<QSharedPointer<Lesson>> Schedule::lessons() const {
    return m_lessons;
}
