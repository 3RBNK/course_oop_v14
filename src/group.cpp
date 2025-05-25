#include "../include/group.h"

Group::Group()
        : User(), m_course(0)
{
}

Group::Group(int id, const QString &name, const QString &role,
             const QString &login, const QString &password,
             int course, const QList<User*> &students)
        : User(id, name, role, login, password),
          m_course(course),
          m_students(students)
{
}


QList<User*> Group::students() const {
    return m_students;
}

int Group::course() const {
    return m_course;
}

void Group::set_course(int course) {
    m_course = course;
}

void Group::add_student(User *student) {
    if (!m_students.contains(student)) {
        m_students.append(student);
    }
}


void Group::delete_student(User *student) {
    m_students.removeAll(student);
}


void Group::set_student(const QList<User*> &students) {
    m_students = students;
}
