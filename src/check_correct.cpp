#include <QStringList>
#include <QDebug>
#include "../include/check_correct.h"


bool CheckCorrect::validate_users(const QList<User *> &users) {
    QStringList users_login;

    for (const auto *user: users) {
        if (const auto *teacher = dynamic_cast<const Teacher *>(user)) {
            users_login.append(teacher->login());
        } else if (const auto *group = dynamic_cast<const Group *>(user)) {
            for (const auto *student: group->students()) {
                users_login.append(student->login());
            }
        } else {
            users_login.append(user->login());
        }
    }

    qDebug() << "Valid users: " << users_login;

    for (int i = 0; i < users_login.size(); ++i) {
        for (int j = i + 1; j < users_login.size(); ++j) {
            if (users_login[i] == users_login[j]) {
                m_errors.append("Совпадают пользователи с логином " + users_login[i] + " и id" +
                                QString::number(users[i]->id()) + " и id" +
                                QString::number(users[j]->id()));

                return false;
            }
        }
    }
    return true;
}


bool CheckCorrect::validate_schedule(const QList<QSharedPointer<Lesson>> &lessons, int day_index, QTime lesson_time) {
    if (!lesson_time.isValid()) {
        m_errors.append("Формат времени неверный!");
        return false;
    }


    for (const auto &lesson: lessons) {
        if (lesson->time_slot().date().dayOfWeek() == day_index &&
            lesson->time_slot().time() == lesson_time) {
            m_errors.append("В это время уже есть занятие!");
            return false;
        }
    }
    return true;
}

QStringList CheckCorrect::errors() const {
    return m_errors;
}
