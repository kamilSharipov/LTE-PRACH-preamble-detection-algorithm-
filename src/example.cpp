#include "fftw3.h"

#include <iostream>
#include "zc_generator.hpp"
#include "cprefix.hpp"

using namespace prach;

int main() {
    std::vector<Complex> res = generate_zc_sequence(3, 7);
    std::vector<Complex> out(7);

    fftw_plan plan_forward = fftw_plan_dft_1d(res.size(), res.data(), out.data(),
                                              FFTW_FORWARD, FFTW_ESTIMATE);

    fftw_execute(plan_forward);

    auto res2 = add_cyclic_prefix(res, 3);

    for (auto el: res2) {
        std::cout << el[0] << " " << el[1] << 'j' << "\n";
    }

    return 0;
}