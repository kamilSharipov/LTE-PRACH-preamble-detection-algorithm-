#include "fftw3.h"

#include <iostream>

#include "zc_generator.hpp"
#include "cprefix.hpp"

using namespace prach;

int main() {
    std::vector<Complex> res = generate_zc_sequence(3, 11);

    auto out = add_cyclic_prefix(res, 5);

    for (auto el: out) {
        std::cout << el << "\n";
    }

    return 0;
}
