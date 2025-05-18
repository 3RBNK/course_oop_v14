#include "../include/lesson.h"


Lesson::Lesson(int id, const QString &subject, const QDateTime &time_slot)
        : m_lesson_id(id), m_subject(subject), m_time_slot(time_slot) {}

int Lesson::lesson_id() const {
    return m_lesson_id;
}

QString Lesson::subject() const {
    return m_subject;
}

QDateTime Lesson::time_slot() const {
    return m_time_slot;
}

bool Lesson::conflict_with(const Lesson &other) const {
    // Предположим, что пара длится 1.5 часа
    QDateTime thisEnd = m_time_slot.addSecs(90 * 60);
    QDateTime otherEnd = other.time_slot().addSecs(90 * 60);

    // Конфликт: если время пересекается
    bool overlaps = m_time_slot < otherEnd && other.time_slot() < thisEnd;
    return overlaps;
}