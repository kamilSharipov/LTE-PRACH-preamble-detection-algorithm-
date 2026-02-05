#include "zc_generator.hpp"

#include <cmath>
#include <stdexcept>

namespace prach {

std::vector<Complex> generate_zc_sequence(int u, int Nzc) {
    if (u <= 0 || u >= Nzc) {
        throw std::invalid_argument("u must be in [1, 838]");
    }

    std::vector<Complex> seq(Nzc);
    const double pi = acos(-1.0);

    for (int n = 0; n < Nzc; ++n) {
        double phase = -PI * u * n * (n + 1) / static_cast<double>(Nzc);
        seq[n][0] = cos(phase);
        seq[n][1] = sin(phase);
    }

    return seq;
}

std::vector<Complex> apply_cyclic_shift(const std::vector<Complex>& seq, int delay) {
    int N = seq.size();
    delay = ((delay % N) + N) % N;

    std::vector<Complex> shifted(N);

    for (int i = 0; i < N; ++i) {
        shifted[i][0] = seq[(i + delay) % N][0];
        shifted[i][1] = seq[(i + delay) % N][1];
    }

    return shifted;
}

} // namespace prach
