#include "fftw3.h"

#include <iostream>
#include <chrono>

#include "zc_generator.hpp"
#include "cprefix.hpp"
#include "zc_fft.property.hpp"

using namespace prach;

int main() {
    auto start = std::chrono::high_resolution_clock::now();
    std::vector<Complex> res = dft_via_zc_property(839, 3);
    auto end = std::chrono::high_resolution_clock::now();

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    double avg_time_ms = static_cast<double>(duration.count()) / 1000.0;

    std::cout << avg_time_ms << "\n";

    //for (auto el: res) {
        //std::cout << el << "\n";
    //}

    return 0;
}
