#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

std::vector<Complex> fft_fftw(const std::vector<Complex>& x);

} // namespace prach
