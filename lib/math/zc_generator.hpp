#include <complex>
#include <vector>

#include <fftw3.h>

namespace prach {

const size_t STANDART_NZC = 839;

using Complex = fftw_complex;

std::vector<Complex> generate_zc_sequence(int u, int Nzc = STANDART_NZC);
std::vector<Complex> apply_cyclic_shift(const std::vector<Complex>& seq, int delay);

} // namespace prach
