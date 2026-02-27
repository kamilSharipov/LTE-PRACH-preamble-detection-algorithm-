#include "fft.hpp"

#include <fftw3.h>

namespace prach {

// IFFT with normalization
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

    std::vector<Complex> x(N);
    for (size_t i = 0; i < N; ++i) {
        x[i] = Complex(out[i][0], out[i][1]) / static_cast<double>(N);
    }

    fftw_destroy_plan(plan);
    fftw_free(in);
    fftw_free(out);

    return x;
}

std::vector<Complex> ifft_fftw_padded(const std::vector<Complex>& X, size_t N) {
    if (X.empty()) {
        throw std::invalid_argument("Input spectrum is empty");
    }

    size_t L = X.size();
    size_t left_part_size = (L + 1) / 2;
    size_t right_part_size = L / 2;

    std::vector<Complex> X_padded(N, Complex(0, 0));

    std::copy(X.begin(),
              X.begin() + left_part_size,
              X_padded.begin());

    size_t right_start = N - right_part_size;
    std::copy(X.begin() + left_part_size,
              X.end(),
              X_padded.begin() + right_start);

    std::vector<Complex> ifft_res = ifft_fftw(X_padded, N);

    //double scale = static_cast<double>(N) / L;
    //for (size_t i = 0; i < N; ++i) {
    //    ifft_res[i] *= scale;
    //}

    return ifft_res;
}

} // namespace prach
