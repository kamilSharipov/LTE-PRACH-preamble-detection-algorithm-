#pragma once

#include "base.hpp"
#include "direct_method.hpp"
#include "hybrid_method.hpp"
#include "cprefix.hpp"

#include <iostream>

namespace prach {

class LteFormat0 : public PreambleFormat {
public:
    std::vector<Complex> generate(const PreambleConfig& cfg) const override {
        if (cfg.N_zc != 839) {
            throw std::invalid_argument("dsfsdf");
        }

        std::vector<Complex> signal = hybrid_method::generate(cfg.N_zc, cfg.root_index, cfg.N_dft);
        
        auto vec =  insert_cyclic_prefix(signal, cfg.N_cp);

        return vec;
    }

    std::string name() const override { return "LTE_PRACH_Format0"; }
};

} // namespace prach
