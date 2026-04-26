#include "roc_experiment.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <random>
#include <iomanip>
#include <filesystem>
#include <cmath>

namespace prach {

static std::vector<Complex> decimate(const std::vector<Complex>& signal, int factor) {
    if (factor <= 1) {
        return signal;
    }

    std::vector<Complex> out;
    out.reserve(signal.size() / factor + 1);

    for (size_t i = 0; i < signal.size(); i += factor) {
        out.push_back(signal[i]);
    }

    return out;
}

Transceiver RocExperiment::create_transceiver(double noise_var, double threshold_param) {
    TransceiverConfig cfg;

    if (cfg_.preamble_length == 839) {
        cfg.preamble_cfg.N_zc       = 839;
        cfg.preamble_cfg.N_dft      = 839;
        cfg.preamble_cfg.N_cp       = 108;
        cfg.preamble_cfg.N_gt       = 101;
        cfg.preamble_cfg.root_index = 25;
        cfg.preamble_cfg.fs         = 1.04875e6;
    }
    else if (cfg_.preamble_length == 1024) {
        cfg.preamble_cfg.N_zc       = 839;
        cfg.preamble_cfg.N_dft      = 1024;
        cfg.preamble_cfg.N_cp       = 132;
        cfg.preamble_cfg.N_gt       = 124;
        cfg.preamble_cfg.root_index = 25;
        cfg.preamble_cfg.fs         = 1.28e6;
    }
    else if (cfg_.preamble_length == 2048) {
        cfg.preamble_cfg.N_zc       = 839;
        cfg.preamble_cfg.N_dft      = 2048;
        cfg.preamble_cfg.N_cp       = 264;
        cfg.preamble_cfg.N_gt       = 248;
        cfg.preamble_cfg.root_index = 25;
        cfg.preamble_cfg.fs         = 2.56e6;
    }
    else {
        throw std::runtime_error("Unsupported preamble_length: " + std::to_string(cfg_.preamble_length));
    }

    cfg.channel_cfg.noise_var      = noise_var;
    cfg.channel_cfg.delay_sec      = 0.0e-6;
    cfg.channel_cfg.freq_offset_hz = 0.0e-6;
    cfg.channel_cfg.fs             = cfg.preamble_cfg.fs;

    return Transceiver(cfg, cfg_.detector_type, threshold_param);
}

std::pair<double, double> RocExperiment::compute_pfa_pd(double noise_var, double threshold_param) {
    size_t fp_count = 0;
    size_t tp_count = 0;

    const int N_dft_tx = 8192;
    const int N_dft_rx = cfg_.preamble_length;
    const int M = N_dft_tx / N_dft_rx;

    auto trx_rx = create_transceiver(noise_var, threshold_param);
    auto rx_cfg = trx_rx.get_config();

    std::random_device rd_noise, rd_delay;
    std::mt19937 gen_noise(rd_noise());
    std::mt19937 gen_delay(rd_delay());

    double sigma_rx = std::sqrt(noise_var / 2.0);
    std::normal_distribution<double> noise_dist_rx(0.0, sigma_rx);
    std::uniform_real_distribution<double> delay_dist_us(0.0, cfg_.max_delay_us);

    const size_t rx_buf_len = rx_cfg.preamble_cfg.N_cp + rx_cfg.preamble_cfg.N_dft;
    const double fs_tx = rx_cfg.preamble_cfg.fs * M;

    // TX сигнал на высокой частоте
    auto tx_cfg = rx_cfg;
    tx_cfg.preamble_cfg.N_dft = N_dft_tx;
    tx_cfg.preamble_cfg.N_cp = rx_cfg.preamble_cfg.N_cp * M;
    tx_cfg.preamble_cfg.N_gt = rx_cfg.preamble_cfg.N_gt * M;
    tx_cfg.preamble_cfg.fs = fs_tx;

    auto trx_tx = Transceiver(tx_cfg, cfg_.detector_type, threshold_param);
    auto tx_signal = trx_tx.transmit();
    for (auto& el: tx_signal) {
        el *= std::sqrt(8192.0 / N_dft_rx);
    }

    // Калибровка шума для порога (для длины приёмника)
    double mean_noise_amp = Transceiver::get_calibrated_noise(
        N_dft_tx, 839, 25, noise_var
    );
    mean_noise_amp *= std::sqrt(M);

    // === False Positive: шум на частоте приёмника ===
    for (size_t i = 0; i < cfg_.num_trials; ++i) {
        std::vector<Complex> noise(rx_buf_len);
        for (auto& s : noise) {
            s = Complex(noise_dist_rx(gen_noise), noise_dist_rx(gen_noise));
        }
        auto res = trx_rx.receive(noise);
        if (res.detected) fp_count++;
    }

    // === True Positive: сигнал + Channel(задержка) + децимация + шум ===
    for (size_t i = 0; i < cfg_.num_trials; ++i) {
        // 1. Задержка через Channel на высокой частоте (без шума)
        ChannelConfig ch_cfg;
        ch_cfg.fs = fs_tx;
        ch_cfg.noise_var = noise_var;  // шум добавим позже на частоте приёмника
        ch_cfg.freq_offset_hz = 0.0;
        
        if (cfg_.max_delay_us > 0.0) {
            double delay_us = delay_dist_us(gen_delay);
            ch_cfg.delay_sec = delay_us * 1e-6;
        } else {
            ch_cfg.delay_sec = 0.0;
        }
        
        Channel channel_tx(ch_cfg);
        auto delayed_tx = channel_tx.apply(tx_signal);

        // 2. Децимация до частоты приёмника
        auto rx_signal = decimate(delayed_tx, M);

        // 3. Приводим к ожидаемой длине буфера
        if (rx_signal.size() < rx_buf_len) {
            rx_signal.resize(rx_buf_len, Complex(0, 0));
        } else if (rx_signal.size() > rx_buf_len) {
            rx_signal.resize(rx_buf_len);
        }

        // 4. Добавляем шум на частоте приёмника
        //for (auto& s : rx_signal) {
        //    s += Complex(noise_dist_rx(gen_noise), noise_dist_rx(gen_noise));
        //}

        // 5. Детектирование
        auto res = trx_rx.receive(rx_signal);
        if (res.detected) tp_count++;

        if (i == 0) {
            std::cerr << "[DEBUG] N_dft_rx=" << N_dft_rx 
                      << " | M=" << M
                      << " | peak_value=" << res.peak_value
                      << " | mean_noise_amp=" << mean_noise_amp
                      << " | threshold=" << mean_noise_amp * threshold_param
                      << "\n";
        }
    }

    double pfa = static_cast<double>(fp_count) / cfg_.num_trials;
    double pd = static_cast<double>(tp_count) / cfg_.num_trials;
    return {pfa, pd};
}

void RocExperiment::run() {
    std::filesystem::path data_dir = "../experiments/roc/data";
    std::filesystem::create_directories(data_dir);

    for (double noise_var : cfg_.noise_var_values) {
        auto trx_for_noise = create_transceiver(noise_var, 1.0);

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

    std::cout << "ROC completed\n";
}

} // namespace prach
