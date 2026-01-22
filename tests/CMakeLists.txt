# Create separate test executables for each test suite

# Test: Booking Service
add_executable(test-booking-service
    test_booking_service.cpp
)

target_link_libraries(test-booking-service
    PRIVATE
        booking_core
        Qt6::Core
        Qt6::Test
)

target_include_directories(test-booking-service PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(NAME BookingServiceTests COMMAND test-booking-service)

# Test: Models
add_executable(test-models
    test_models.cpp
)

target_link_libraries(test-models
    PRIVATE
        booking_core
        Qt6::Core
        Qt6::Test
)

target_include_directories(test-models PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(NAME ModelsTests COMMAND test-models)

# Test: Thread Safety
add_executable(test-thread-safety
    test_thread_safety.cpp
)

target_link_libraries(test-thread-safety
    PRIVATE
        booking_core
        Qt6::Core
        Qt6::Test
        Qt6::Concurrent
)

target_include_directories(test-thread-safety PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_CURRENT_BINARY_DIR}
)

add_test(NAME ThreadSafetyTests COMMAND test-thread-safety)

# Optional: Create a convenience target to run all tests
add_custom_target(run-all-tests
    COMMAND ${CMAKE_CTEST_COMMAND} --verbose
    DEPENDS test-booking-service test-models test-thread-safety
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
)
