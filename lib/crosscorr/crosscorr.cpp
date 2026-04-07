#include "crosscorr.hpp"
#include "fft.hpp"
#include "ifft.hpp"

#include <iostream>

namespace prach {

std::vector<Complex> count_cross_correlation(const std::vector<Complex>& x,
                                             const std::vector<Complex>& y,
                                             const size_t N) 
{
    size_t L               = y.size();
    size_t left_part_size  = (L + 1) / 2;
    size_t right_part_size = L / 2;

    std::vector<Complex> Y_padded(N, Complex(0, 0));

    std::copy(y.begin(),
              y.begin() + left_part_size,
              Y_padded.begin());

    size_t right_start = N - right_part_size;

    std::copy(y.begin() + left_part_size,
              y.end(),
              Y_padded.begin() + right_start);

    std::vector<Complex> res(N);

    for (size_t i = 0; i < N; i++) {
        res[i] = x[i] * std::conj(Y_padded[i]);
    }

    std::vector<Complex> ifft_res = ifft_fftw(res, N);
    return ifft_res;
}

} // namespace prach
