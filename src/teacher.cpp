#include "../include/teacher.h"


Teacher::Teacher()
        : User(), m_subjects("") {
}

Teacher::Teacher(int id, const QString &name, const QString &role,
                 const QString &login, const QString &password,
                 const QStringList &subjects)
        : User(id, name, role, login, password),
          m_subjects(subjects) {
}

Teacher::Teacher(const QString &name)
        : User(), m_subjects({}) {
    m_name = name;
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
    return QString("%1")
            .arg(name());
}



