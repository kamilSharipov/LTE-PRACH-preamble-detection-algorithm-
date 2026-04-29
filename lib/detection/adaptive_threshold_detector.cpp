#include "adaptive_threshold_detector.hpp"

#include <numeric>
#include <cmath>
#include <iostream>
#include <algorithm>

namespace prach {

DetectionResult AdaptiveThresholdDetector::detect(
    const std::vector<Complex>& correlation,
    const double fs,
    double external_noise_amplitude)
{
    DetectionResult res;

    if (correlation.empty()) {
        return res;
    }

    //std::vector<double> amplitudes;
    //amplitudes.reserve(correlation.size());

    double sum_amplitude = 0.0;
    for (size_t i = 0; i < correlation.size(); ++i) {
        double val = std::abs(correlation[i]);
        sum_amplitude += val;
        //amplitudes.push_back(val);

        if (val > res.peak_value) {
            res.peak_value = val;
            res.peak_index = i;
        }
    }

    [[maybe_unused]]double mean_amplitude;
    
    if (external_noise_amplitude >= 0) {
        mean_amplitude = external_noise_amplitude;
    } else {
        mean_amplitude = sum_amplitude / static_cast<double>(correlation.size());
    }
    //double adaptive_threshold = 26.37 * std::sqrt(839.0 / correlation.size());
    double adaptive_threshold = coefficient_ * mean_amplitude;

    res.detected            = (res.peak_value > adaptive_threshold);
    res.estimated_delay_sec = static_cast<double>(res.peak_index) / fs;

    return res;
}

double AdaptiveThresholdDetector::compute_median(std::vector<double> values) {
    if (values.empty()) {
        return 0.0;
    }

    const size_t n = values.size();
    const size_t mid = n / 2;

    std::nth_element(values.begin(), values.begin() + mid, values.end());

    if (n % 2 != 0) {
        return values[mid];
    } else {
        auto left_max = std::max_element(values.begin(), values.begin() + mid);

        return (*left_max + values[mid]) / 2.0;
    }
}

double AdaptiveThresholdDetector::get_coefficient() const { return coefficient_; }

} // namespace prach
