#pragma once

#include <vector>
#include <complex>

#include "prach.hpp"

#include "fftw3.h"

namespace prach {

inline bool complex_vectors_near(const std::vector<Complex>& a,
                                 const std::vector<Complex>& b,
                                 double eps = 1e-9) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > eps) {
            return false;
        }
    }

    return true;
}

} // namespace prach
