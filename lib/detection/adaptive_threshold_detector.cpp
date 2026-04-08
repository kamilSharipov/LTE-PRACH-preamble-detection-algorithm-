#include "adaptive_threshold_detector.hpp"

#include <numeric>
#include <cmath>
#include <iostream>
#include <algorithm>

namespace prach {

DetectionResult AdaptiveThresholdDetector::detect(
    const std::vector<Complex>& correlation,
    const double fs)
{
    DetectionResult res;

    if (correlation.empty()) {
        return res;
    }

    double sum_amplitude = 0.0;
    for (size_t i = 0; i < correlation.size(); ++i) {
        double val = std::abs(correlation[i]);
        sum_amplitude += val;

        if (val > res.peak_value) {
            res.peak_value = val;
            res.peak_index = i;
        }
    }

    double mean_amplitude     = sum_amplitude / static_cast<double>(correlation.size());
    double adaptive_threshold = coefficient_ * mean_amplitude;

    res.detected            = (res.peak_value > adaptive_threshold);
    res.estimated_delay_sec = static_cast<double>(res.peak_index) / fs;

    return res;
}

} // namespace prach
