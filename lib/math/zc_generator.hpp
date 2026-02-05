#pragma once

#include <vector>

#include <fftw3.h>

#include "prach.hpp"

namespace prach {

std::vector<Complex> generate_zc_sequence(int u, int Nzc = STANDART_NZC);
std::vector<Complex> apply_cyclic_shift(const std::vector<Complex>& seq, int delay);

} // namespace prach
