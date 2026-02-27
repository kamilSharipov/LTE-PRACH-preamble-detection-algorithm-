cmake_minimum_required(VERSION 3.14)
project(PRACH)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -Werror")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O2 -march=native -DNDEBUG")

    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -g -fsanitize=address -fno-omit-frame-pointer")
    set(CMAKE_EXE_LINKER_FLAGS_DEBUG "${CMAKE_EXE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

find_package(PkgConfig REQUIRED)
pkg_check_modules(FFTW REQUIRED fftw3)
include_directories(${FFTW_INCLUDE_DIRS})

add_library(prach_lib
    lib/math/zc_generator.cpp
    lib/math/zc_fft_property.cpp
    lib/cprefix/cprefix.cpp
    lib/fft/fft.cpp
    lib/fft/ifft.cpp
    lib/crosscorr/crosscorr.cpp
)

target_include_directories(prach_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/cprefix
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/crosscorr
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/fft
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/math
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/prach
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/prach/format
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/prach/method
)

add_executable(example
    src/channel.cpp
    src/transceiver.cpp
    example/example.cpp
)

target_link_libraries(example PRIVATE prach_lib ${FFTW_LIBRARIES})

enable_testing()
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.zip
)
FetchContent_MakeAvailable(googletest)

add_executable(tests
    tests/test_cprefix.cpp
    tests/test_zc_fft_property.cpp
    tests/test_crosscorr.cpp
)

target_link_libraries(tests PRIVATE prach_lib GTest::gtest_main ${FFTW_LIBRARIES})

add_test(NAME tests COMMAND tests)
