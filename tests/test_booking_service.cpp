#include <QtTest/QtTest>
#include "core/BookingService.h"

/**
 * @brief Test suite for BookingService functionality
 */
class TestBookingService : public QObject {
    Q_OBJECT

private slots:
    void initTestCase() {
        m_service = std::make_unique<BookingService>();
    }
    
    void cleanupTestCase() {
        m_service.reset();
    }
    
    /**
     * @brief Test that getMovies returns a non-empty list
     */
    void testGetMoviesReturnsNonEmptyList() {
        auto movies = m_service->getMovies();
        QVERIFY(!movies.isEmpty());
        QVERIFY(movies.size() >= 1);
    }
    
    /**
     * @brief Test that getTheaters returns a non-empty list
     */
    void testGetTheatersReturnsNonEmptyList() {
        auto movies = m_service->getMovies();
        QVERIFY(!movies.isEmpty());
        
        auto theaters = m_service->getTheaters(movies[0]->getId());
        QVERIFY(!theaters.isEmpty());
    }
    
    /**
     * @brief Test that each theater has 20 available seats initially
     */
    void testGetAvailableSeatsReturns20Seats() {
        auto movies = m_service->getMovies();
        auto theaters = m_service->getTheaters(movies[0]->getId());
        
        QVERIFY(!movies.isEmpty());
        QVERIFY(!theaters.isEmpty());
        
        auto seats = m_service->getAvailableSeats(theaters[0]->getId(), movies[0]->getId());
        QCOMPARE(seats.size(), 20);
    }
    
    /**
     * @brief Test single seat reservation succeeds
     */
    void testReserveSingleSeatSuccess() {
        auto movies = m_service->getMovies();
        auto theaters = m_service->getTheaters(movies[0]->getId());
        
        QVERIFY(!movies.isEmpty());
        QVERIFY(!theaters.isEmpty());
        
        QStringList seatIds = {"A1"};
        bool result = m_service->reserveSeats(theaters[0]->getId(), movies[0]->getId(),
                                             seatIds, "John Doe");
        
        QVERIFY(result);
        
        // Verify seat is no longer available
        auto availableSeats = m_service->getAvailableSeats(theaters[0]->getId(), movies[0]->getId());
        QCOMPARE(availableSeats.size(), 19);
    }
    
    /**
     * @brief Test multiple seat reservation succeeds
     */
    void testReserveMultipleSeatsSuccess() {
        auto service = std::make_unique<BookingService>();
        auto movies = service->getMovies();
        auto theaters = service->getTheaters(movies[0]->getId());
        
        QStringList seatIds = {"A1", "A2", "A3"};
        bool result = service->reserveSeats(theaters[0]->getId(), movies[0]->getId(),
                                           seatIds, "Jane Doe");
        
        QVERIFY(result);
        
        auto availableSeats = service->getAvailableSeats(theaters[0]->getId(), movies[0]->getId());
        QCOMPARE(availableSeats.size(), 17);
    }
    
    /**
     * @brief Test that reserving already reserved seat fails
     */
    void testReserveAlreadyReservedSeatFails() {
        auto service = std::make_unique<BookingService>();
        auto movies = service->getMovies();
        auto theaters = service->getTheaters(movies[0]->getId());
        
        QStringList seatIds = {"A1"};
        
        // First reservation
        bool result1 = service->reserveSeats(theaters[0]->getId(), movies[0]->getId(),
                                            seatIds, "Customer1");
        QVERIFY(result1);
        
        // Second reservation of same seat
        bool result2 = service->reserveSeats(theaters[0]->getId(), movies[0]->getId(),
                                            seatIds, "Customer2");
        QVERIFY(!result2);
    }
    
    /**
     * @brief Test that getBookings returns customer bookings
     */
    void testGetBookingsReturnsCustomerBookings() {
        auto service = std::make_unique<BookingService>();
        auto movies = service->getMovies();
        auto theaters = service->getTheaters(movies[0]->getId());
        
        QString customerName = "Test Customer";
        QStringList seatIds = {"A1", "A2"};
        
        service->reserveSeats(theaters[0]->getId(), movies[0]->getId(), seatIds, customerName);
        
        // Use getBookingData instead of getBookings for thread-safe access
        auto bookings = service->getBookingData(customerName);
        QCOMPARE(bookings.size(), 1);
        QCOMPARE(bookings[0].customerId, customerName);
        QCOMPARE(bookings[0].seatIds.size(), 2);
    }

private:
    std::unique_ptr<BookingService> m_service;
};

QTEST_MAIN(TestBookingService)
#include "test_booking_service.moc"
