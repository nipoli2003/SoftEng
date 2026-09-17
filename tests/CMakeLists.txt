include(FetchContent)
# Download and unpack googletest at configure time
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
# Prevent overriding parent project compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(unit_tests
    test_deck.cpp
    test_briscola_rules.cpp
)

target_link_libraries(unit_tests PRIVATE
    card_model
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(unit_tests)