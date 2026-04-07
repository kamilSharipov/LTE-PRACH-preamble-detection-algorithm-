#include "transceiver.hpp"
#include "zc_fft.property.hpp"
#include "generator.hpp"
#include "crosscorr.hpp"
#include "fft.hpp"

namespace prach {

Transceiver::Transceiver(TransceiverConfig cfg, DetectorType detector_type, double param)
    : cfg_(cfg), detector_(DetectorFactory::create(detector_type, param)) 
{
    reference_ = dft_via_zc_property(cfg_.preamble_cfg.N_zc, cfg_.preamble_cfg.root_index);
}

std::vector<Complex> Transceiver::transmit() {
    auto format = create_format(FormatType::LTE_FORMAT_0);
    auto signal = format->generate(cfg_.preamble_cfg);
    
    return signal;
}

DetectionResult
Transceiver::receive(const std::vector<Complex>& rx_signal) {
    const size_t N_dft = cfg_.preamble_cfg.N_dft;
    const size_t N_cp = cfg_.preamble_cfg.N_cp;
    const double fs = cfg_.preamble_cfg.fs;

    size_t needed_length = N_cp + N_dft;
    if (rx_signal.size() < needed_length) {
        DetectionResult res;
        res.detected = false;
        return res;
    }

    auto start = rx_signal.begin() + N_cp;
    auto end = rx_signal.begin() + needed_length;
    
    std::vector<Complex> rx_signal_window(start, end);

    std::vector<Complex> freq_rx_signal_window = fft_fftw(rx_signal_window, N_dft);

    auto cross_corr = count_cross_correlation(freq_rx_signal_window, reference_, N_dft);

    return detector_->detect(cross_corr, fs);
}

TransceiverConfig Transceiver::get_config() const {
    return cfg_;
}

} // namespace prach
