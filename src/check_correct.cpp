#include "../include/check_correct.h"


CheckCorrect::CheckCorrect(QObject *parent)
        : QObject(parent) {}

bool CheckCorrect::validate_schedule(const QList<QSharedPointer<Lesson>> &lessons) {
    for (int i = 0; i < lessons.size(); ++i) {
        for (int j = i + 1; j < lessons.size(); ++j) {
            if (lessons[i]->conflict_with(*lessons[j])) {
                m_errors.append("Conflict between lessons " +
                              QString::number(lessons[i]->lesson_id()) + " and " +
                              QString::number(lessons[j]->lesson_id()));
                return false;
            }
        }
    }
    return true;
}

QStringList CheckCorrect::errors() const {
    return m_errors;
}
