#include "../include/teacher.h"

Teacher::Teacher(const QString& teacher_name)
        : m_teacher_name(teacher_name)
{
}


Teacher::Teacher()
        : User(), m_teacher_id(0), m_subjects("")
{
}

Teacher::Teacher(int user_id, const QString &name, const QString &role,
                 const QString &login, const QString &password,
                 int teacherId, const QStringList &subjects)
        : User(user_id, name, role, login, password),
          m_teacher_id(teacherId),
          m_subjects(subjects)
{
}


int Teacher::teacher_id() const {
    return m_teacher_id;
}

QStringList Teacher::subjects() const {
    return m_subjects;
}

void Teacher::set_subjects(const QStringList &subjects) {
    m_subjects = subjects;
}

void Teacher::add_subject(const QString &subject) {
    if (!m_subjects.contains(subject)) {
        m_subjects.append(subject);
    }
}

QString Teacher::teacher_info() const {
    return QString("%1\nПредметы: %2")
            .arg(name())
            .arg(m_subjects.join(", "));
}



