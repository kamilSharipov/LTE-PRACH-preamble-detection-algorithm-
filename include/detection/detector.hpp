#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

struct DetectionResult {
    bool detected = false;
    size_t peak_index = 0;
    double peak_value = 0.0;
    double estimated_delay_sec = 0.0;
};

class AbstractDetector {
public:
    virtual ~AbstractDetector() = default;
    virtual DetectionResult detect(
        const std::vector<Complex>& correlation,
        const double fs,
        double external_noise_amplitude = -1.0
    ) = 0;
    virtual double get_coefficient() const = 0;
};

} // namespace prach
