cmake_minimum_required(VERSION 3.20)
project(TicketBooking VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Qt6 auto-generated files
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_DOCS "Build documentation" ON)

# Conan support
if(EXISTS ${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
    include(${CMAKE_BINARY_DIR}/conan_toolchain.cmake)
endif()

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core)
if(BUILD_TESTS)
    find_package(Qt6 REQUIRED COMPONENTS Test Concurrent)
endif()

# Include directories
include_directories(include)

# Core library sources
set(CORE_SOURCES
    src/models/Movie.cpp
    src/models/Theater.cpp
    src/models/Seat.cpp
    src/models/Booking.cpp
    src/core/BookingService.cpp
)

# Core library headers (for MOC)
set(CORE_HEADERS
    include/models/Movie.h
    include/models/Theater.h
    include/models/Seat.h
    include/models/Booking.h
    include/core/BookingService.h
)

# Core library
add_library(booking_core ${CORE_SOURCES} ${CORE_HEADERS})

target_include_directories(booking_core PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)
target_compile_features(booking_core PUBLIC cxx_std_20)
target_link_libraries(booking_core PUBLIC Qt6::Core)

# CLI sources
set(CLI_SOURCES
    src/cli/main.cpp
    src/cli/CLIInterface.cpp
)

# CLI headers (for MOC)
set(CLI_HEADERS
    include/cli/CLIInterface.h
)

# CLI executable
add_executable(ticket-booking-cli ${CLI_SOURCES} ${CLI_HEADERS})

target_link_libraries(ticket-booking-cli PRIVATE booking_core Qt6::Core)

# Tests
if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
    
    # Add custom message
    message(STATUS "Tests enabled - Individual test executables will be created")
    message(STATUS "  - test-booking-service")
    message(STATUS "  - test-models")
    message(STATUS "  - test-thread-safety")
    message(STATUS "Run with: ctest --verbose or run individual tests")
endif()

# Installation
install(TARGETS ticket-booking-cli booking_core
    RUNTIME DESTINATION bin
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib
)

install(DIRECTORY include/ DESTINATION include)

# Install test binaries (optional)
if(BUILD_TESTS)
    install(TARGETS test-booking-service test-models test-thread-safety
        RUNTIME DESTINATION bin/tests
        OPTIONAL
    )
endif()
