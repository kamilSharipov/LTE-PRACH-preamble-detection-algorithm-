#pragma once

#include <vector>
#include <string>

#include "prach.hpp"

namespace prach {

struct PreambleConfig {
    size_t N_zc;
    size_t root_index;
};

class PreambleFormat {
public:
    virtual std::vector<Complex> generate(const PreambleConfig& cfg) const = 0;
    virtual std::string name() const = 0;

    virtual ~PreambleFormat() = default;
};

} // namespace prach
 