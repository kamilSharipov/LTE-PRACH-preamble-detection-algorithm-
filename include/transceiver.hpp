#pragma once

#include "channel.hpp"
#include "base.hpp"

namespace prach {

struct TransceiverConfig {
    PreambleConfig preamble_cfg;
    ChannelConfig channel_cfg;
};

class Transceiver {
public:
    explicit Transceiver(TransceiverConfig cfg);

    std::vector<Complex> transmit();

    struct DetectionResult {
        bool detected = false;
        size_t peak_index = 0;
        double peak_value = 0.0;
        double estimated_delay_sec = 0.0;
    };

    DetectionResult receive(const std::vector<Complex>& rx_signal);

private:
    TransceiverConfig cfg_;
    std::vector<Complex> reference_;
};

} // namespace prach
