#include "../include/auditorium.h"

Auditorium::Auditorium(const QString& auditorium_name)
        : m_auditorium_name(auditorium_name)
{
}


Auditorium::Auditorium()
        : m_room_id(0), m_capacity(0) {}

Auditorium::Auditorium(int roomId, int capacity, const QStringList &equipment)
        : m_room_id(roomId), m_capacity(capacity), m_equipment(equipment) {}

int Auditorium::room_id() const {
    return m_room_id;
}

void Auditorium::set_room_id(int id) {
    m_room_id = id;
}

int Auditorium::capacity() const {
    return m_capacity;
}

void Auditorium::set_capacity(int capacity) {
    m_capacity = capacity;
}

QStringList Auditorium::equipment() const {
    return m_equipment;
}

void Auditorium::set_equipment(const QStringList &equipment) {
    m_equipment = equipment;
}

QString Auditorium::auditorium_info() const {
    return QString("Ауд. %1 (мест: %2, оборудование: %3)")
            .arg(m_room_id)
            .arg(m_capacity)
            .arg(m_equipment.join(", "));
}

bool Auditorium::is_available(const QDateTime &dateTime) const {
    return !m_reserved_slots.contains(dateTime);
}

void Auditorium::reserve_slot(const QDateTime &dateTime) {
    if (!m_reserved_slots.contains(dateTime))
        m_reserved_slots.append(dateTime);
}

void Auditorium::release_slot(const QDateTime &dateTime) {
    m_reserved_slots.removeAll(dateTime);
}

QList<QDateTime> Auditorium::reserved_slots() const {
    return m_reserved_slots;
}
