#pragma once

#include "base.hpp"

namespace prach {

class LteFormat0 : public PreambleFormat {
    std::vector<Complex> generate(const PreambleConfig& cfg) const override {
        ;
    }

    std::string name() const override { return "LTE_PRACH_Format0"; }
};

} // namespace prach
