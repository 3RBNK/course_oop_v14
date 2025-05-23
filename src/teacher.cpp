#include "../include/teacher.h"

Teacher::Teacher(const QString& teacher_name)
        : m_teacher_name(teacher_name)
{
}


Teacher::Teacher()
        : User(), m_teacher_id(0), m_subject("")
{
}

Teacher::Teacher(int userId, const QString &name, const QString &role,
                 const QString &login, const QString &password,
                 int teacherId, const QString &subject)
        : User(userId, name, role, login, password),
          m_teacher_id(teacherId),
          m_subject(subject)
{
}

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

QString Teacher::teacher_info() const {
    return QString("Ауд. %1 (мест: %2, оборудование: %3)")
            .arg(m_teacher_id)
            .arg(m_subject);
}


