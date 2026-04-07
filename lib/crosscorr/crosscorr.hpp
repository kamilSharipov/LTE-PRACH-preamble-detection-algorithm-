#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

std::vector<Complex> count_cross_correlation(const std::vector<Complex>& x,
                                             const std::vector<Complex>& y,
                                             const size_t N);

} // namespace prach
