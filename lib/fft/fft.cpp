#include "fft.hpp"

#include <fftw3.h>

namespace prach {

std::vector<Complex> fft_fftw(const std::vector<Complex>& x, size_t N) {
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

std::vector<Complex> fft_fftw_padded(const std::vector<Complex>& x, size_t N) {
    if (x.empty()) {
        throw std::invalid_argument("Input sequence is empty");
    }

    size_t L = x.size();
    size_t left_part_size = (L + 1) / 2;
    size_t right_part_size = L / 2;

    std::vector<Complex> x_padded(N, Complex(0, 0));

    std::copy(x.begin(),
              x.begin() + left_part_size,
              x_padded.begin());

    size_t right_start = N - right_part_size;
    std::copy(x.begin() + left_part_size,
              x.end(),
              x_padded.begin() + right_start);

    return fft_fftw(x_padded, N);
}

} // namespace prach
