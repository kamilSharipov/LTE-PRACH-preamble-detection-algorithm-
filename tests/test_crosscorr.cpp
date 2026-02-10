#include "cprefix.hpp"
#include "crosscorr.hpp"
#include "test_utils.hpp"
#include "zc_generator.hpp"

#include <gtest/gtest.h>

using namespace prach;

std::vector<Complex> count_cross_correlation_naive(std::vector<Complex>& x, 
                                            std::vector<Complex>& y) {
    size_t N = x.size();
    std::vector<Complex> res(N, Complex{0, 0});

    for (size_t lag = 0; lag < N; ++lag) {
        Complex sum{0, 0};

        for (size_t n = 0; n < N; ++n) {
            size_t m = (n + lag) % N;
            sum += x[n] * std::conj(y[m]); 
        }

        res[lag] = sum;
    }
    
    return res;
}


TEST(CrossCorrTest, AutoCorrelationPeakAtZero) {
    const size_t Nzc = 839;
    const int u = 25;
    auto x = generate_zc_sequence(u, Nzc);

    auto corr = count_cross_correlation(x, x, Nzc);

    size_t max_index = 0;
    double max_value = std::abs(corr[0]);

    for (size_t i = 0; i < Nzc; ++i) {
        double value = std::abs(corr[i]);

        if (value > max_value) {
            max_value = value;
            max_index = i;
        }
    }

    EXPECT_EQ(max_index, 0);
    EXPECT_NEAR(std::abs(corr[0]), static_cast<double>(Nzc), 1e-6);
}

TEST(CrossCorrTest, MatchesNaiveAutoCorrelation) {
    const size_t Nzc = 839;
    const int u = 25;
    auto x = generate_zc_sequence(u, Nzc);

    auto fft_corr = count_cross_correlation(x, x, Nzc);
    auto naive_corr = count_cross_correlation_naive(x, x);

    ASSERT_TRUE(complex_vectors_near(fft_corr, naive_corr, 1e-8));
}

TEST(CrossCorrTest, MatchesNaiveCrossCorrelation) {
    const size_t Nzc = 839;
    const int u1 = 25, u2 = 29;
    auto x = generate_zc_sequence(u1, Nzc);
    auto y = generate_zc_sequence(u2, Nzc);

    auto fft_corr = count_cross_correlation(x, y, Nzc);
    auto naive_corr = count_cross_correlation_naive(x, y);

    ASSERT_TRUE(complex_vectors_near(fft_corr, naive_corr, 1e-8));
}

TEST(CrossCorrTest, LowCrossCorrelationForDifferentRoots) {
    const size_t Nzc = 839;
    const int u1 = 25;
    const int u2 = 26;

    auto x1 = generate_zc_sequence(u1, Nzc);
    auto x2 = generate_zc_sequence(u2, Nzc);

    auto corr = count_cross_correlation(x1, x2, Nzc);

    double max_corr = 0.0;
    for (const auto& c : corr) {
        max_corr = std::max(max_corr, std::abs(c));
    }

    EXPECT_LT(max_corr, std::sqrt(static_cast<double>(Nzc)) + 1.0);
}
