#ifndef COURSE_USER_INTERFACE_H
#define COURSE_USER_INTERFACE_H

#include "user.h"
#include "interface.h"
#include "schedule.h"
#include "auth_system.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QHeaderView>

class UserInterface : public Interface {
Q_OBJECT

public:
    explicit UserInterface(User *user, QWidget *parent = nullptr);

    void display() override;

private:
    User *m_user;
    AuthSystem *m_auth_system;
    Schedule *m_schedule;
    QWidget *user_info_widget;
    QTableWidget *schedule_table;

    void create_schedule_table();
    void populate_user_info();
    void search_free_auditorium();
};

#endif //COURSE_USER_INTERFACE_H
