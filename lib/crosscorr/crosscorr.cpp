#include "crosscorr.hpp"
#include "fft.hpp"
#include "ifft.hpp"
#include "zc_fft.property.hpp"

namespace prach {

std::vector<Complex> count_cross_correlation(std::vector<Complex>& x,
                                             std::vector<Complex>& y,
                                             size_t N) {
    std::vector<Complex> fft_x = fft_fftw(x, N);
    std::vector<Complex> fft_y = fft_fftw(y, N);
    std::vector<Complex> res(N);

    for (size_t i = 0; i < N; i++) {
        res[i] = fft_x[i] * std::conj(fft_y[i]);
    }
    
    std::vector ifft_res = ifft_fftw(res, N);
    return ifft_res;
}

} // namespace prach
