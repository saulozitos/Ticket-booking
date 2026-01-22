#pragma once

#include <QObject>
#include <QString>
#include <QStringList>
#include <QDateTime>

/**
 * @brief Represents a booking/reservation
 * 
 * This class encapsulates all information related to a customer's
 * booking, including movie, theater, seats, and timestamp.
 */
class Booking : public QObject {
    Q_OBJECT
    Q_PROPERTY(int id READ getId CONSTANT)
    Q_PROPERTY(QString customerId READ getCustomerId CONSTANT)
    Q_PROPERTY(int movieId READ getMovieId CONSTANT)
    Q_PROPERTY(int theaterId READ getTheaterId CONSTANT)
    Q_PROPERTY(QStringList seatIds READ getSeatIds CONSTANT)
    Q_PROPERTY(QDateTime bookingTime READ getBookingTime CONSTANT)

public:
    /**
     * @brief Constructs a new Booking object
     * @param id Unique booking identifier
     * @param customerId Customer identifier/name
     * @param movieId Movie identifier
     * @param theaterId Theater identifier
     * @param seatIds List of reserved seat IDs
     * @param parent Parent QObject for memory management
     */
    explicit Booking(int id, const QString& customerId, int movieId, int theaterId,
                    const QStringList& seatIds, QObject* parent = nullptr);
    
    /**
     * @brief Gets the booking ID
     * @return Unique booking identifier
     */
    int getId() const { return m_id; }
    
    /**
     * @brief Gets the customer ID
     * @return Customer identifier
     */
    const QString& getCustomerId() const { return m_customerId; }
    
    /**
     * @brief Gets the movie ID
     * @return Movie identifier
     */
    int getMovieId() const { return m_movieId; }
    
    /**
     * @brief Gets the theater ID
     * @return Theater identifier
     */
    int getTheaterId() const { return m_theaterId; }
    
    /**
     * @brief Gets the list of reserved seat IDs
     * @return List of seat identifiers
     */
    const QStringList& getSeatIds() const { return m_seatIds; }
    
    /**
     * @brief Gets the booking timestamp
     * @return Date and time of booking
     */
    QDateTime getBookingTime() const { return m_bookingTime; }
    
signals:
    /**
     * @brief Emitted when booking is created
     */
    void created();
    
private:
    int m_id;                   ///< Unique booking identifier
    QString m_customerId;       ///< Customer identifier
    int m_movieId;              ///< Movie identifier
    int m_theaterId;            ///< Theater identifier
    QStringList m_seatIds;      ///< List of seat IDs
    QDateTime m_bookingTime;    ///< Booking timestamp
};
