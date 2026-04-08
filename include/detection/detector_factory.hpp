#pragma once

#include "detector.hpp"
#include "constant_threshold_detector.hpp"
#include "adaptive_threshold_detector.hpp"

#include <memory>

namespace prach {

enum class DetectorType {
    CONSTANT_THRESHOLD,
    ADAPTIVE_THRESHOLD
};

class DetectorFactory {
public:
    static std::unique_ptr<AbstractDetector> create(DetectorType type, double parameter = 0.7) {
        switch (type) {
        case DetectorType::CONSTANT_THRESHOLD:
            return std::make_unique<ConstantThresholdDetector>(parameter);
        case DetectorType::ADAPTIVE_THRESHOLD:
            return std::make_unique<AdaptiveThresholdDetector>(parameter);
        default:
            throw std::invalid_argument("Unknown detector type");
        }
    }
};

} // namespace prach
