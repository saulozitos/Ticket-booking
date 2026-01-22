#include "models/Booking.h"

Booking::Booking(int id, const QString& customerId, int movieId, int theaterId,
                 const QStringList& seatIds, QObject* parent)
    : QObject(parent)
    , m_id(id)
    , m_customerId(customerId)
    , m_movieId(movieId)
    , m_theaterId(theaterId)
    , m_seatIds(seatIds)
    , m_bookingTime(QDateTime::currentDateTime())
{
    emit created();
}
