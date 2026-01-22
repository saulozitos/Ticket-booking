#pragma once

#include "core/BookingService.h"
#include <QObject>

/**
 * @brief Command-line interface for the booking system
 * 
 * Provides an interactive text-based interface for users to
 * interact with the booking service. Handles user input,
 * validation, and display of information.
 */
class CLIInterface : public QObject {
    Q_OBJECT

public:
    /**
     * @brief Constructs the CLI interface
     * @param parent Parent QObject for memory management
     */
    explicit CLIInterface(QObject* parent = nullptr);
    
    /**
     * @brief Runs the main CLI application loop
     * 
     * Displays menu, processes user input, and executes commands
     * until the user chooses to exit.
     */
    void run();
    
private slots:
    /**
     * @brief Handles booking created signal
     * @param booking Pointer to the created booking
     */
    void onBookingCreated(const Booking* booking);
    
    /**
     * @brief Handles seats reserved signal
     * @param theaterId Theater identifier
     * @param movieId Movie identifier
     * @param seatIds List of reserved seat IDs
     */
    void onSeatsReserved(int theaterId, int movieId, const QStringList& seatIds);
    
private:
    BookingService* m_service;  ///< Booking service instance (managed by Qt parent)
    int m_selectedMovieId;      ///< Currently selected movie ID
    int m_selectedTheaterId;    ///< Currently selected theater ID
    QString m_customerName;     ///< Customer name for bookings
    
    /**
     * @brief Displays the main menu
     */
    void displayMenu();
    
    /**
     * @brief Lists all available movies
     */
    void listMovies();
    
    /**
     * @brief Allows user to select a movie
     */
    void selectMovie();
    
    /**
     * @brief Lists theaters showing the selected movie
     */
    void listTheaters();
    
    /**
     * @brief Displays available seats for selected movie and theater
     */
    void viewAvailableSeats();
    
    /**
     * @brief Handles seat reservation process
     */
    void reserveSeats();
    
    /**
     * @brief Displays customer's bookings
     */
    void viewMyBookings();
    
    /**
     * @brief Clears the terminal screen
     */
    void clearScreen();
    
    /**
     * @brief Pauses execution until user presses Enter
     */
    void pause();
};
