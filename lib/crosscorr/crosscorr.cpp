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

    //for (size_t i = 0; i < N; ++i) {
    //    std::cout << "product[" << i << "] = " << std::abs(res[i]) << "\n";
    //}

    std::vector<Complex> ifft_res = ifft_fftw(res, N);

    //double scale = std::sqrt(static_cast<double>(N) / 839);
    //for (auto& el: ifft_res) {
    //    el *= scale;
    //}

    return ifft_res;
}

} // namespace prach
