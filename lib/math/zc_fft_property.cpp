#include "zc_fft.property.hpp"

#include "zc_generator.hpp"

#include <cmath>

namespace prach {

int modular_inverse(int a, int mod) {
    int t = 0, newt = 1;
    int r = mod, newr = a;

    while (newr != 0) {
        int q = r / newr;

        std::swap(t, newt);
        newt -= q * t;

        std::swap(r, newr);
        newr -= q * r;
    }

    if (r > 1) {
        throw std::invalid_argument("lib/math/zc_fft_property.cpp: a is not invertible");
    }

    if (t < 0) {
        t += mod;
    }

    return t;
}

Complex zc_dft_zero_freq(size_t Nzc, int u) {
    Complex sum = {0, 0};

    for (size_t n = 0; n < Nzc; ++n) {
        double phase = -PI * u * n * (n + 1) / static_cast<double>(Nzc);
        auto element = Complex(cos(phase), sin(phase));
        sum += element;
    }

    return sum;
}

std::vector<Complex> dft_via_zc_property(size_t Nzc, int u) {
    if (u <= 0 || (size_t)u >= Nzc) {
        throw std::invalid_argument("u must be in [1, Nzc-1]");
    }

    int new_u = modular_inverse(u, Nzc);
    std::vector<Complex> result = generate_zc_sequence(new_u, Nzc);

    Complex dft_zero_freq = zc_dft_zero_freq(Nzc, u);

    for (size_t i = 0; i < Nzc; ++i) {
        double phase = 2 * modular_inverse(2, Nzc) * PI * (1 - static_cast<double>(new_u)) * static_cast<double>(i) / static_cast<double>(Nzc);
        result[i] = std::conj(result[i]) * Complex(cos(phase), sin(phase)) * dft_zero_freq;
    }

    return result;
}

} // namespace prach
