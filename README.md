# Ticket Booking System

Cinema ticket booking system built with C++20 and Qt6.



## 📋 Description

Thread-safe backend service for cinema ticket booking with CLI interface. Supports multiple concurrent requests without overbooking using Qt's threading capabilities.



## ✨ Features

- ✅ View all movies currently showing

- ✅ Select a movie

- ✅ View available cinema halls showing the selected movie

- ✅ View available seats (20 seats per hall)

- ✅ Reserve multiple seats simultaneously

- ✅ Thread-safe operations (no overbooking)

- ✅ 100% documented codebase

- ✅ Full unit test coverage

- ✅ Doxygen documentation

  

## 🏗️ Architecture
```txt
ticket-booking/
├── src/
│ ├── core/
│ │ └── BookingService.cpp
│ ├── models/
│ │ ├── Movie.cpp
│ │ ├── Theater.cpp
│ │ ├── Seat.cpp
│ │ └── Booking.cpp
│ └── cli/
│ ├── main.cpp
│ └── CLIInterface.cpp
├── include/
│ ├── core/
│ │ └── BookingService.h
│ ├── models/
│ │ ├── Movie.h
│ │ ├── Theater.h
│ │ ├── Seat.h
│ │ └── Booking.h
│ └── cli/
│ └── CLIInterface.h
├── tests/
│ ├── test_booking_service.cpp
│ ├── test_models.cpp
│ └── test_thread_safety.cpp
├── docs/ # Documentation
│ └── Doxyfile
├── CMakeLists.txt
├── conanfile.txt 
├── Dockerfile
└── README.md
```



## 🔧 Requirements

### Option 1: Local Build

- **C++ Compiler**: GCC 11+, Clang 13+, or MSVC 2022+
- **CMake**: 3.20 or higher
- **Qt6**: 6.5 or higher (tested with Qt 6.8.2)

---



## 🚀 Build Instructions

### Method 1: Qt Creator (Recommended for Development)

1. **Install Qt Creator**
   ```bash
   # Download from https://www.qt.io/download
   # Or install via package manager (example for Ubuntu)
   sudo apt-get install qtcreator

1. **Open Project**
   - Launch Qt Creator
   - File → Open File or Project
   - Select [CMakeLists.txt](vscode-file://vscode-app/opt/visual-studio-code/resources/app/out/vs/code/electron-browser/workbench/workbench.html) in the project root
   - Choose Qt 6.8.2+ kit when prompted
2. **Configure Build**
   - Select "Desktop Qt 6.8.2" kit
   - Build type: Debug or Release
3. **Build**
   - Press `Ctrl+B` or click Build button
   - Wait for compilation to complete
4. **Run**
   - Press `Ctrl+R` or click Run button
   - The CLI application will start



### Method 2: Command Line (Linux/macOS)

1. **Install Dependencies**

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake qt6-base-dev

# Fedora/RHEL
sudo dnf install gcc-c++ cmake qt6-qtbase-devel

# macOS (using Homebrew)
brew install cmake qt@6
```

2. **Set Qt Environment**

```bash
# Ubuntu/Debian (if Qt installed via package manager)
export Qt6_DIR=/usr/lib/x86_64-linux-gnu/cmake/Qt6

# Custom Qt installation
export Qt6_DIR=/opt/Qt/6.8.2/gcc_64
export PATH=$Qt6_DIR/bin:$PATH
export LD_LIBRARY_PATH=$Qt6_DIR/lib:$LD_LIBRARY_PATH
```

3. **Build Project**

```bash
# Navigate to project directory
cd Ticket-booking

# Create build directory
mkdir -p build && cd build

# Configure with CMake
cmake .. -DCMAKE_PREFIX_PATH=$Qt6_DIR -DCMAKE_BUILD_TYPE=Release

# Build (use all CPU cores)
cmake --build . -j$(nproc)
```

4. **Run Application**

```bash
# From build directory
./bin/ticket-booking-cli
```

---

## 📖 Using the Application

### Starting the Application

```bash
# From build directory
./bin/ticket-booking-cli

# Or from project root
./build/bin/ticket-booking-cli
```

### CLI Interface

When you start the application, you'll see:

```bash
Welcome to Cinema Ticket Booking System!
Please enter your name: John Doe

=== CINEMA TICKET BOOKING SYSTEM ===

1. List Movies
2. Select Movie
3. List Theaters
4. View Available Seats
5. Reserve Seats
6. View My Bookings
0. Exit

Choose an option:
```

### Workflow Example

**Step 1: List Movies**

```bash
Choose an option: 1

=== MOVIES CURRENTLY SHOWING ===

ID: 1
Title: The Matrix Resurrections
Duration: 148 min
Genre: Sci-Fi
---
ID: 2
Title: Dune: Part Two
Duration: 166 min
Genre: Sci-Fi
---
ID: 3
Title: Oppenheimer
Duration: 180 min
Genre: Drama
---
ID: 4
Title: Barbie
Duration: 114 min
Genre: Comedy
---
```

**Step 2: Select a Movie**

```bash
Choose an option: 2
Enter movie ID: 1
Movie selected: The Matrix Resurrections
```

**Step 3: View Theaters**

```bash
Choose an option: 3

=== AVAILABLE THEATERS ===

ID: 1
Name: IMAX Hall
Capacity: 20 seats
---
ID: 2
Name: VIP Hall
Capacity: 20 seats
---
ID: 3
Name: Standard Hall A
Capacity: 20 seats
---
```

**Step 4: View Available Seats**

```bash
Choose an option: 4
Enter theater ID: 1

=== AVAILABLE SEATS ===

Total: 20 seats

A1 A2 A3 A4 A5 A6 A7 A8 A9 A10
A11 A12 A13 A14 A15 A16 A17 A18 A19 A20
```

**Step 5: Reserve Seats**

```bash
Choose an option: 5
Enter seat IDs (comma-separated, e.g., A1,A2,A3): A5,A6,A7

✓ Booking successful!
Reserved seats: A5, A6, A7
```

**Step 6: View Your Bookings**

```bash
Choose an option: 6

=== MY BOOKINGS ===

Booking #1
Movie: The Matrix Resurrections
Theater: IMAX Hall
Seats: A5, A6, A7
Time: 2024-01-15T10:30:45
---
```

**Step 7: Exit**

```bash
Choose an option: 0
Thank you for using our system!
```



## 🧪 Testing

```bash
cd build

# Run all tests
ctest --verbose

# Or run individual test suites
./bin/test-booking-service
./bin/test-models
./bin/test-thread-safety

# Run with Qt Test options
./bin/test-booking-service -v2  # Verbose output
./bin/test-thread-safety -silent  # Silent mode
```

**Test Coverage:**

- ✅ Simple seat reservation
- ✅ Concurrent reservations (thread-safety)
- ✅ Overbooking prevention
- ✅ Seat validation
- ✅ Movie and theater management
- ✅ Qt signals/slots mechanism

**Test Suites:**

1. **test-booking-service**: Core booking functionality
2. **test-models**: Model classes (Movie, Theater, Seat, Booking)
3. **test-thread-safety**: Concurrent operations and thread safety



## 📚 API Documentation

### Generate Documentation

```bash
# Install Doxygen
sudo apt-get install doxygen graphviz  # Ubuntu/Debian
brew install doxygen graphviz          # macOS

# Generate docs
cd docs
doxygen Doxyfile

# View documentation
xdg-open html/index.html  # Linux
open html/index.html      # macOS
```



### Core API Example

```cpp
#include "core/BookingService.h"

// Create service
BookingService service;

// Get movies
QVector<Movie*> movies = service.getMovies();

// Get theaters for a movie
QVector<Theater*> theaters = service.getTheaters(movieId);

// Get available seats
QVector<Seat*> seats = service.getAvailableSeats(theaterId, movieId);

// Reserve seats (thread-safe)
QStringList seatIds = {"A1", "A2"};
bool success = service.reserveSeats(theaterId, movieId, seatIds, "Customer Name");

// Get bookings (thread-safe)
QVector<BookingService::BookingData> bookings = service.getBookingData("Customer Name");
```



---

## 🎯 Design Decisions

1. **In-Memory Storage**: No database as per requirements
2. **Qt6 Over STL**: Used QVector, QString, QMutex, etc.
3. **Qt Memory Management**: Parent-child automatic memory management
4. **Thread-Safe**: QMutex and QReadWriteLock for critical operations
5. **Signals/Slots**: Qt's event system for loose coupling
6. **MOC Integration**: Proper CMake configuration for Meta-Object Compiler
7. **Thread-Safe Data Access**: Separate `BookingData` struct for cross-thread access without violating Qt's QObject threading rules



## 🚧 Known Limitations

- **Data Persistence**: Data is stored in memory only; restarting the application clears all bookings
- **Fixed Capacity**: All theaters have exactly 20 seats (A1-A20)
- **No Authentication**: No user login or password protection
- **No Payment**: Booking system only; no payment processing
- **CLI Only**: Command-line interface; no graphical UI
- **Single Instance**: Not designed for distributed deployment
- **English Only**: Interface messages in English only



## 🤔 Reflections

### Most Interesting Aspect

Implementing thread-safety using Qt's threading primitives (QMutex, QReadWriteLock) combined with Qt's signal/slot mechanism for event-driven architecture. The challenge of preventing overbooking in a highly concurrent environment using only in-memory storage was intellectually stimulating and showcased Qt's powerful threading capabilities.

The design decision to separate `BookingData` (plain struct) from `Booking` (QObject) to handle cross-thread access elegantly demonstrates deep understanding of Qt's threading model and the constraints of QObject parent-child relationships.



### Most Challenging Aspect

Balancing the requirement to use Qt6 classes instead of STL while maintaining clean, modern C++20 code. Managing memory with Qt's parent-child system (avoiding `std::unique_ptr` in Qt containers), ensuring proper MOC (Meta-Object Compiler) integration, and handling QObject creation across threads required careful consideration.



## 📝 License

MIT License