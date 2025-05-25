#include "../include/check_correct.h"


bool CheckCorrect::validate_schedule(const QList<QSharedPointer<Lesson>> &lessons) {
    for (int i = 0; i < lessons.size(); ++i) {
        for (int j = i + 1; j < lessons.size(); ++j) {
            if (*lessons[i] == *lessons[j]) {
                m_errors.append("Conflict between lessons " +
                              QString::number(lessons[i]->lesson_id()) + " and " +
                              QString::number(lessons[j]->lesson_id()));
                return false;
            }
        }
    }
    return true;
}


bool CheckCorrect::validate_users(const QList<User*> &users) {
    for (int i = 0; i < users.size(); ++i) {
        for (int j = i + 1; j < users.size(); ++j) {
            if (*users[i] == *users[j]) {
                m_errors.append("Conflict between users " +
                                users[i]->login() + " and " +
                                users[j]->login());
                return false;
            }
        }
    }
    return true;
}

QStringList CheckCorrect::errors() const {
    return m_errors;
}
