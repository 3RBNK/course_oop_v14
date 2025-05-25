#ifndef COURSE_USER_INTERFACE_H
#define COURSE_USER_INTERFACE_H

#include "user.h"
#include "interface.h"
#include "schedule.h"

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
    Schedule *m_schedule;
    QWidget *user_info_widget;
    QTableWidget *schedule_table;

    void create_schedule_table();
    void populate_user_info();
};

#endif //COURSE_USER_INTERFACE_H
