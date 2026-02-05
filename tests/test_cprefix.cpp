#include <vector>
#include <complex>

#include <gtest/gtest.h>

#include "cprefix.hpp"

using namespace prach;

bool complexVectorsNear(const std::vector<Complex>& a,
                        const std::vector<Complex>& b,
                        double eps = 1e-9) {
    if (a.size() != b.size()) {
        return false;
    }

    for (size_t i = 0; i < a.size(); ++i) {
        if (std::abs(a[i] - b[i]) > eps) {
            return false;
        }
    }

    return true;
}

TEST(CPrefixTest, BasicFunctionality) {
    std::vector<Complex> signal = {{1, 0}, {2, 0}, {3, 0}};
    auto result = add_cyclic_prefix(signal, 2);

    std::vector<Complex> expected = {
        {2, 0}, {3, 0},
        {1, 0}, {2, 0}, {3, 0}
    };

    ASSERT_TRUE(complexVectorsNear(result, expected));
}

TEST(CPrefixTest, ZeroCpLength) {
    std::vector<Complex> signal = {{1, 1}, {2, -1}, {0, 3}};
    auto result = add_cyclic_prefix(signal, 0);

    EXPECT_TRUE(complexVectorsNear(result, signal));
}

TEST(CPrefixTest, CpEqualsSignalLength) {
    std::vector<Complex> signal = {{1, 0}, {0, 1}};
    auto result = add_cyclic_prefix(signal, 2);

    std::vector<Complex> expected = {
        {1, 0}, {0, 1},
        {1, 0}, {0, 1}
    };

    ASSERT_TRUE(complexVectorsNear(result, expected));
}

TEST(CPrefixTest, EmptySignal) {
    std::vector<Complex> empty;
    EXPECT_THROW(add_cyclic_prefix(empty, 1), std::invalid_argument);
}

TEST(CPrefixTest, CpLongerThanSignal) {
    std::vector<Complex> signal = {{1, 0}, {2, 0}};
    EXPECT_THROW(add_cyclic_prefix(signal, 5), std::invalid_argument);
}

TEST(CPrefixTest, ComplexValues) {
    std::vector<Complex> signal = {
        Complex(1.0, 2.0),
        Complex(-1.0, 0.5),
        Complex(0.0, -3.0)
    };
    auto result = add_cyclic_prefix(signal, 1);

    std::vector<Complex> expected = {
        Complex(0.0, -3.0),
        Complex(1.0, 2.0),
        Complex(-1.0, 0.5),
        Complex(0.0, -3.0)
    };
    ASSERT_TRUE(complexVectorsNear(result, expected));
}

TEST(CPrefixTest, LargePrachLikeCase) {
    const size_t Nzc = 839;
    const size_t Ncp = 106;
    std::vector<Complex> signal(Nzc, Complex(1.0, 0.0));

    auto result = add_cyclic_prefix(signal, Ncp);

    ASSERT_EQ(result.size(), Nzc + Ncp);

    for (size_t i = 0; i < Ncp; ++i) {
        EXPECT_NEAR(result[i].real(), 1.0, 1e-9);
        EXPECT_NEAR(result[i].imag(), 0.0, 1e-9);
    }

    for (size_t i = 0; i < Nzc; ++i) {
        EXPECT_NEAR(result[Ncp + i].real(), 1.0, 1e-9);
        EXPECT_NEAR(result[Ncp + i].imag(), 0.0, 1e-9);
    }
}
