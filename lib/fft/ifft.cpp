#include "fft.hpp"

#include <fftw3.h>

namespace prach {

// IFFT without normalization
std::vector<Complex> ifft_fftw(const std::vector<Complex>& X, size_t N) {
    fftw_complex* in = fftw_alloc_complex(N);
    fftw_complex* out = fftw_alloc_complex(N);

    for (size_t i = 0; i < N; ++i) {
        in[i][0] = X[i].real();
        in[i][1] = X[i].imag();
    }

    fftw_plan plan = fftw_plan_dft_1d(static_cast<int>(N), in, out,
                                      FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(plan);

    // Optionally: TODO normalization
    std::vector<Complex> x(N);
    for (size_t i = 0; i < N; ++i) {
        x[i] = Complex(out[i][0], out[i][1]) / static_cast<double>(N);
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return x;
}

} // namespace prach
