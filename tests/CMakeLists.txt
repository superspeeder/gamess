cmake_minimum_required(VERSION 3.30)

option(GAME_TESTS "Build tests for game" ON)

if (GAME_TESTS)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/52eb8108c5bdec04579160ae17225d66034bd723.zip)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
    FetchContent_MakeAvailable(googletest)

    enable_testing()
    add_executable(game_test src/game/tests/test_main.cpp)
    target_link_libraries(game_test PRIVATE GTest::gtest_main game_internal)

    include(GoogleTest)
    gtest_discover_tests(game_test)
endif()

