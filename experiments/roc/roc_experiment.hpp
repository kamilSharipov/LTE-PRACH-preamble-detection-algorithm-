#pragma once
#include <vector>
#include <string>
#include "transceiver.hpp"

namespace prach {

struct RocPoint {
    double pfa;
    double pd;
    double threshold_param;
};

class RocExperiment {
public:
    struct Config {
        size_t num_trials = 1000;

        std::vector<double> noise_var_values = {49.0};
        std::vector<double> threshold_params;

        DetectorType detector_type;

        std::string output_file_prefix = "roc_results";

        int preamble_length = 839;
    
        bool use_resampling      = false;
        int  oversampling_factor = 4;
        double max_delay_us      = 0.0;
    };

    explicit RocExperiment(Config cfg) : cfg_(cfg) {}

    void run();

private:
    Config cfg_;
    
    std::pair<double, double> compute_pfa_pd(double snr_db, double threshold_param);
    Transceiver create_transceiver(double snr_db, double threshold_param);
};

} // namespace prach
