#include "transceiver.hpp"
#include "zc_fft.property.hpp"
#include "generator.hpp"
#include "crosscorr.hpp"
#include "fft.hpp"

#include <fstream>

namespace prach {

Transceiver::Transceiver(TransceiverConfig cfg) : cfg_(cfg) {
    reference_ = dft_via_zc_property(cfg_.preamble_cfg.N_zc, cfg_.preamble_cfg.root_index);
}

std::vector<Complex> Transceiver::transmit() {
    auto format = create_format(FormatType::LTE_FORMAT_0);
    return format->generate(cfg_.preamble_cfg);
}

Transceiver::DetectionResult
Transceiver::receive(const std::vector<Complex>& rx_signal) {
    DetectionResult res;
    const size_t N_dft = cfg_.preamble_cfg.N_dft;
    const size_t N_cp = cfg_.preamble_cfg.N_cp;
    const size_t N_gt = cfg_.preamble_cfg.N_gt;
    const double fs = cfg_.preamble_cfg.fs;

    const size_t MAX_SAMPLES = N_cp + N_dft + N_gt;
    std::vector<Complex> limited_rx_signal(
        rx_signal.begin(),
        rx_signal.begin() + std::min(rx_signal.size(), MAX_SAMPLES)
    );

    size_t best_start = 0;
    double max_energy = 0.0;
    for (size_t i = 0; i <= limited_rx_signal.size() - (N_dft + N_cp); ++i) {
        double energy = 0.0;

        for (size_t j = 0; j < (N_dft + N_cp); ++j) {
            energy += std::norm(limited_rx_signal[i + j]);
        }

        if (energy > max_energy) {
            max_energy = energy;
            best_start = i;
        }
    }

    std::vector<Complex> full_window(N_dft + N_cp, Complex(0, 0));
    size_t samples_to_copy = std::min(N_dft + N_cp, limited_rx_signal.size() - best_start);
    std::copy(limited_rx_signal.begin() + best_start,
              limited_rx_signal.begin() + best_start + samples_to_copy,
              full_window.begin());

    std::vector<Complex> rx_signal_window(
        full_window.begin() + N_cp,
        full_window.begin() + N_cp + N_dft
    );

    auto fft_rx_signal_window = fft_fftw(rx_signal_window, N_dft);
    auto cross_corr = count_cross_correlation(fft_rx_signal_window, reference_, N_dft);

    std::ofstream file("corr_phase.txt");

    for (size_t i = 0; i < N_dft; ++i) {
        double val = std::abs(cross_corr[i]) / static_cast<double>(839);
        file << val << "\n";
        if (val > res.peak_value) {
            res.peak_value = val;
            res.peak_index = i;
        }
    }

    file.close();

    res.detected = (res.peak_value > 0.7 * N_dft);
    res.estimated_delay_sec = (best_start + res.peak_index) / fs;

    return res;
}

} // namespace prach
