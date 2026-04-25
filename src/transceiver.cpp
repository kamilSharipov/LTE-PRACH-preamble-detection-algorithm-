#include "transceiver.hpp"
#include "zc_fft.property.hpp"
#include "generator.hpp"
#include "crosscorr.hpp"
#include "fft.hpp"
#include "utils.hpp"

#include <fstream>
#include <map>
#include <sstream>
#include <iomanip>
#include <random>

namespace prach {

static double calibrate_noise_level(size_t N_dft, size_t N_zc, int root_index, double noise_var, size_t trials) {
    int noise_root = (root_index + 100) % N_zc;
    if (noise_root == root_index) noise_root = (noise_root + 1) % N_zc;

    auto ref_839 = dft_via_zc_property(N_zc, noise_root);
    auto reference = (N_dft != N_zc) ? add_zero_padding(ref_839, N_dft) : ref_839;

    std::mt19937 gen(42);
    double sigma = std::sqrt(noise_var / 2.0);
    std::normal_distribution<double> noise_dist(0.0, sigma);

    double total_amp = 0.0;
    size_t total_samples = 0;

    for (size_t t = 0; t < trials; ++t) {
        std::vector<Complex> noise_time(N_dft);
        for (auto& s : noise_time) {
            s = Complex(noise_dist(gen), noise_dist(gen));
        }
        
        auto freq_noise = fft_fftw(noise_time, N_dft);
        auto corr = count_cross_correlation(freq_noise, reference, N_dft);
        
        for (const auto& c : corr) {
            total_amp += std::abs(c);
            ++total_samples;
        }
    }

    return total_amp / static_cast<double>(total_samples);
}

static std::map<std::string, double> g_noise_calibration_cache;

static std::string make_noise_key(size_t N_dft, double noise_var) {
    std::ostringstream oss;
    oss << N_dft << ":" << std::fixed << std::setprecision(2) << noise_var;
    return oss.str();
}

double Transceiver::get_calibrated_noise(size_t N_dft, size_t N_zc, int root_index, double noise_var) {
    std::string key = make_noise_key(N_dft, noise_var);
    
    auto it = g_noise_calibration_cache.find(key);
    if (it != g_noise_calibration_cache.end()) {
        return it->second;
    }

    double calibrated = calibrate_noise_level(N_dft, N_zc, root_index, noise_var, 5000);
    //calibrated = 26.37 / 4.2 * std::sqrt(839.0 / N_dft);
    g_noise_calibration_cache[key] = calibrated;
    
    return calibrated;
}

Transceiver::Transceiver(TransceiverConfig cfg, DetectorType detector_type, double param)
    : cfg_(cfg), detector_(DetectorFactory::create(detector_type, param)) 
{
    auto reference_839 = dft_via_zc_property(cfg_.preamble_cfg.N_zc, cfg_.preamble_cfg.root_index);

    noise_root_index_ = 100;
    if (noise_root_index_ >= cfg_.preamble_cfg.N_zc) {
        noise_root_index_ = 0;
    }

    auto noise_ref_839 = dft_via_zc_property(cfg_.preamble_cfg.N_zc, noise_root_index_);

    if (cfg_.preamble_cfg.N_dft != STANDART_NZC) {
        reference_       = add_zero_padding(reference_839, cfg_.preamble_cfg.N_dft);
        noise_reference_ = add_zero_padding(noise_ref_839, cfg_.preamble_cfg.N_dft);
    } else {
        reference_       = reference_839;
        noise_reference_ = noise_ref_839;
    }
}

std::vector<Complex> Transceiver::transmit() {
    auto format = create_format(FormatType::LTE_FORMAT_0);
    auto signal = format->generate(cfg_.preamble_cfg);

    return signal;
}

DetectionResult Transceiver::receive(const std::vector<Complex>& rx_signal) {
    const size_t N_dft = cfg_.preamble_cfg.N_dft;
    const size_t N_cp = cfg_.preamble_cfg.N_cp;
    const double fs = cfg_.preamble_cfg.fs;

    size_t needed_length = N_cp + N_dft;
    if (rx_signal.size() < needed_length) {
        return DetectionResult{};
    }

    auto start = rx_signal.begin() + N_cp;
    auto end = rx_signal.begin() + needed_length;
    std::vector<Complex> rx_window(start, end);

    std::vector<Complex> freq_rx = fft_fftw(rx_window, N_dft);

    auto corr_main = count_cross_correlation(freq_rx, reference_, N_dft);

    std::ofstream file("corr_phase.txt");
    for (const auto& el: corr_main) {
        file << std::abs(el) << "\n";
    }
    file.close();

    double mean_noise_amplitude = get_calibrated_noise(
        N_dft,
        cfg_.preamble_cfg.N_zc,
        cfg_.preamble_cfg.root_index,
        cfg_.channel_cfg.noise_var
    );
    
    return detector_->detect(corr_main, fs, mean_noise_amplitude);
}

TransceiverConfig Transceiver::get_config() const {
    return cfg_;
}

void Transceiver::precalibrate_noise(size_t N_dft, size_t N_zc, int root_index, double noise_var) {
    get_calibrated_noise(N_dft, N_zc, root_index, noise_var);
}

} // namespace prach
