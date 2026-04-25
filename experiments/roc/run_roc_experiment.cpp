#include "roc_experiment.hpp"

#include <iostream>
#include <vector>

int main() {
    std::vector<int> preamble_lengths = {839, 1024, 2048};
    
    prach::RocExperiment::Config cfg;

    cfg.num_trials       = 10000;
    cfg.noise_var_values = {48.0};
    cfg.threshold_params = {
        1.0, 1.25, 1.5, 1.75,
        2.0, 2.25, 2.5, 2.75,
        3.0, 3.25, 3.5, 3.75,
        4.0, 4.25, 4.5, 4.75,
        5.0, 5.25, 5.5, 5.75, 
        6.0
    };
    cfg.detector_type = prach::DetectorType::ADAPTIVE_THRESHOLD;

    for (int plen : preamble_lengths) {
        std::cout << "\nN_dft: " << plen << "\n";

        cfg.preamble_length    = plen;
        cfg.output_file_prefix = "roc_N" + std::to_string(plen);

        prach::Transceiver::precalibrate_noise(
            (plen == 839) ? 839 : (plen == 1024) ? 1024 : 2048,
            839, 30, 49.0
        );

        prach::RocExperiment experiment(cfg);
        experiment.run();
    }

    return 0;
}
