#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

std::vector<Complex> count_cross_correlation(std::vector<Complex>& x,
                                             std::vector<Complex>& y,
                                             size_t N);

} // namespace prach
