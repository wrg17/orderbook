cmake_minimum_required(VERSION 3.25)

project(orderbook
        VERSION 0.1.0
        DESCRIPTION "Orderbook"
        LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(ORDERBOOK_BUILD_TESTS "Build tests" ON)
option(ORDERBOOK_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)

add_library(orderbook_core
        src/core/order_book.cpp
        src/core/matching_engine.cpp
        src/domain/order.cpp
        src/domain/trade.cpp
)

target_include_directories(orderbook_core
        PUBLIC
        ${PROJECT_SOURCE_DIR}/include
)

target_compile_features(orderbook_core PUBLIC cxx_std_20)

target_compile_options(orderbook_core PRIVATE -Wall -Wextra -Wpedantic)
if (ORDERBOOK_WARNINGS_AS_ERRORS)
    target_compile_options(orderbook_core PRIVATE -Werror)
endif()

add_executable(orderbook_cli
        apps/cli/main.cpp
)

target_link_libraries(orderbook_cli
        PRIVATE orderbook_core
)

if (ORDERBOOK_BUILD_TESTS)
    include(CTest)
    enable_testing()

    find_package(GTest CONFIG REQUIRED)

    add_executable(orderbook_tests
            tests/unit/test_order_book.cpp
            tests/unit/test_matching_engine.cpp
    )

    target_link_libraries(orderbook_tests
            PRIVATE
            orderbook_core
            GTest::gtest_main
    )

    include(GoogleTest)
    gtest_discover_tests(orderbook_tests)
endif()