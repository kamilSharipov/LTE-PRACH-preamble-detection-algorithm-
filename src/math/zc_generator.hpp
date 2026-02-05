#include <complex>
#include <vector>

namespace prach {

const size_t STANDART_NZC = 839;

using Complex = std::complex<double>;

std::vector<Complex> generate_ZC_sequence(int u, int Nzc = STANDART_NZC);
std::vector<Complex> apply_cyclic_shift(const std::vector<Complex>& seq, int delay);

} // namespace prach
