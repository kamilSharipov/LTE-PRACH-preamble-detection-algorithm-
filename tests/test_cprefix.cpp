#include "cprefix.hpp"
#include "test_utils.hpp"

#include <complex>
#include <vector>

#include <gtest/gtest.h>

using namespace prach;

TEST(CPrefixTest, BasicFunctionality) {
    std::vector<Complex> signal = {{1, 0}, {2, 0}, {3, 0}};
    auto result = insert_cyclic_prefix(signal, 2);

    std::vector<Complex> expected = {
        {2, 0}, {3, 0},
        {1, 0}, {2, 0}, {3, 0}
    };

    ASSERT_TRUE(complex_vectors_near(result, expected));
}

TEST(CPrefixTest, ZeroCpLength) {
    std::vector<Complex> signal = {{1, 1}, {2, -1}, {0, 3}};
    auto result = insert_cyclic_prefix(signal, 0);

    EXPECT_TRUE(complex_vectors_near(result, signal));
}

TEST(CPrefixTest, CpEqualsSignalLength) {
    std::vector<Complex> signal = {{1, 0}, {0, 1}};
    auto result = insert_cyclic_prefix(signal, 2);

    std::vector<Complex> expected = {
        {1, 0}, {0, 1},
        {1, 0}, {0, 1}
    };

    ASSERT_TRUE(complex_vectors_near(result, expected));
}

TEST(CPrefixTest, EmptySignal) {
    std::vector<Complex> empty;
    EXPECT_THROW(insert_cyclic_prefix(empty, 1), std::invalid_argument);
}

TEST(CPrefixTest, CpLongerThanSignal) {
    std::vector<Complex> signal = {{1, 0}, {2, 0}};
    EXPECT_THROW(insert_cyclic_prefix(signal, 5), std::invalid_argument);
}

TEST(CPrefixTest, ComplexValues) {
    std::vector<Complex> signal = {
        Complex(1.0, 2.0),
        Complex(-1.0, 0.5),
        Complex(0.0, -3.0)
    };
    auto result = insert_cyclic_prefix(signal, 1);

    std::vector<Complex> expected = {
        Complex(0.0, -3.0),
        Complex(1.0, 2.0),
        Complex(-1.0, 0.5),
        Complex(0.0, -3.0)
    };
    ASSERT_TRUE(complex_vectors_near(result, expected));
}

TEST(CPrefixTest, LargePrachLikeCase) {
    const size_t Nzc = 839;
    const size_t Ncp = 106;
    std::vector<Complex> signal(Nzc, Complex(1.0, 0.0));

    auto result = insert_cyclic_prefix(signal, Ncp);

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
