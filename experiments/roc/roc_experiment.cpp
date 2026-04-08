#include "roc_experiment.hpp"

#include <fstream>
#include <iostream>
#include <random>
#include <iomanip>
#include <filesystem>

namespace prach {

Transceiver RocExperiment::create_transceiver(double noise_var, double threshold_param) {
    TransceiverConfig cfg;

    if (cfg_.preamble_length == 839) {
        cfg.preamble_cfg.N_zc = 839;
        cfg.preamble_cfg.N_dft = 839;
        cfg.preamble_cfg.N_cp = 108;
        cfg.preamble_cfg.N_gt = 101;
        cfg.preamble_cfg.root_index = 25;
        cfg.preamble_cfg.fs = 1.04875e6;
    }
    else if (cfg_.preamble_length == 1024) {
        cfg.preamble_cfg.N_zc = 839;
        cfg.preamble_cfg.N_dft = 1024;
        cfg.preamble_cfg.N_cp = 132;
        cfg.preamble_cfg.N_gt = 124;
        cfg.preamble_cfg.root_index = 25;
        cfg.preamble_cfg.fs = 1.28e6;
    }
    else if (cfg_.preamble_length == 2048) {
        cfg.preamble_cfg.N_zc = 839;
        cfg.preamble_cfg.N_dft = 2048;
        cfg.preamble_cfg.N_cp = 264;
        cfg.preamble_cfg.N_gt = 248;
        cfg.preamble_cfg.root_index = 25;
        cfg.preamble_cfg.fs = 2.56e6;
    }
    else {
        throw std::runtime_error("Unsupported preamble_length: " + std::to_string(cfg_.preamble_length));
    }

    cfg.channel_cfg.noise_var = noise_var;
    cfg.channel_cfg.delay_sec = 0.0;
    cfg.channel_cfg.freq_offset_hz = 0.0e-6;
    cfg.channel_cfg.fs = cfg.preamble_cfg.fs;

    return Transceiver(cfg, cfg_.detector_type, threshold_param);
}

std::pair<double, double> RocExperiment::compute_pfa_pd(double noise_var, double threshold_param) {
    size_t fp_count = 0;
    size_t tp_count = 0;

    auto trx = create_transceiver(noise_var, threshold_param);
    auto tx_signal = trx.transmit();

    std::random_device rd;
    std::mt19937 gen(rd());
    double sigma = std::sqrt(noise_var);
    std::normal_distribution<double> noise_dist(0.0, sigma);

    size_t signal_len = tx_signal.size();

    // False Positive trials (только шум)
    for (size_t i = 0; i < cfg_.num_trials; ++i) {
        std::vector<Complex> rx(signal_len);
        for (auto& sample : rx) {
            sample = Complex(noise_dist(gen), noise_dist(gen));
        }

        auto result = trx.receive(rx);
        if (result.detected) {
            fp_count++;
        }
    }

    // True Positive trials (сигнал + шум)
    for (size_t i = 0; i < cfg_.num_trials; ++i) {
        Channel channel(trx.get_config().channel_cfg);
        
        auto rx = channel.apply(tx_signal);
        
        auto result = trx.receive(rx);
        if (result.detected) {
            tp_count++;
        }
    }

    double pfa = static_cast<double>(fp_count) / cfg_.num_trials;
    double pd  = static_cast<double>(tp_count) / cfg_.num_trials;

    return {pfa, pd};
}

void RocExperiment::run() {
    std::filesystem::path data_dir = "../experiments/roc/data";
    std::filesystem::create_directories(data_dir);

    for (double noise_var : cfg_.noise_var_values) {
        std::vector<RocPoint> roc_points;
        
        std::cout << "Processing noise variance = " << noise_var << "\n";
        
        for (double param : cfg_.threshold_params) {
            auto [pfa, pd] = compute_pfa_pd(noise_var, param);
            roc_points.push_back({pfa, pd, param});
            
            std::cout << "  Param=" << param << ", PFA=" << pfa << ", PD=" << pd << "\n";
        }

        std::ostringstream filename;
        filename << cfg_.output_file_prefix << "_noisevar_" 
                 << std::fixed << std::setprecision(0) << noise_var << ".csv";

        std::filesystem::path file_path = data_dir / filename.str();
        std::ofstream file(file_path);

        if (!file.is_open()) {
            throw std::runtime_error("Failed to create file: " + file_path.string());
        }

        file << "pfa,pd,threshold_param\n";

        for (const auto& point : roc_points) {
            file << point.pfa << "," << point.pd << "," << point.threshold_param << "\n";
        }

        file.close();
    }

    std::cout << "ROC experiments completed!\n";
}

} // namespace prach
