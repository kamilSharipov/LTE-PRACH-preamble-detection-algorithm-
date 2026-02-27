#pragma once

#include "base.hpp"
#include "zc_fft.property.hpp"
#include "ifft.hpp"

namespace prach::direct_method {

inline std::vector<Complex> generate(
    size_t N_zc,
    size_t root_index,
    size_t N_dft
) {
    if (N_zc == 0 || N_dft < N_zc || N_zc % 2 == 0) {
        throw std::invalid_argument("N_zc must be odd and <= N_dft");
    }

    std::vector<Complex> X = dft_via_zc_property(N_zc, static_cast<int>(root_index));
    
    std::vector<Complex> freq_signal(N_dft, Complex(0, 0));

    constexpr size_t left_zeros = 12;
    constexpr size_t right_zeros = 13;

    std::copy(X.begin(), X.begin() + 420, freq_signal.begin() + left_zeros);
    size_t right_start = N_dft - right_zeros - 419;
    std::copy(X.begin() + 420, X.end(), freq_signal.begin() + right_start);

    std::vector<Complex> time_signal = ifft_fftw(freq_signal, 24576);

    return time_signal;
}

} // namespace prach::direct_method
