cmake_minimum_required(VERSION 3.20)

project(
    cpp_limit_orderbook
    VERSION 0.2.0
    LANGUAGES CXX
)

option(
    ORDERBOOK_ENABLE_SANITIZERS
    "Enable AddressSanitizer and UndefinedBehaviorSanitizer"
    OFF
)

option(
    ORDERBOOK_ENABLE_CLANG_TIDY
    "Run clang-tidy while compiling project targets"
    OFF
)

option(
    ORDERBOOK_WARNINGS_AS_ERRORS
    "Treat compiler warnings as errors"
    ON
)

option(
    ORDERBOOK_BUILD_BENCHMARK
    "Build the benchmark executable"
    ON
)

option(
    ORDERBOOK_BUILD_DEMO
    "Build the demo executable"
    ON
)

function(orderbook_configure_target target)
    target_compile_features(
        ${target}
        PUBLIC
        cxx_std_17
    )

    if(MSVC)
        target_compile_options(
            ${target}
            PRIVATE
            /W4
        )

        if(ORDERBOOK_WARNINGS_AS_ERRORS)
            target_compile_options(
                ${target}
                PRIVATE
                /WX
            )
        endif()
    else()
        target_compile_options(
            ${target}
            PRIVATE
            -Wall
            -Wextra
            -Wpedantic
            -Wconversion
            -Wshadow
        )

        if(ORDERBOOK_WARNINGS_AS_ERRORS)
            target_compile_options(
                ${target}
                PRIVATE
                -Werror
            )
        endif()
    endif()

    if(
        ORDERBOOK_ENABLE_SANITIZERS
        AND
        CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU"
    )
        target_compile_options(
            ${target}
            PUBLIC
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )

        target_link_options(
            ${target}
            PUBLIC
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
        )
    endif()

    if(ORDERBOOK_ENABLE_CLANG_TIDY)
        find_program(
            CLANG_TIDY_EXE
            NAMES clang-tidy
            REQUIRED
        )

        set_target_properties(
            ${target}
            PROPERTIES
            CXX_CLANG_TIDY
            "${CLANG_TIDY_EXE}"
        )
    endif()
endfunction()

add_library(
    orderbook
    src/order.cpp
    src/orderbook.cpp
)

target_include_directories(
    orderbook
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

orderbook_configure_target(orderbook)

if(ORDERBOOK_BUILD_DEMO)
    add_executable(
        orderbook_demo
        examples/demo.cpp
    )

    target_link_libraries(
        orderbook_demo
        PRIVATE
        orderbook
    )

    orderbook_configure_target(
        orderbook_demo
    )
endif()

if(ORDERBOOK_BUILD_BENCHMARK)
    add_executable(
        orderbook_benchmark
        benchmarks/benchmark.cpp
    )

    target_link_libraries(
        orderbook_benchmark
        PRIVATE
        orderbook
    )

    orderbook_configure_target(
        orderbook_benchmark
    )
endif()

include(CTest)

if(BUILD_TESTING)
    include(FetchContent)

    FetchContent_Declare(
        googletest
        GIT_REPOSITORY
            https://github.com/google/googletest.git
        GIT_TAG
            52eb8108c5bdec04579160ae17225d66034bd723
    )

    set(
        gtest_force_shared_crt
        ON
        CACHE BOOL ""
        FORCE
    )

    FetchContent_MakeAvailable(
        googletest
    )

    add_executable(
        orderbook_tests
        tests/order_tests.cpp
        tests/orderbook_tests.cpp
    )

    target_link_libraries(
        orderbook_tests
        PRIVATE
        orderbook
        GTest::gtest_main
    )

    orderbook_configure_target(
        orderbook_tests
    )

    include(GoogleTest)

    gtest_discover_tests(
        orderbook_tests
    )
endif()