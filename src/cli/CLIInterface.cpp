#include "cli/CLIInterface.h"
#include <QTextStream>
#include <QCoreApplication>

CLIInterface::CLIInterface(QObject* parent)
    : QObject(parent)
    , m_service(nullptr)
    , m_selectedMovieId(-1)
    , m_selectedTheaterId(-1)
{
    // Create service with this as parent for automatic memory management
    m_service = new BookingService(this);
    
    // Connect signals
    connect(m_service, &BookingService::bookingCreated,
            this, &CLIInterface::onBookingCreated);
    connect(m_service, &BookingService::seatsReserved,
            this, &CLIInterface::onSeatsReserved);
}

void CLIInterface::run()
{
    QTextStream in(stdin);
    QTextStream out(stdout);
    
    out << "Welcome to Cinema Ticket Booking System!\n";
    out << "Please enter your name: ";
    out.flush();
    m_customerName = in.readLine();
    
    int choice;
    do {
        displayMenu();
        QString input = in.readLine();
        choice = input.toInt();
        
        clearScreen();
        
        switch (choice) {
            case 1: listMovies(); break;
            case 2: selectMovie(); break;
            case 3: listTheaters(); break;
            case 4: viewAvailableSeats(); break;
            case 5: reserveSeats(); break;
            case 6: viewMyBookings(); break;
            case 0: out << "Thank you for using our system!\n"; break;
            default: out << "Invalid option!\n"; break;
        }
        
        if (choice != 0) {
            pause();
        }
    } while (choice != 0);
}

void CLIInterface::displayMenu()
{
    QTextStream out(stdout);
    
    out << "\n=== CINEMA TICKET BOOKING SYSTEM ===\n";
    out << "1. List Movies\n";
    out << "2. Select Movie\n";
    out << "3. List Theaters\n";
    out << "4. View Available Seats\n";
    out << "5. Reserve Seats\n";
    out << "6. View My Bookings\n";
    out << "0. Exit\n";
    out << "\nChoose an option: ";
    out.flush();
}

void CLIInterface::listMovies()
{
    QTextStream out(stdout);
    out << "=== MOVIES CURRENTLY SHOWING ===\n\n";
    
    auto movies = m_service->getMovies();
    for (Movie* movie : movies) {
        out << "ID: " << movie->getId() << "\n";
        out << "Title: " << movie->getTitle() << "\n";
        out << "Duration: " << movie->getDuration() << " min\n";
        out << "Genre: " << movie->getGenre() << "\n";
        out << "---\n";
    }
}

void CLIInterface::selectMovie()
{
    QTextStream in(stdin);
    QTextStream out(stdout);
    
    out << "Enter movie ID: ";
    out.flush();
    
    QString input = in.readLine();
    m_selectedMovieId = input.toInt();
    
    auto movies = m_service->getMovies();
    Movie* selectedMovie = nullptr;
    
    for (Movie* movie : movies) {
        if (movie->getId() == m_selectedMovieId) {
            selectedMovie = movie;
            break;
        }
    }
    
    if (selectedMovie) {
        out << "Movie selected: " << selectedMovie->getTitle() << "\n";
    } else {
        out << "Movie not found!\n";
        m_selectedMovieId = -1;
    }
}

void CLIInterface::listTheaters()
{
    QTextStream out(stdout);
    
    if (m_selectedMovieId == -1) {
        out << "Please select a movie first!\n";
        return;
    }
    
    out << "=== AVAILABLE THEATERS ===\n\n";
    
    auto theaters = m_service->getTheaters(m_selectedMovieId);
    for (Theater* theater : theaters) {
        out << "ID: " << theater->getId() << "\n";
        out << "Name: " << theater->getName() << "\n";
        out << "Capacity: " << theater->getCapacity() << " seats\n";
        out << "---\n";
    }
}

void CLIInterface::viewAvailableSeats()
{
    QTextStream in(stdin);
    QTextStream out(stdout);
    
    if (m_selectedMovieId == -1) {
        out << "Please select a movie first!\n";
        return;
    }
    
    out << "Enter theater ID: ";
    out.flush();
    
    QString input = in.readLine();
    m_selectedTheaterId = input.toInt();
    
    auto seats = m_service->getAvailableSeats(m_selectedTheaterId, m_selectedMovieId);
    
    out << "\n=== AVAILABLE SEATS ===\n\n";
    out << "Total: " << seats.size() << " seats\n\n";
    
    for (int i = 0; i < seats.size(); ++i) {
        out << seats[i]->getId() << " ";
        if ((i + 1) % 10 == 0) {
            out << "\n";
        }
    }
    out << "\n";
}

void CLIInterface::reserveSeats()
{
    QTextStream in(stdin);
    QTextStream out(stdout);
    
    if (m_selectedMovieId == -1 || m_selectedTheaterId == -1) {
        out << "Please select a movie and theater first!\n";
        return;
    }
    
    out << "Enter seat IDs (comma-separated, e.g., A1,A2,A3): ";
    out.flush();
    
    QString input = in.readLine();
    QStringList seatIds = input.split(',', Qt::SkipEmptyParts);
    
    // Trim whitespace
    for (QString& seatId : seatIds) {
        seatId = seatId.trimmed();
    }
    
    if (m_service->reserveSeats(m_selectedTheaterId, m_selectedMovieId, seatIds, m_customerName)) {
        out << "\n✓ Booking successful!\n";
        out << "Reserved seats: " << seatIds.join(", ") << "\n";
    } else {
        out << "\n✗ Booking failed. Please check seat availability.\n";
    }
}

void CLIInterface::viewMyBookings()
{
    QTextStream out(stdout);
    
    // Use thread-safe BookingData instead of QObject pointers
    auto bookings = m_service->getBookingData(m_customerName);
    
    out << "=== MY BOOKINGS ===\n\n";
    
    if (bookings.isEmpty()) {
        out << "You have no bookings.\n";
        return;
    }
    
    auto movies = m_service->getMovies();
    auto theaters = m_service->getTheaters(0);
    
    for (const auto& booking : bookings) {
        out << "Booking #" << booking.id << "\n";
        
        // Find movie
        for (Movie* movie : movies) {
            if (movie->getId() == booking.movieId) {
                out << "Movie: " << movie->getTitle() << "\n";
                break;
            }
        }
        
        // Find theater
        for (Theater* theater : theaters) {
            if (theater->getId() == booking.theaterId) {
                out << "Theater: " << theater->getName() << "\n";
                break;
            }
        }
        
        out << "Seats: " << booking.seatIds.join(", ") << "\n";
        out << "Time: " << booking.bookingTime.toString(Qt::ISODate) << "\n";
        out << "---\n";
    }
}

void CLIInterface::onBookingCreated(const Booking* booking)
{
    // Can be used for logging or notifications
    Q_UNUSED(booking)
}

void CLIInterface::onSeatsReserved(int theaterId, int movieId, const QStringList& seatIds)
{
    // Can be used for logging or notifications
    Q_UNUSED(theaterId)
    Q_UNUSED(movieId)
    Q_UNUSED(seatIds)
}

void CLIInterface::clearScreen()
{
#ifdef Q_OS_WIN
    system("cls");
#else
    system("clear");
#endif
}

void CLIInterface::pause()
{
    QTextStream in(stdin);
    QTextStream out(stdout);
    
    out << "\nPress ENTER to continue...";
    out.flush();
    in.readLine();
}
