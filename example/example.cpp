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

    cfg.channel_cfg.snr_db         = -10.0;
    cfg.channel_cfg.delay_sec      = 10e-6; // 10 мкс
    cfg.channel_cfg.freq_offset_hz = 0.0;
    cfg.channel_cfg.fs             = 1.28e6;

    Transceiver trx(cfg);

    auto tx = trx.transmit();

    Channel channel(cfg.channel_cfg);
    auto rx = channel.apply(tx);

    auto result = trx.receive(rx);

    std::cout << "Detected: " << (result.detected ? "YES" : "NO") << "\n";
    std::cout << "Peak value: " << result.peak_value << "\n";
    std::cout << "Estimated delay: " << result.estimated_delay_sec * 1e6 << " mks\n";

    return 0;
}
