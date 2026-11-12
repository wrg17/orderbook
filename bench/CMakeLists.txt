file(GLOB_RECURSE BENCH_SRC CONFIGURE_DEPENDS "src/*.cpp")
add_library(orderbook_bench
        ${BENCH_SRC}
)

target_include_directories(orderbook_bench
        PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(orderbook_bench
        PUBLIC
        orderbook_core
)

target_compile_features(orderbook_bench PUBLIC cxx_std_20)

target_compile_options(orderbook_bench PRIVATE -Wall -Wextra -Wpedantic)
if (ORDERBOOK_WARNINGS_AS_ERRORS)
    target_compile_options(orderbook_bench PRIVATE -Werror)
endif ()

add_executable(orderbook_bench_app
        main.cpp
)

target_link_libraries(orderbook_bench_app
        PRIVATE
        orderbook_bench
)

if (ORDERBOOK_BUILD_TESTS)
    add_test(NAME orderbook_bench_smoke COMMAND orderbook_bench_app)

    file(GLOB_RECURSE BENCH_UNIT_TESTS CONFIGURE_DEPENDS "tests/*.unit.test.cpp")
    add_executable(orderbook_bench_tests ${BENCH_UNIT_TESTS})
    target_link_libraries(orderbook_bench_tests
            PRIVATE
            orderbook_bench
            GTest::gtest_main
    )

    gtest_discover_tests(orderbook_bench_tests)
endif ()
