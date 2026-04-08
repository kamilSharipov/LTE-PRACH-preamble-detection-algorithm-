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

        // if (cfg.fs != 1.92e6) {
        //     throw std::invalid_argument("dsfdsfsdf");
        // }

        std::vector<Complex> signal = hybrid_method::generate(cfg.N_zc, cfg.root_index, cfg.N_dft);

        auto vec =  insert_cyclic_prefix(signal, cfg.N_cp);

        //double energy = 0.0;
        //for (auto i : vec) {
        //    energy += std::norm(i);
        //}
        //std::cout << energy / (1024 + 132) << "\n";

        return vec;
    }

    std::string name() const override { return "LTE_PRACH_Format0"; }
};

} // namespace prach
