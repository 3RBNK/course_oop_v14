#include "../include/user_interface.h"

UserInterface::UserInterface(User *user, QWidget *parent)
        : Interface(parent), m_user(user)
{
    m_schedule = new Schedule(this);
    userInfoWidget = new QWidget(this);

    QHBoxLayout *mainLayout = new QHBoxLayout(this);

    populateUserInfo();
    mainLayout->addWidget(userInfoWidget, 1);
    mainLayout->addWidget(m_schedule, 3);

    setLayout(mainLayout);
}

void UserInterface::populateUserInfo() {
    QVBoxLayout *infoLayout = new QVBoxLayout(userInfoWidget);

    QLabel *nameLabel = new QLabel("Имя: " + m_user->name());
    QLabel *roleLabel = new QLabel("Роль: " + m_user->role());
    QLabel *idLabel = new QLabel(QString("ID: %1").arg(m_user->user_id()));

    infoLayout->addWidget(nameLabel);
    infoLayout->addWidget(roleLabel);
    infoLayout->addWidget(idLabel);
    infoLayout->addStretch();
}

void UserInterface::display() {
    show();
}

