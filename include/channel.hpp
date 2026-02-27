#pragma once

#include "prach.hpp"

#include <vector>

namespace prach {

struct ChannelConfig {
    double snr_db = 10.0;
    double delay_sec = 0.0;
    double freq_offset_hz = 0.0;
    double fs = 1.92e6;
};

class Channel {
public:
    explicit Channel(ChannelConfig cfg) : cfg_(cfg) {}
    std::vector<Complex> apply(const std::vector<Complex>& tx) const;

private:
    ChannelConfig cfg_;
};

} // namespace prach
