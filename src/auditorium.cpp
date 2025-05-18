#include "../include/auditorium.h"

Auditorium::Auditorium(QObject *parent)
        : QObject(parent), m_room_id(0), m_capacity(0) {
}


Auditorium::Auditorium(int room_id, int capacity, const QStringList &equipment, QObject *parent)
        : QObject(parent),
          m_room_id(room_id),
          m_capacity(capacity),
          m_equipment(equipment) {
}

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
