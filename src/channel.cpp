#include "channel.hpp"

#include <random>
#include <cmath>
#include <iostream>

namespace prach {

std::vector<Complex> Channel::apply(const std::vector<Complex>& tx) const {
    size_t delay_samples = static_cast<size_t>(cfg_.delay_sec * cfg_.fs);
    std::vector<Complex> rx(delay_samples, Complex(0, 0));
    rx.insert(rx.end(), tx.begin(), tx.end());

    double phase = 0.0;
    double phase_step = 2.0 * PI * cfg_.freq_offset_hz / cfg_.fs;
    for (auto& s : rx) {
        s *= Complex(std::cos(phase), std::sin(phase));
        phase += phase_step;
    }

    double signal_power = 0.0;
    for (const auto& s : tx) {
        signal_power += std::norm(s);
    }
    signal_power /= tx.size();

    std::cout << signal_power << "\n";

    double noise_power = signal_power / std::pow(10.0, cfg_.snr_db / 10.0);
    std::normal_distribution<double> noise_dist(0.0, std::sqrt(noise_power / 2.0));
    std::random_device rd;
    std::seed_seq seed{rd(), rd(), rd(), rd()};
    std::mt19937 gen(seed);

    for (auto& s : rx) {
        s += Complex(noise_dist(gen), noise_dist(gen));
    }

    return rx;
}

} // namespace prach
