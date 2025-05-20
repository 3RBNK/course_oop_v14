#include "../include/group.h"

Group::Group()
        : User(), m_group_id(0), m_course(0)
{
}

Group::Group(int userId, const QString &name, const QString &role,
             const QString &login, const QString &password,
             int groupId, int course)
        : User(userId, name, role, login, password),
          m_group_id(groupId),
          m_course(course)
{
}

int Group::group_id() const {
    return m_group_id;
}

int Group::course() const {
    return m_course;
}

void Group::set_group_id(int id) {
    m_group_id = id;
}

void Group::set_course(int course) {
    m_course = course;
}

//Lesson* Group::get_subject(const QDateTime &date) const {
//    return nullptr;
//}

