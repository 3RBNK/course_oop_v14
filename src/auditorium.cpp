#include "../include/auditorium.h"

Auditorium::Auditorium(const QString& auditorium_name)
        : m_auditorium_name(auditorium_name)
{
}

Auditorium::Auditorium()
        : m_room_id(0), m_capacity(0) {}

Auditorium:: Auditorium(int room_id, const QString& auditorium_name, int capacity)
        : m_room_id(room_id), m_auditorium_name(auditorium_name), m_capacity(capacity) {}

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

QString Auditorium::name() const {
    return m_auditorium_name;
}

void Auditorium::set_name(const QString &auditorium_name) {
    m_auditorium_name = auditorium_name;
}

QString Auditorium::auditorium_info() const {
    return QString("Ауд. %1 (мест: %2)")
            .arg(m_room_id)
            .arg(m_capacity);
}

bool Auditorium::is_reserved() const {
    return m_reserved_slots;
}
