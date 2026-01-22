#include "models/Seat.h"

Seat::Seat(const QString& id, Status status, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_status(status)
{
}

void Seat::setStatus(Status status)
{
    if (m_status != status) {
        m_status = status;
        emit statusChanged(status);
    }
}
