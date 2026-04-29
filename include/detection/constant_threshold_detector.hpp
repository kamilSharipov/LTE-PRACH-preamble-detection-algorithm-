#pragma once

#include "detector.hpp"

namespace prach {

class ConstantThresholdDetector : public AbstractDetector {
public:
    explicit ConstantThresholdDetector(double threshold = 0.7) 
        : threshold_(threshold) {}

    DetectionResult detect(
        const std::vector<Complex>& correlation,
        const double fs,
        double external_noise_amplitude = -1.0
    ) override;
    double get_coefficient() const override;

private:
    double threshold_;
};

} // namespace prach
