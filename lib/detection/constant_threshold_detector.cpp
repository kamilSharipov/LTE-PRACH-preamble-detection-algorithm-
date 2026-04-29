#include "constant_threshold_detector.hpp"

#include <cmath>

namespace prach {

DetectionResult ConstantThresholdDetector::detect(
    const std::vector<Complex>& correlation,
    const double fs,
    [[maybe_unused]]double external_noise_amplitude)
{
    DetectionResult res;

    for (size_t i = 0; i < correlation.size(); ++i) {
        double val = std::abs(correlation[i]);
        if (val > res.peak_value) {
            res.peak_value = val;
            res.peak_index = i;
        }
    }

    res.detected = (res.peak_value > threshold_ * static_cast<double>(correlation.size()));
    res.estimated_delay_sec = (res.peak_index) / fs;

    return res;
}

double ConstantThresholdDetector::get_coefficient() const { return threshold_; }

} // namespace prach
