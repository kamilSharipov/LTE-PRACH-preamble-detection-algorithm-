#include "transceiver.hpp"
#include <iostream>

using namespace prach;

int main() {
    TransceiverConfig cfg;

    cfg.preamble_cfg.N_zc          = 839;
    cfg.preamble_cfg.N_dft         = 1024;
    cfg.preamble_cfg.N_cp          = 132;
    cfg.preamble_cfg.N_gt          = 124;
    cfg.preamble_cfg.root_index    = 25;
    cfg.preamble_cfg.fs            = 1.28e6;

    cfg.channel_cfg.noise_var      = 0.0;
    cfg.channel_cfg.delay_sec      = 50e-6;
    cfg.channel_cfg.freq_offset_hz = 0.0;
    cfg.channel_cfg.fs             = cfg.preamble_cfg.fs;

    Transceiver trx(cfg, DetectorType::ADAPTIVE_THRESHOLD, 4.25);

    auto tx = trx.transmit();

    Channel channel(cfg.channel_cfg);
    auto rx = channel.apply(tx);

    auto res = trx.receive(rx);

    std::cout << "\n=== Adaptive Threshold ===\n";
    std::cout << "Detected: " << (res.detected ? "YES" : "NO") << "\n";
    std::cout << "Peak value: " << res.peak_value << "\n";
    std::cout << "Estimated delay: " << res.estimated_delay_sec * 1000 * 1000 << "\n";

    return 0;
}
