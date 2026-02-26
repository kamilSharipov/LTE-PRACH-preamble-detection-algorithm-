#pragma once

#include <memory>

#include "prach.hpp"
#include "base.hpp"

#include "lte_format0.hpp"
#include "lte_format1.hpp"
#include "lte_format2.hpp"
#include "lte_format3.hpp"
#include "lte_format4.hpp"

namespace prach {

enum class FormatType {
    LTE_FORMAT_0,
    LTE_FORMAT_1,
    LTE_FORMAT_2,
    LTE_FORMAT_3,
    LTE_FORMAT_4
};

std::unique_ptr<PreambleFormat> create_format(FormatType format) {
    switch (format) {
        case FormatType::LTE_FORMAT_0:
            return std::make_unique<LteFormat0>();
        // case FormatType::LTE_FORMAT_1:
        //     return std::make_unique<LteFormat1>();
        // case FormatType::LTE_FORMAT_2:
        //     return std::make_unique<LteFormat2>();
        // case FormatType::LTE_FORMAT_3:
        //     return std::make_unique<LteFormat3>();
        // case FormatType::LTE_FORMAT_4:
        //     return std::make_unique<LteFormat4>();
        default:
            throw std::invalid_argument("lib/prach/generator.hpp: unsupported PRACH format type");
    }
}

std::vector<Complex> generate_preamble(const FormatType format, 
                                       const PreambleConfig& cfg) {
    auto fmt = create_format(format);
    return fmt->generate(cfg);
}

} // namespace prach
