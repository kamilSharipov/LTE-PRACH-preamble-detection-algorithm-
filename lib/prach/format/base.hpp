#pragma once

#include <vector>
#include <string>

#include "prach.hpp"

namespace prach {

struct PreambleConfig {
    size_t N_zc;
    size_t root_index;

    double fs;

    size_t N_rb;
};

class PreambleFormat {
public:
    virtual std::vector<Complex> generate(const PreambleConfig& cfg) const = 0;
    virtual std::string name() const = 0;

    virtual ~PreambleFormat() = default;
};

} // namespace prach
 