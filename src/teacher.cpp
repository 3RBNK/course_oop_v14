#include "../include/teacher.h"

Teacher::Teacher(QObject *parent)
        : User(parent), m_teacher_id(0), m_subject("") {}

Teacher::Teacher(int userId, const QString &name, const QString &role,
                 const QString &login, const QString &password,
                 int teacherId, const QString &subject, QObject *parent)
        : User(userId, name, role, login, password, parent),
          m_teacher_id(teacherId), m_subject(subject) {}

int Teacher::teacher_id() const {
    return m_teacher_id;
}

QString Teacher::subject() const {
    return m_subject;
}

void Teacher::set_teacher_id(int id) {
    m_teacher_id = id;
}

void Teacher::set_subject(const QString &subject) {
    m_subject = subject;
}

QString Teacher::get_schedule(const QDateTime &date) const {
    return QString("Teacher schedule for %1: Subject %2, Teacher ID %3")
            .arg(date.toString())
            .arg(m_subject)
            .arg(m_teacher_id);
}

