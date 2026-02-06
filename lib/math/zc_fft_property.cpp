#include "zc_fft.property.hpp"

#include "zc_generator.hpp"

#include <cmath>

namespace prach {

int modular_inverse(int a, int mod) {
    int t = 0, newt = 1;
    int r = mod, newr = a;

    while (newr != 0) {
        int q = r / newr;

        int tmp = newt;
        newt = t - q * newt;
        t = tmp;

        tmp = newr;
        newr = r - q * newr;
        r = tmp;
    }

    if (r > 1) {
        throw std::invalid_argument("lib/math/zc_fft_property.cpp: a is not invertible");
    }

    if (t < 0) {
        t += mod;
    }

    return t;
}

std::vector<Complex> dft_via_zc_property(size_t Nzc, int u) {
    if (u <= 0 || u >= Nzc) {
        throw std::invalid_argument("u must be in [1, Nzc-1]");
    }

    const double phase = -PI * (Nzc - 1) / 2.0;
    const Complex weight = Complex(cos(phase), sin(phase));

    int new_u = ((-modular_inverse(u, Nzc)) % Nzc + Nzc) % Nzc;

    std::vector<Complex> result = generate_zc_sequence(new_u, Nzc);

    for (auto& element: result) {
        element *= weight;
    }

    return result;
}

} // namespace prach
