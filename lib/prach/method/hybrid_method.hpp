#pragma once

#include "base.hpp"
#include "zc_fft.property.hpp"
#include "ifft.hpp"

#include <iostream>

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

    size_t L               = freq_signal.size();
    size_t left_part_size  = (L + 1) / 2;
    size_t right_part_size = L / 2;

    std::vector<Complex> padded_freq_signal(N_dft, Complex(0, 0));

    std::copy(freq_signal.begin(),
              freq_signal.begin() + left_part_size,
              padded_freq_signal.begin());

    size_t right_start = N_dft - right_part_size;

    std::copy(freq_signal.begin() + left_part_size,
              freq_signal.end(),
              padded_freq_signal.begin() + right_start);

    std::vector<Complex> time_signal = ifft_fftw(padded_freq_signal, N_dft);

    return time_signal;
}

} // namespace prach::hybrid_method
