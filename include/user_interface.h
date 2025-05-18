#ifndef CODE_V1_USER_INTERFACE_H
#define CODE_V1_USER_INTERFACE_H

#include "user.h"
#include "interface.h"
#include "schedule.h"

#include <QLabel>
#include <QVBoxLayout>
#include <QHBoxLayout>


class UserInterface : public Interface {
Q_OBJECT

public:
    explicit UserInterface(User *user, QWidget *parent = nullptr);

    void display() override;
    void show_schedule();
private:
    User *m_user;
    Schedule *m_schedule;
    QWidget *userInfoWidget;

    void populateUserInfo();
};

#endif //CODE_V1_USER_INTERFACE_H
