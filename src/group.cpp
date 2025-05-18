#include "../include/group.h"

Group::Group(QObject *parent)
        : User(parent), m_group_id(0), m_course(0) {}

Group::Group(int userId, const QString &name, const QString &role,
             const QString &login, const QString &password,
             int groupId, int course, QObject *parent)
        : User(userId, name, role, login, password, parent),
          m_group_id(groupId), m_course(course) {}

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

QString Group::get_schedule(const QDateTime &date) const {
    return QString("Group schedule for %1: Course %2, Group ID %3")
            .arg(date.toString())
            .arg(m_course)
            .arg(m_group_id);
}

