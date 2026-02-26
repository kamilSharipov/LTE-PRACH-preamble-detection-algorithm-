#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

std::vector<Complex> ifft_fftw_padded(const std::vector<Complex>& X, size_t N);
std::vector<Complex> ifft_fftw(const std::vector<Complex>& X, size_t N);

} // namespace prach
