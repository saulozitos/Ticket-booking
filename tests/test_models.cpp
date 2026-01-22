#include <QtTest/QtTest>
#include "models/Movie.h"
#include "models/Theater.h"
#include "models/Seat.h"
#include "models/Booking.h"

/**
 * @brief Test suite for model classes
 */
class TestModels : public QObject {
    Q_OBJECT

private slots:
    /**
     * @brief Test Movie constructor and getters
     */
    void testMovieConstructorAndGetters() {
        Movie movie(1, "Test Movie", 120, "Action");
        
        QCOMPARE(movie.getId(), 1);
        QCOMPARE(movie.getTitle(), QString("Test Movie"));
        QCOMPARE(movie.getDuration(), 120);
        QCOMPARE(movie.getGenre(), QString("Action"));
    }
    
    /**
     * @brief Test Movie equality operator
     */
    void testMovieEqualityOperator() {
        Movie movie1(1, "Movie A", 120, "Action");
        Movie movie2(1, "Movie B", 90, "Drama");
        Movie movie3(2, "Movie C", 120, "Action");
        
        QVERIFY(movie1 == movie2);  // Same ID
        QVERIFY(!(movie1 == movie3)); // Different ID
    }
    
    /**
     * @brief Test Theater constructor and getters
     */
    void testTheaterConstructorAndGetters() {
        Theater theater(1, "IMAX", 30);
        
        QCOMPARE(theater.getId(), 1);
        QCOMPARE(theater.getName(), QString("IMAX"));
        QCOMPARE(theater.getCapacity(), 30);
    }
    
    /**
     * @brief Test Theater default capacity
     */
    void testTheaterDefaultCapacity() {
        Theater theater(1, "Standard");
        QCOMPARE(theater.getCapacity(), Theater::TOTAL_SEATS);
    }
    
    /**
     * @brief Test Seat constructor and getters
     */
    void testSeatConstructorAndGetters() {
        Seat seat("A1");
        
        QCOMPARE(seat.getId(), QString("A1"));
        QCOMPARE(seat.getStatus(), Seat::Status::Available);
        QVERIFY(seat.isAvailable());
    }
    
    /**
     * @brief Test Seat status change
     */
    void testSeatStatusChange() {
        Seat seat("A1");
        
        QSignalSpy spy(&seat, &Seat::statusChanged);
        
        seat.setStatus(Seat::Status::Reserved);
        QCOMPARE(seat.getStatus(), Seat::Status::Reserved);
        QVERIFY(!seat.isAvailable());
        QCOMPARE(spy.count(), 1);
    }
    
    /**
     * @brief Test Booking constructor and getters
     */
    void testBookingConstructorAndGetters() {
        QStringList seatIds = {"A1", "A2", "A3"};
        Booking booking(1, "customer123", 5, 2, seatIds);
        
        QCOMPARE(booking.getId(), 1);
        QCOMPARE(booking.getCustomerId(), QString("customer123"));
        QCOMPARE(booking.getMovieId(), 5);
        QCOMPARE(booking.getTheaterId(), 2);
        QCOMPARE(booking.getSeatIds().size(), 3);
        QCOMPARE(booking.getSeatIds()[0], QString("A1"));
        QVERIFY(booking.getBookingTime().isValid());
    }
};

QTEST_MAIN(TestModels)
#include "test_models.moc"
