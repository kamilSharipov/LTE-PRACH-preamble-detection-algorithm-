#include "zc_generator.hpp"

#include <iostream>
#include <fstream>
#include <cmath>

using Complex = std::complex<double>;

using namespace prach;

std::vector<Complex> generate_zc_sequence(int u, int Nzc = 839) {
    if (u <= 0 || u >= Nzc) {
        throw std::invalid_argument("u must be in [1, 838]");
    }

    std::vector<Complex> seq(Nzc);
    const double pi = acos(-1.0);

    for (int n = 0; n < Nzc; ++n) {
        double phase = -pi * u * n * (n + 1) / static_cast<double>(Nzc);
        seq[n] = Complex(cos(phase), sin(phase));
    }

    return seq;
}

std::vector<Complex> apply_cyclic_shift(const std::vector<Complex>& seq, int delay) {
    int N = seq.size();
    delay = ((delay % N) + N) % N;

    std::vector<Complex> shifted(N);

    for (int i = 0; i < N; ++i) {
        shifted[i] = seq[(i + delay) % N];
    }

    return shifted;
}
