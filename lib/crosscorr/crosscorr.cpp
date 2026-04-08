#include "crosscorr.hpp"
#include "fft.hpp"
#include "ifft.hpp"

#include <iostream>

namespace prach {

std::vector<Complex> count_cross_correlation(const std::vector<Complex>& x,
                                             const std::vector<Complex>& y,
                                             const size_t N) 
{
    std::vector<Complex> res(N);

    for (size_t i = 0; i < N; i++) {
        res[i] = x[i] * std::conj(y[i]);
    }

    std::vector<Complex> ifft_res = ifft_fftw(res, N);
    return ifft_res;
}

} // namespace prach
