cmake_minimum_required(VERSION 3.14)
project(PRACH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native -DNDEBUG")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
endif()

add_library(prach_lib
    lib/math/zc_generator.cpp
    lib/math/zc_fft_property.cpp
    lib/cprefix/cprefix.cpp
    lib/fft/fft.cpp
    lib/fft/ifft.cpp
)

target_include_directories(prach_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/math
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/cprefix
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/fft
)

add_executable(example
    src/example.cpp
)

target_link_libraries(example PRIVATE prach_lib)

enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    tests/test_cprefix.cpp
)

target_link_libraries(tests PRIVATE prach_lib GTest::gtest_main)

add_test(NAME tests COMMAND tests)

