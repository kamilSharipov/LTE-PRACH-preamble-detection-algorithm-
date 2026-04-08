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
    lib/detection/adaptive_threshold_detector.cpp
    lib/detection/constant_threshold_detector.cpp
)

target_include_directories(prach_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/include/detection
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/cprefix
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/crosscorr
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/fft
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/math
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/prach
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/detection
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/prach/format
    ${CMAKE_CURRENT_SOURCE_DIR}/lib/prach/method
    ${CMAKE_CURRENT_SOURCE_DIR}/experiments/roc
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

#add_executable(run_true_positive_experiment
#    experiments/true_positive/run_experiments.cpp
#    experiments/true_positive/experiment_runner.cpp
#    src/channel.cpp
#    src/transceiver.cpp
#)
#target_link_libraries(run_true_positive_experiment PRIVATE prach_lib ${FFTW_LIBRARIES})

add_executable(run_roc_experiment 
    experiments/roc/run_roc_experiment.cpp
    experiments/roc/roc_experiment.cpp
    src/channel.cpp
    src/transceiver.cpp
)
target_link_libraries(run_roc_experiment PRIVATE prach_lib ${FFTW_LIBRARIES})

find_program(PYTHON3_EXECUTABLE python3)

add_custom_target(run_roc
    COMMAND run_roc_experiment
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
    COMMENT "Running ROC experiment..."
    DEPENDS run_roc_experiment
)

if(PYTHON3_EXECUTABLE)
    add_custom_target(plot_roc
        COMMAND ${PYTHON3_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/experiments/roc/plot_roc_curves.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Plotting ROC curves..."
    )
else()
    message(WARNING "Python3 not found. 'plot_roc' target will not be available.")

    add_custom_target(plot_roc
        COMMAND ${CMAKE_COMMAND} -E echo "Error: Python3 is required for plotting"
        COMMAND ${CMAKE_COMMAND} -E false
    )
endif()

#add_executable(run_delay_experiment 
#    experiments/delay/run_delay_estimation.cpp
#    experiments/delay/delay_estimation_experiment.cpp
#    src/channel.cpp
#    src/transceiver.cpp
#)
#target_link_libraries(run_delay_experiment PRIVATE prach_lib ${FFTW_LIBRARIES})
