#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

std::vector<Complex> dft_via_zc_property(size_t Nzc, int u);

} // namespace prach
