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

    static void precalibrate_noise(size_t N_dft, size_t N_zc, int root_index, double noise_var);

    static double get_calibrated_noise(size_t N_dft, size_t N_zc, int root_index, double noise_var);

private:
    TransceiverConfig                 cfg_;
    std::vector<Complex>              reference_;
    std::unique_ptr<AbstractDetector> detector_;

    std::vector<Complex> noise_reference_;
    size_t noise_root_index_;
};

} // namespace prach