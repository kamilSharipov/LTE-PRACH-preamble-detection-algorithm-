#pragma once

#include "base.hpp"
#include "zc_fft.property.hpp"
#include "ifft.hpp"

namespace prach::hybrid_method {

inline std::vector<Complex> generate(
    size_t N_zc,
    size_t root_index,
    size_t N_dft
) {
    if (N_zc == 0 || N_dft < N_zc || N_zc % 2 == 0) {
        throw std::invalid_argument("N_zc must be odd and <= N_dft");
    }

    std::vector<Complex> freq_signal = dft_via_zc_property(N_zc, static_cast<int>(root_index));
    //std::vector<Complex> X(864, Complex(0, 0));
    //constexpr size_t left_zeros = 12;
    //std::copy(freq_signal.begin(), freq_signal.end(), X.begin() + left_zeros);

    std::vector<Complex> time_signal = ifft_fftw_padded(freq_signal, N_dft);

    return time_signal;
}

} // namespace prach::hybrid_method
