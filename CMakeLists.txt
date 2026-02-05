cmake_minimum_required(VERSION 3.14)
project(PRACH)

set(CMAKE_CXX_STANDARD 17)

add_executable(example
    src/example.cpp
    lib/cprefix/cprefix.cpp
    lib/fft/fft.cpp
    lib/fft/ifft.cpp
    lib/math/zc_generator.cpp
)

target_include_directories(example PRIVATE
    include
    lib/cprefix
    lib/fft
    lib/math
)

find_package(FFTW REQUIRED)
target_link_libraries(example PRIVATE FFTW::fftw3)
