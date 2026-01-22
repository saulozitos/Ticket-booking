#pragma once

#include <QObject>
#include <QString>

/**
 * @brief Represents a seat in a theater
 * 
 * This class manages seat information including its identifier
 * and current reservation status. Thread-safe status updates
 * are handled by the parent BookingService.
 */
class Seat : public QObject {
    Q_OBJECT
    Q_PROPERTY(QString id READ getId CONSTANT)
    Q_PROPERTY(Status status READ getStatus WRITE setStatus NOTIFY statusChanged)

public:
    /**
     * @brief Enumeration of possible seat statuses
     */
    enum class Status {
        Available,  ///< Seat is available for booking
        Reserved,   ///< Seat has been reserved
        Occupied    ///< Seat is occupied (future use)
    };
    Q_ENUM(Status)
    
    /**
     * @brief Constructs a new Seat object
     * @param id Seat identifier (e.g., "A1", "A2")
     * @param status Initial seat status
     * @param parent Parent QObject for memory management
     */
    explicit Seat(const QString& id, Status status = Status::Available,
                  QObject* parent = nullptr);
    
    /**
     * @brief Gets the seat identifier
     * @return Seat ID (e.g., "A1")
     */
    const QString& getId() const { return m_id; }
    
    /**
     * @brief Gets the current seat status
     * @return Current status
     */
    Status getStatus() const { return m_status; }
    
    /**
     * @brief Sets the seat status
     * @param status New status
     */
    void setStatus(Status status);
    
    /**
     * @brief Checks if seat is available for booking
     * @return true if seat is available
     */
    bool isAvailable() const { return m_status == Status::Available; }
    
    /**
     * @brief Equality operator based on ID
     * @param other Seat to compare with
     * @return true if seats have the same ID
     */
    bool operator==(const Seat& other) const { return m_id == other.m_id; }
    
signals:
    /**
     * @brief Emitted when seat status changes
     * @param newStatus The new status
     */
    void statusChanged(Status newStatus);
    
private:
    QString m_id;       ///< Seat identifier
    Status m_status;    ///< Current status
};
