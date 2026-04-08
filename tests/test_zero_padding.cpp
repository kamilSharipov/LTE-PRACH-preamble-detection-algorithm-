#include "fft.hpp"
#include "ifft.hpp"
#include "zc_generator.hpp"
#include "zc_fft.property.hpp"
#include "utils.hpp"
#include "test_utils.hpp"

#include <vector>
#include <complex>
#include <cmath>

#include <gtest/gtest.h>


using namespace prach;

TEST(ZeroPaddingTest, FFT_IFFT_RoundtripPreservesSignal_839) {
    const size_t Nzc = 839;
    const int u = 25;
    const size_t Ndft = 839;

    auto freq_signal = dft_via_zc_property(Nzc, u);

    auto padded_freq_signal = add_zero_padding(freq_signal, Ndft);
    
    auto time_restored = ifft_fftw(padded_freq_signal, Ndft);
    auto freq_restored = fft_fftw(time_restored, Ndft);

    EXPECT_TRUE(complex_vectors_near(padded_freq_signal, freq_restored, 1e-6));
}

TEST(ZeroPaddingTest, FFT_IFFT_RoundtripPreservesSignal_1024) {
    const size_t Nzc = 839;
    const int u = 25;
    const size_t Ndft = 1024;

    auto freq_signal = dft_via_zc_property(Nzc, u);

    auto padded_freq_signal = add_zero_padding(freq_signal, Ndft);
    
    auto time_restored = ifft_fftw(padded_freq_signal, Ndft);
    auto freq_restored = fft_fftw(time_restored, Ndft);

    EXPECT_TRUE(complex_vectors_near(padded_freq_signal, freq_restored, 1e-6));
}

TEST(ZeroPaddingTest, FFT_IFFT_RoundtripPreservesSignal_2048) {
    const size_t Nzc = 839;
    const int u = 25;
    const size_t Ndft = 2048;

    auto freq_signal = dft_via_zc_property(Nzc, u);

    auto padded_freq_signal = add_zero_padding(freq_signal, Ndft);
    
    auto time_restored = ifft_fftw(padded_freq_signal, Ndft);
    auto freq_restored = fft_fftw(time_restored, Ndft);

    EXPECT_TRUE(complex_vectors_near(padded_freq_signal, freq_restored, 1e-6));
}
