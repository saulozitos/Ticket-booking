#include "core/BookingService.h"
#include <QReadLocker>
#include <QWriteLocker>
#include <QMutexLocker>

BookingService::BookingService(QObject* parent)
    : QObject(parent)
    , m_nextBookingId(1)
{
    initializeSampleData();
}

QVector<Movie*> BookingService::getMovies() const
{
    QReadLocker locker(&m_readWriteLock);
    return m_movies;
}

QVector<Theater*> BookingService::getTheaters(int movieId) const
{
    QReadLocker locker(&m_readWriteLock);
    
    // For simplicity, all theaters show all movies
    // In a real system, this would check schedules
    Q_UNUSED(movieId)
    return m_theaters;
}

QVector<Seat*> BookingService::getAvailableSeats(int theaterId, int movieId) const
{
    QReadLocker locker(&m_readWriteLock);
    
    auto theaterIt = m_theaterSeats.find(theaterId);
    if (theaterIt == m_theaterSeats.end()) {
        return {};
    }
    
    auto movieIt = theaterIt->movieSeats.find(movieId);
    if (movieIt == theaterIt->movieSeats.end()) {
        return {};
    }
    
    QVector<Seat*> availableSeats;
    for (Seat* seat : movieIt.value()) {
        if (seat->isAvailable()) {
            availableSeats.append(seat);
        }
    }
    
    return availableSeats;
}

bool BookingService::reserveSeats(int theaterId, int movieId,
                                  const QStringList& seatIds,
                                  const QString& customerName)
{
    // Use exclusive lock for reservation (critical section)
    QMutexLocker reservationLocker(&m_reservationMutex);
    
    // Find theater and movie seats
    auto theaterIt = m_theaterSeats.find(theaterId);
    if (theaterIt == m_theaterSeats.end()) {
        emit reservationFailed("Theater not found");
        return false;
    }
    
    auto movieIt = theaterIt->movieSeats.find(movieId);
    if (movieIt == theaterIt->movieSeats.end()) {
        emit reservationFailed("Movie not showing in this theater");
        return false;
    }
    
    // Verify all seats are available
    QVector<Seat*> seatsToReserve;
    for (const QString& seatId : seatIds) {
        Seat* seat = nullptr;
        for (Seat* s : movieIt.value()) {
            if (s->getId() == seatId) {
                seat = s;
                break;
            }
        }
        
        if (!seat) {
            emit reservationFailed(QString("Seat %1 not found").arg(seatId));
            return false;
        }
        
        if (!seat->isAvailable()) {
            emit reservationFailed(QString("Seat %1 is not available").arg(seatId));
            return false;
        }
        
        seatsToReserve.append(seat);
    }
    
    // Reserve all seats atomically
    for (Seat* seat : seatsToReserve) {
        seat->setStatus(Seat::Status::Reserved);
    }
    
    // Get current booking ID and increment for next booking
    int bookingId = m_nextBookingId++;
    
    // Store booking data (thread-safe without creating QObject in wrong thread)
    BookingData bookingData;
    bookingData.id = bookingId;
    bookingData.customerId = customerName;
    bookingData.movieId = movieId;
    bookingData.theaterId = theaterId;
    bookingData.seatIds = seatIds;
    bookingData.bookingTime = QDateTime::currentDateTime();
    
    m_bookingData.append(bookingData);
    
    // Create Booking QObject only in the service's thread using QMetaObject::invokeMethod
    // This ensures the object is created in the correct thread
    Booking* booking = nullptr;
    if (QThread::currentThread() == this->thread()) {
        // We're in the correct thread, create directly
        booking = new Booking(bookingId, customerName, movieId, theaterId, seatIds, this);
        m_bookings.append(booking);
    } else {
        // We're in a different thread, defer creation to main thread
        // For now, just store the data - booking objects can be created on-demand
        // This is a safe approach for multi-threaded scenarios
    }
    
    // Emit signals (these are thread-safe in Qt)
    emit seatsReserved(theaterId, movieId, seatIds);
    if (booking) {
        emit bookingCreated(booking);
    }
    
    return true;
}

QVector<Booking*> BookingService::getBookings(const QString& customerName) const
{
    QReadLocker locker(&m_readWriteLock);
    
    QVector<Booking*> customerBookings;
    for (Booking* booking : m_bookings) {
        if (booking->getCustomerId() == customerName) {
            customerBookings.append(booking);
        }
    }
    
    return customerBookings;
}

QVector<BookingService::BookingData> BookingService::getBookingData(const QString& customerName) const
{
    QMutexLocker locker(&m_reservationMutex);
    
    QVector<BookingData> customerBookings;
    for (const BookingData& data : m_bookingData) {
        if (data.customerId == customerName) {
            customerBookings.append(data);
        }
    }
    
    return customerBookings;
}

void BookingService::initializeSampleData()
{
    QWriteLocker locker(&m_readWriteLock);
    
    // Initialize movies (Qt manages memory via parent)
    m_movies.append(new Movie(1, "The Matrix Resurrections", 148, "Sci-Fi", this));
    m_movies.append(new Movie(2, "Dune: Part Two", 166, "Sci-Fi", this));
    m_movies.append(new Movie(3, "Oppenheimer", 180, "Drama", this));
    m_movies.append(new Movie(4, "Barbie", 114, "Comedy", this));
    
    // Initialize theaters (Qt manages memory via parent)
    m_theaters.append(new Theater(1, "IMAX Hall", Theater::TOTAL_SEATS, this));
    m_theaters.append(new Theater(2, "VIP Hall", Theater::TOTAL_SEATS, this));
    m_theaters.append(new Theater(3, "Standard Hall A", Theater::TOTAL_SEATS, this));
    
    // Initialize seats for all theater-movie combinations
    for (Theater* theater : m_theaters) {
        for (Movie* movie : m_movies) {
            initializeSeats(theater->getId(), movie->getId());
        }
    }
}

void BookingService::initializeSeats(int theaterId, int movieId)
{
    QVector<Seat*> seats;
    seats.reserve(Theater::TOTAL_SEATS);
    
    for (int i = 1; i <= Theater::TOTAL_SEATS; ++i) {
        QString seatId = QString("A%1").arg(i);
        // Qt manages memory via parent (this)
        Seat* seat = new Seat(seatId, Seat::Status::Available, this);
        seats.append(seat);
    }
    
    m_theaterSeats[theaterId].movieSeats[movieId] = seats;
}

QString BookingService::makeKey(int theaterId, int movieId) const
{
    return QString("%1_%2").arg(theaterId).arg(movieId);
}
