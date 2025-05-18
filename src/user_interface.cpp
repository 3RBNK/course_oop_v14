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

void UserInterface::show_schedule() {
    // Заглушка: можно добавить получение уроков по m_user->user_id()
    m_schedule->add_lesson(Lesson(1, "ООП", QDateTime(QDate(2025, 5, 16), QTime(8, 15))));
    m_schedule->add_lesson(Lesson(2, "Иссл. опер.", QDateTime(QDate(2025, 5, 16), QTime(10, 0))));
    m_schedule->add_lesson(Lesson(3, "Выч. матем.", QDateTime(QDate(2025, 5, 14), QTime(11, 45))));

    m_schedule->draw_schedule();
}

