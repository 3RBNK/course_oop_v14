#ifndef CODE_V1_AUDITORIUM_H
#define CODE_V1_AUDITORIUM_H

#include <QString>
#include <QStringList>
#include <QDateTime>

class Auditorium {
public:
    Auditorium();
    Auditorium(int roomId, int capacity, const QStringList &equipment);

    int room_id() const;
    void set_room_id(int id);

    int capacity() const;
    void set_capacity(int capacity);

    QStringList equipment() const;
    void set_equipment(const QStringList &equipment);

    QString auditorium_info() const;

    bool is_available(const QDateTime &dateTime) const;
    void reserve_slot(const QDateTime &date_time);
    void release_slot(const QDateTime &date_time);

    QList<QDateTime> reserved_slots() const;

private:
    int m_room_id;
    int m_capacity;
    QStringList m_equipment;
    QList<QDateTime> m_reserved_slots;
};

#endif //CODE_V1_AUDITORIUM_H