#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

std::vector<Complex> ifft_fftw(const std::vector<Complex>& X);

} // namespace prach
