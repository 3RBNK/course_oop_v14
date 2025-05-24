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

QString Lesson::lesson_info() const {
    QString info;

    info += QString("Урок: %1\n").arg(m_subject);
    info += QString("Дата и время: %1\n").arg(m_time_slot.toString("dd.MM.yyyy hh:mm"));
    info += QString("Преподаватель: %1 (ID: %2, Предмет: %3)\n")
            .arg(m_teacher.name())
            .arg(m_teacher.teacher_id())
            .arg(m_teacher.subjects().join(", "));;

    info += "Группы:\n";
    for (const Group &group : m_groups) {
        info += QString("  - %1 (ID: %2, Курс: %3)\n")
                .arg(group.name())
                .arg(group.group_id())
                .arg(group.course());
    }

    info += QString("Аудитория: %1 (Вместимость: %2, Оборудование: %3)\n")
            .arg(m_auditorium.room_id())
            .arg(m_auditorium.capacity())
            .arg(m_auditorium.equipment().join(", "));

    return info;
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
    QDateTime this_end = m_time_slot.addSecs(90 * 60);
    QDateTime other_end = other.time_slot().addSecs(90 * 60);
    bool overlaps = m_time_slot < other_end && other.time_slot() < this_end;

    bool teacher_conflict = m_teacher.teacher_id() == other.teacher().teacher_id();
    bool room_conflict = m_auditorium.room_id() == other.auditorium().room_id();

    bool group_conflict = false;
    for (const Group &group : m_groups) {
        for (const Group &otherGroup : other.groups()) {
            if (group.group_id() == otherGroup.group_id()) {
                group_conflict = true;
                break;
            }
        }
        if (group_conflict) {
            break;
        }
    }

    return overlaps && (teacher_conflict || room_conflict || group_conflict);
}
