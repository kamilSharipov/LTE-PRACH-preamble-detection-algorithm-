#include "fftw3.h"

#include <iostream>
#include <chrono>
#include <fstream>
#include <iomanip>

#include "zc_generator.hpp"
#include "cprefix.hpp"
#include "zc_fft.property.hpp"
#include "crosscorr.hpp"
#include "ifft.hpp"
#include "fft.hpp"

#include "base.hpp"
#include "generator.hpp"

using namespace prach;

int main() {
    try {
        prach::PreambleConfig cfg;
        cfg.N_zc = 839;
        cfg.root_index = 25;
        cfg.fs = 1.92e6;

        auto format = prach::create_format(prach::FormatType::LTE_FORMAT_0);
        std::vector<Complex> preamble = format->generate(cfg);
        auto fft_preamble = fft_fftw(preamble, 1024);

        std::vector<Complex> reference = dft_via_zc_property(cfg.N_zc, cfg.root_index);

        auto res = count_cross_correlation(fft_preamble, reference, 1024);

        for (auto el: res) {
            std::cout << std::abs(el) << "\n";
        }   

        {
            std::ofstream file("corr_phase.txt");
            file << std::fixed << std::setprecision(10);
            for (const auto& val : res) {
                file << std::abs(val) / static_cast<double>(839) << "\n";
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
