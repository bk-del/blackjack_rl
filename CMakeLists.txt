cmake_minimum_required(VERSION 3.20)

project(blackjack_rl LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(blackjack_lib
    src/hand.cpp
    src/blackjack_env.cpp
    src/q_agent.cpp
)

target_include_directories(blackjack_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

if(MSVC)
    target_compile_options(blackjack_lib PRIVATE /W4)
else()
    target_compile_options(blackjack_lib PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(blackjack_app
    src/main.cpp
)
target_link_libraries(blackjack_app PRIVATE blackjack_lib)

if(MSVC)
    target_compile_options(blackjack_app PRIVATE /W4)
else()
    target_compile_options(blackjack_app PRIVATE -Wall -Wextra -Wpedantic)
endif()

enable_testing()

find_package(GTest CONFIG REQUIRED)

add_executable(blackjack_tests
    tests/test_hand.cpp
    tests/test_blackjack_env.cpp
    tests/test_q_agent.cpp
)

target_link_libraries(blackjack_tests PRIVATE
    blackjack_lib
    GTest::gtest_main
)

if(MSVC)
    target_compile_options(blackjack_tests PRIVATE /W4)
else()
    target_compile_options(blackjack_tests PRIVATE -Wall -Wextra -Wpedantic)
endif()

include(GoogleTest)
gtest_discover_tests(blackjack_tests)