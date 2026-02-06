#include "zc_generator.hpp"

#include <cmath>
#include <stdexcept>

namespace prach {

std::vector<Complex> generate_zc_sequence(int u, int Nzc) {
    if (u <= 0 || u >= Nzc) {
        throw std::invalid_argument("lib/math/zc_generator.cpp: u must be in [1, Nzc]");
    }

    std::vector<Complex> seq(Nzc);


    for (int n = 0; n < Nzc; ++n) {
        double phase = -PI * u * n * (n + 1) / static_cast<double>(Nzc);
        seq[n] = Complex(cos(phase), sin(phase));
    }

    return seq;
}

std::vector<Complex> apply_cyclic_shift(const std::vector<Complex>& seq, int delay) {
    int N = seq.size();
    delay = ((delay % N) + N) % N;

    std::vector<Complex> shifted(N);

    for (int i = 0; i < N; ++i) {
        shifted[i] = Complex(seq[(i + delay) % N]);
    }

    return shifted;
}

} // namespace prach
