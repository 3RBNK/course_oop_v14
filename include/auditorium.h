#ifndef COURSE_AUDITORIUM_H
#define COURSE_AUDITORIUM_H

#include <QString>
#include <QStringList>
#include <QDateTime>

class Auditorium {
public:
    explicit Auditorium(const QString& auditorium_name);
    Auditorium();
    Auditorium(int room_id, const QString& auditorium_name, int capacity);

    int room_id() const;
    void set_room_id(int id);

    int capacity() const;
    void set_capacity(int capacity);

    QString name() const;
    void set_name(const QString &auditorium_name);

    QString auditorium_info() const;

    bool is_reserved() const;

private:
    int m_room_id;
    int m_capacity;
    QString m_auditorium_name;
    bool m_reserved_slots;
};

#endif //COURSE_AUDITORIUM_H