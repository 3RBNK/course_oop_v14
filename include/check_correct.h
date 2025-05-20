#ifndef CODE_V1_CHECK_CORRECT_H
#define CODE_V1_CHECK_CORRECT_H

#include <QObject>
#include <QStringList>
#include "schedule.h"

class CheckCorrect : public QObject {
Q_OBJECT

public:
    explicit CheckCorrect(QObject *parent = nullptr);
    bool validate_schedule(const QList<QSharedPointer<Lesson>> &lessons);
    QStringList errors() const;

private:
    QStringList m_errors;
};

#endif //CODE_V1_CHECK_CORRECT_H
