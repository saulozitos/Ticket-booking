#pragma once

#include "models/Movie.h"
#include "models/Theater.h"
#include "models/Seat.h"
#include "models/Booking.h"

#include <QObject>
#include <QVector>
#include <QMap>
#include <QMutex>
#include <QReadWriteLock>
#include <QThread>
#include <QDateTime>

/**
 * @brief Thread-safe booking service for cinema reservations
 * 
 * This service is the core of the booking system, handling all
 * booking operations with thread-safety guarantees to prevent
 * overbooking in concurrent scenarios. Uses Qt's threading
 * primitives (QMutex, QReadWriteLock) for synchronization.
 * 
 * The service maintains in-memory storage of movies, theaters,
 * seats, and bookings without relying on any database system.
 * Memory management is handled by Qt's parent-child system.
 * 
 * @note QObject children must be created in the same thread as parent.
 * This class handles multi-threaded booking by storing data separately
 * from QObject instances to avoid cross-thread QObject creation.
 */
class BookingService : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Simple booking data structure (non-QObject)
     * 
     * Used for thread-safe storage of booking information
     * without violating Qt's threading rules for QObjects.
     */
    struct BookingData {
        int id;                     ///< Booking ID
        QString customerId;         ///< Customer identifier
        int movieId;                ///< Movie ID
        int theaterId;              ///< Theater ID
        QStringList seatIds;        ///< Seat IDs
        QDateTime bookingTime;      ///< Booking timestamp
    };

    /**
     * @brief Constructs the booking service and initializes sample data
     * @param parent Parent QObject for memory management
     */
    explicit BookingService(QObject* parent = nullptr);
    
    /**
     * @brief Destructor
     */
    ~BookingService() override = default;
    
    // Prevent copying
    BookingService(const BookingService&) = delete;
    BookingService& operator=(const BookingService&) = delete;
    
    /**
     * @brief Gets all available movies (thread-safe)
     * @return Vector of movie pointers (caller must not delete)
     */
    QVector<Movie*> getMovies() const;
    
    /**
     * @brief Gets theaters showing a specific movie (thread-safe)
     * @param movieId Movie identifier
     * @return Vector of theater pointers (caller must not delete)
     */
    QVector<Theater*> getTheaters(int movieId) const;
    
    /**
     * @brief Gets available seats for a movie in a theater (thread-safe)
     * @param theaterId Theater identifier
     * @param movieId Movie identifier
     * @return Vector of available seat pointers (caller must not delete)
     */
    QVector<Seat*> getAvailableSeats(int theaterId, int movieId) const;
    
    /**
     * @brief Reserves seats atomically (thread-safe)
     * 
     * This method ensures that multiple concurrent requests cannot
     * reserve the same seats, preventing overbooking.
     * 
     * @param theaterId Theater identifier
     * @param movieId Movie identifier
     * @param seatIds List of seat IDs to reserve
     * @param customerName Customer name/identifier
     * @return true if reservation successful, false otherwise
     */
    bool reserveSeats(int theaterId, int movieId,
                     const QStringList& seatIds,
                     const QString& customerName);
    
    /**
     * @brief Gets all bookings for a customer (thread-safe)
     * @param customerName Customer name/identifier
     * @return Vector of booking pointers (caller must not delete)
     * @note May return empty if bookings were created in different threads
     */
    QVector<Booking*> getBookings(const QString& customerName) const;
    
    /**
     * @brief Gets booking data for a customer (thread-safe)
     * 
     * This method returns plain data structures instead of QObject pointers,
     * making it safe to call from any thread.
     * 
     * @param customerName Customer name/identifier
     * @return Vector of booking data structures
     */
    QVector<BookingData> getBookingData(const QString& customerName) const;
    
    /**
     * @brief Initializes sample data for testing purposes
     * 
     * Creates sample movies, theaters, and seat layouts.
     * Called automatically during construction.
     */
    void initializeSampleData();

signals:
    /**
     * @brief Emitted when a new booking is created
     * @param booking Pointer to the created booking
     */
    void bookingCreated(const Booking* booking);
    
    /**
     * @brief Emitted when seats are successfully reserved
     * @param theaterId Theater identifier
     * @param movieId Movie identifier
     * @param seatIds List of reserved seat IDs
     */
    void seatsReserved(int theaterId, int movieId, const QStringList& seatIds);
    
    /**
     * @brief Emitted when a reservation attempt fails
     * @param reason Reason for failure
     */
    void reservationFailed(const QString& reason);

private:
    /**
     * @brief Internal structure to manage seats per movie in a theater
     */
    struct TheaterSeats {
        QMap<int, QVector<Seat*>> movieSeats; ///< movieId -> seats
    };
    
    mutable QReadWriteLock m_readWriteLock;     ///< Read-write lock for optimized access
    mutable QMutex m_reservationMutex;          ///< Mutex for critical reservation operations
    
    QVector<Movie*> m_movies;                   ///< Movie objects (managed by Qt parent)
    QVector<Theater*> m_theaters;               ///< Theater objects (managed by Qt parent)
    QMap<int, TheaterSeats> m_theaterSeats;     ///< theaterId -> TheaterSeats
    QVector<Booking*> m_bookings;               ///< Booking objects (managed by Qt parent)
    QVector<BookingData> m_bookingData;         ///< Plain booking data (thread-safe)
    int m_nextBookingId;                        ///< Counter for booking IDs
    
    /**
     * @brief Creates a unique key for theater-movie combination
     * @param theaterId Theater identifier
     * @param movieId Movie identifier
     * @return Composite key string
     */
    QString makeKey(int theaterId, int movieId) const;
    
    /**
     * @brief Initializes seat layout for a specific theater-movie combination
     * @param theaterId Theater identifier
     * @param movieId Movie identifier
     */
    void initializeSeats(int theaterId, int movieId);
};
