#include "fftw3.h"

#include <iostream>

#include "zc_generator.hpp"
#include "cprefix.hpp"
#include "zc_fft.property.hpp"

using namespace prach;

int main() {
    std::vector<Complex> res = dft_via_zc_property(11, 3);

    for (auto el: res) {
        std::cout << el << "\n";
    }

    return 0;
}
