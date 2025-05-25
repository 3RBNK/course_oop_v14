#include "../include/user_interface.h"
#include <QDialog>
#include <QDebug>
#include <QTableWidget>
#include <QHeaderView>
#include <QDateTime>
#include <QLabel>

UserInterface::UserInterface(User *user, QWidget *parent)
        : Interface(parent), m_user(user)
{
    m_schedule = new Schedule(this);
    m_schedule->load_from_json("D:\\home_work\\oop\\course\\code_v1\\users\\users.json", *user);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);

    user_info_widget = new QWidget(this);
    populate_user_info();
    mainLayout->addWidget(user_info_widget);

    create_schedule_table();
    mainLayout->addWidget(schedule_table);

    setLayout(mainLayout);
}

void UserInterface::populate_user_info() {
    QVBoxLayout *infoLayout = new QVBoxLayout(user_info_widget);

    if (auto *group = dynamic_cast<Group *>(m_user)) {
        infoLayout->addWidget(new QLabel(QString("ID группы: %1").arg(group->id())));
        infoLayout->addWidget(new QLabel("Название группы: " + group->name()));
        infoLayout->addWidget(new QLabel(QString("Курс: %1").arg(group->course())));

        const QList<User*> students = group->students();
        QStringList studentNames;
        for (User* student : students) {
            if (student && student->role() == "student") {
                studentNames << student->name();
            }
        }

        QLabel *studentsLabel = new QLabel("Студенты группы: " + (studentNames.isEmpty() ? "нет" : studentNames.join(", ")));
        infoLayout->addWidget(studentsLabel);
    } else if (m_user->role() == "teacher") {
        if (auto *teacher = dynamic_cast<Teacher *>(m_user)) {
            infoLayout->addWidget(new QLabel(QString("ID: %1").arg(teacher->id())));
            infoLayout->addWidget(new QLabel("Имя: " + teacher->name()));
            infoLayout->addWidget(new QLabel("Роль: " + teacher->role()));
            infoLayout->addWidget(new QLabel("Дисциплины: " + teacher->subjects().join(", ")));
        }
    }

    infoLayout->addStretch();
}

void UserInterface::create_schedule_table() {
    schedule_table = new QTableWidget(6, 6, this);

    QStringList days = {"Понедельник", "Вторник", "Среда", "Четверг", "Пятница", "Суббота"};
    QStringList times = {
            "08:15-09:50", "10:00-11:35", "11:45-13:20",
            "14:20-15:55", "16:05-17:40", "17:50-19:25"
    };

    schedule_table->setHorizontalHeaderLabels(days);
    schedule_table->setVerticalHeaderLabels(times);

    schedule_table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    schedule_table->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    for (const QSharedPointer<Lesson> &lesson : m_schedule->lessons()) {
        QDateTime time = lesson->time_slot();

        int day = time.date().dayOfWeek();
        if (day > 6) {
            continue;
        }

        QTime lesson_time = time.time();
        int row = -1;

        if (lesson_time >= QTime(8, 15) && lesson_time < QTime(9, 50)) row = 0;
        else if (lesson_time >= QTime(10, 0) && lesson_time < QTime(11, 35)) row = 1;
        else if (lesson_time >= QTime(11, 45) && lesson_time < QTime(13, 20)) row = 2;
        else if (lesson_time >= QTime(14, 20) && lesson_time < QTime(15, 55)) row = 3;
        else if (lesson_time >= QTime(16, 5) && lesson_time < QTime(17, 40)) row = 4;
        else if (lesson_time >= QTime(17, 50) && lesson_time < QTime(19, 25)) row = 5;

        if (row == -1) {
            continue;
        }

        QString text = lesson->subject() + "\n"
                       + lesson->auditorium().auditorium_info() + "\n"
                       + lesson->teacher().teacher_info();

        QTableWidgetItem *item = new QTableWidgetItem(text);
        item->setTextAlignment(Qt::AlignCenter);
        schedule_table->setItem(row, day - 1, item);
    }

    schedule_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    schedule_table->setSelectionMode(QAbstractItemView::NoSelection);
}

void UserInterface::display() {
    show();
}
