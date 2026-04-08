#pragma once

#include "detector.hpp"

namespace prach {

class AdaptiveThresholdDetector : public AbstractDetector {
public:
    explicit AdaptiveThresholdDetector(double coefficient = 0.1) 
        : coefficient_(coefficient) {}

    DetectionResult detect(
        const std::vector<Complex>& correlation,
        const double fs
    ) override;

private:
    double coefficient_;
};

} // namespace prach
