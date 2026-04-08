#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

inline std::vector<Complex> add_zero_padding(const std::vector<Complex>& signal, 
                                             const size_t N) {
    if (N == 0) {
        throw std::invalid_argument("Padding size N must be greater than 0");
    }

    if (signal.empty()) {
        return std::vector<Complex>(N, Complex{0, 0});
    }

    size_t L = signal.size();

    if (L > N) {
        L = N;
    }

    size_t left_part_size = (L + 1) / 2;
    size_t right_part_size = L / 2;

    std::vector<Complex> padded(N, Complex{0, 0});

    std::copy(signal.begin(),
              signal.begin() + left_part_size,
              padded.begin());

    size_t right_start = N - right_part_size;
    std::copy(signal.begin() + left_part_size,
              signal.begin() + L,
              padded.begin() + right_start);
    
    return padded;
}

} // namespace prach