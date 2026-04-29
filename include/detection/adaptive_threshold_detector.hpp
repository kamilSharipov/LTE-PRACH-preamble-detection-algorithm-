#pragma once

#include "detector.hpp"

namespace prach {

class AdaptiveThresholdDetector : public AbstractDetector {
public:
    explicit AdaptiveThresholdDetector(double coefficient = 0.1) 
        : coefficient_(coefficient) {}

    DetectionResult detect(
        const std::vector<Complex>& correlation,
        const double fs,
        double external_noise_amplitude = -1.0
    ) override;

    double compute_median(std::vector<double> values);
    double get_coefficient() const override;

private:
    double coefficient_;
};

} // namespace prach
