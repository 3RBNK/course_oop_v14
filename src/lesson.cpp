#include "../include/lesson.h"

Lesson::Lesson(int id,
               const QString &subject,
               const Teacher &teacher,
               const QList<Group> &groups,
               const QDateTime &time_slot,
               const Auditorium &auditorium)
        : m_lesson_id(id),
          m_subject(subject),
          m_teacher(teacher),
          m_groups(groups),
          m_time_slot(time_slot),
          m_auditorium(auditorium) {}

int Lesson::lesson_id() const {
    return m_lesson_id;
}

QString Lesson::subject() const {
    return m_subject;
}

QDateTime Lesson::time_slot() const {
    return m_time_slot;
}

Teacher Lesson::teacher() const {
    return m_teacher;
}

QList<Group> Lesson::groups() const {
    return m_groups;
}

Auditorium Lesson::auditorium() const {
    return m_auditorium;
}

bool Lesson::conflict_with(const Lesson &other) const {
    // Время занятий
    QDateTime thisEnd = m_time_slot.addSecs(90 * 60);  // 1.5 часа
    QDateTime otherEnd = other.time_slot().addSecs(90 * 60);
    bool overlaps = m_time_slot < otherEnd && other.time_slot() < thisEnd;

    bool teacherConflict = m_teacher.teacher_id() == other.teacher().teacher_id();
    bool roomConflict = m_auditorium.room_id() == other.auditorium().room_id();

    bool groupConflict = false;
    for (const Group &group : m_groups) {
        for (const Group &otherGroup : other.groups()) {
            if (group.group_id() == otherGroup.group_id()) {
                groupConflict = true;
                break;
            }
        }
        if (groupConflict) break;
    }

    return overlaps && (teacherConflict || roomConflict || groupConflict);
}
