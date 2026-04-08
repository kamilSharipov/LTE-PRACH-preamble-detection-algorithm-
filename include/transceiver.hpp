#pragma once

#include "channel.hpp"
#include "base.hpp"
#include "detector.hpp"
#include "detector_factory.hpp"

namespace prach {

struct TransceiverConfig {
    PreambleConfig preamble_cfg;
    ChannelConfig  channel_cfg;
};

class Transceiver {
public:
    explicit Transceiver(TransceiverConfig cfg, DetectorType detector_type = DetectorType::CONSTANT_THRESHOLD, double param = 0.7);

    std::vector<Complex> transmit();
    DetectionResult receive(const std::vector<Complex>& rx_signal);

    TransceiverConfig get_config() const;

private:
    TransceiverConfig                 cfg_;
    std::vector<Complex>              reference_;
    std::unique_ptr<AbstractDetector> detector_;
};

} // namespace prach
