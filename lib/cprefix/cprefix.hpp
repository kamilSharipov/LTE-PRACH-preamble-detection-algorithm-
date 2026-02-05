#pragma once

#include <vector>
#include <cstdint>

#include "prach.hpp"

namespace prach {

std::vector<Complex> add_cyclic_prefix(const std::vector<Complex>& signal,
                                       size_t cp_length);

} // namespace prach
