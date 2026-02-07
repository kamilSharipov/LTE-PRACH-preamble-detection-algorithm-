#include "zc_fft.property.hpp"
#include "zc_generator.hpp"
#include "test_utils.hpp"

#include <cmath>
#include <complex>
#include <vector>
#include <iostream>

#include <gtest/gtest.h>
#include <fftw3.h>

using namespace prach;

class ZCFFTPropertyTest : public ::testing::TestWithParam<std::tuple<size_t, int>> {};

TEST_P(ZCFFTPropertyTest, MatchesFFTW) {
    auto [Nzc, u] = GetParam();

    auto X_fast = dft_via_zc_property(Nzc, u);
    auto x_u = generate_zc_sequence(u, Nzc);
    auto X_fftw = fft_fftw(x_u);

    double eps = 1e-7;

    ASSERT_TRUE(complex_vectors_near(X_fast, X_fftw, eps));
}

INSTANTIATE_TEST_SUITE_P(
    SmallN, ZCFFTPropertyTest,
    ::testing::Values(
        std::make_tuple(11, 3),
        std::make_tuple(13, 5),
        std::make_tuple(17, 7)
    )
);

INSTANTIATE_TEST_SUITE_P(
    LTE_Sizes, ZCFFTPropertyTest,
    ::testing::Values(
        std::make_tuple(139, 25),
        std::make_tuple(139, 100),
        std::make_tuple(839, 25),
        std::make_tuple(839, 500),
        std::make_tuple(839, 838)
    )
);

TEST(ZCFFTPropertyTest, BoundaryRootIndices) {
    const size_t Nzc = 139;

    auto X1_fast = dft_via_zc_property(Nzc, 1);
    auto x1 = generate_zc_sequence(1, Nzc);
    auto X1_fftw = fft_fftw(x1);
    EXPECT_TRUE(complex_vectors_near(X1_fast, X1_fftw, 1e-9));

    auto X2_fast = dft_via_zc_property(Nzc, static_cast<int>(Nzc - 1));
    auto x2 = generate_zc_sequence(static_cast<int>(Nzc - 1), Nzc);
    auto X2_fftw = fft_fftw(x2);
    EXPECT_TRUE(complex_vectors_near(X2_fast, X2_fftw, 1e-9));
}

TEST(ZCFFTPropertyTest, InvalidURaisesException) {
    const size_t Nzc = 11;
    EXPECT_THROW(dft_via_zc_property(Nzc, 0), std::invalid_argument);
    EXPECT_THROW(dft_via_zc_property(Nzc, static_cast<int>(Nzc)), std::invalid_argument);
    EXPECT_THROW(dft_via_zc_property(Nzc, -5), std::invalid_argument);
}
