#include <QtTest/QtTest>
#include <QtConcurrent/QtConcurrent>
#include "core/BookingService.h"

/**
 * @brief Test suite for thread-safety of BookingService
 */
class TestThreadSafety : public QObject {
    Q_OBJECT

private slots:
    /**
     * @brief Test concurrent reservations don't cause overbooking
     */
    void testConcurrentReservationsNoOverbooking() {
        auto service = std::make_unique<BookingService>();
        
        auto movies = service->getMovies();
        auto theaters = service->getTheaters(movies[0]->getId());
        
        QVERIFY(!movies.isEmpty());
        QVERIFY(!theaters.isEmpty());
        
        int theaterId = theaters[0]->getId();
        int movieId = movies[0]->getId();
        
        QAtomicInt successCount = 0;
        QAtomicInt failCount = 0;
        
        const int NUM_THREADS = 20;
        
        // Each thread tries to reserve A1
        auto reservationTask = [&service, theaterId, movieId, &successCount, &failCount](int threadId) {
            QStringList seatIds = {"A1"};
            bool result = service->reserveSeats(theaterId, movieId, seatIds,
                                               QString("Customer%1").arg(threadId));
            if (result) {
                successCount.fetchAndAddOrdered(1);
            } else {
                failCount.fetchAndAddOrdered(1);
            }
        };
        
        QVector<QFuture<void>> futures;
        for (int i = 0; i < NUM_THREADS; ++i) {
            futures.append(QtConcurrent::run(reservationTask, i));
        }
        
        // Wait for all threads to complete
        for (auto& future : futures) {
            future.waitForFinished();
        }
        
        // Only one thread should succeed
        QCOMPARE(successCount.loadAcquire(), 1);
        QCOMPARE(failCount.loadAcquire(), NUM_THREADS - 1);
        
        // Verify only 19 seats remain
        auto availableSeats = service->getAvailableSeats(theaterId, movieId);
        QCOMPARE(availableSeats.size(), 19);
    }
    
    /**
     * @brief Test concurrent reservations of different seats all succeed
     */
    void testConcurrentDifferentSeatsAllSucceed() {
        auto service = std::make_unique<BookingService>();
        
        auto movies = service->getMovies();
        auto theaters = service->getTheaters(movies[0]->getId());
        
        int theaterId = theaters[0]->getId();
        int movieId = movies[0]->getId();
        
        QAtomicInt successCount = 0;
        
        const int NUM_THREADS = 10;
        
        // Each thread reserves different seat
        auto reservationTask = [&service, theaterId, movieId, &successCount](int threadId) {
            QStringList seatIds = {QString("A%1").arg(threadId + 1)};
            bool result = service->reserveSeats(theaterId, movieId, seatIds,
                                               QString("Customer%1").arg(threadId));
            if (result) {
                successCount.fetchAndAddOrdered(1);
            }
        };
        
        QVector<QFuture<void>> futures;
        for (int i = 0; i < NUM_THREADS; ++i) {
            futures.append(QtConcurrent::run(reservationTask, i));
        }
        
        for (auto& future : futures) {
            future.waitForFinished();
        }
        
        // All should succeed
        QCOMPARE(successCount.loadAcquire(), NUM_THREADS);
        
        // Verify correct number of seats remain
        auto availableSeats = service->getAvailableSeats(theaterId, movieId);
        QCOMPARE(availableSeats.size(), 20 - NUM_THREADS);
    }
    
    /**
     * @brief Test concurrent multiple seat reservations
     */
    void testConcurrentMultipleSeatReservations() {
        auto service = std::make_unique<BookingService>();
        
        auto movies = service->getMovies();
        auto theaters = service->getTheaters(movies[0]->getId());
        
        int theaterId = theaters[0]->getId();
        int movieId = movies[0]->getId();
        
        QAtomicInt successCount = 0;
        
        const int NUM_THREADS = 5;
        
        // Each thread reserves multiple consecutive seats
        auto reservationTask = [&service, theaterId, movieId, &successCount](int threadId) {
            int startSeat = threadId * 3 + 1;
            QStringList seatIds;
            for (int j = 0; j < 3; ++j) {
                seatIds.append(QString("A%1").arg(startSeat + j));
            }
            
            bool result = service->reserveSeats(theaterId, movieId, seatIds,
                                               QString("Customer%1").arg(threadId));
            if (result) {
                successCount.fetchAndAddOrdered(1);
            }
        };
        
        QVector<QFuture<void>> futures;
        for (int i = 0; i < NUM_THREADS; ++i) {
            futures.append(QtConcurrent::run(reservationTask, i));
        }
        
        for (auto& future : futures) {
            future.waitForFinished();
        }
        
        // All should succeed (non-overlapping seats)
        QCOMPARE(successCount.loadAcquire(), NUM_THREADS);
        
        auto availableSeats = service->getAvailableSeats(theaterId, movieId);
        QCOMPARE(availableSeats.size(), 20 - (NUM_THREADS * 3));
    }
};

QTEST_MAIN(TestThreadSafety)
#include "test_thread_safety.moc"
