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

inline std::vector<Complex> fft_fftw(const std::vector<Complex>& x) {
    size_t N = x.size();
    fftw_complex* in = fftw_alloc_complex(N);
    fftw_complex* out = fftw_alloc_complex(N);

    for (size_t i = 0; i < N; ++i) {
        in[i][0] = x[i].real();
        in[i][1] = x[i].imag();
    }

    fftw_plan plan = fftw_plan_dft_1d(static_cast<int>(N), in, out,
                                      FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    std::vector<Complex> X(N);
    for (size_t i = 0; i < N; ++i) {
        X[i] = Complex(out[i][0], out[i][1]);
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return X;
}

} // namespace prach
