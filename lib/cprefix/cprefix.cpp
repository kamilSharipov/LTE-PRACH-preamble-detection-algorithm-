#include "cprefix.hpp"

#include <stdexcept>

namespace prach {

std::vector<Complex> add_cyclic_prefix(const std::vector<Complex>& signal, 
                                       size_t cp_length) {
    if (cp_length == 0) {
        return signal;
    }

    if (cp_length > signal.size()) {
        throw std::invalid_argument("lib/cprefix/cprefix.cpp: CP length cannot exceed signal length");
    }

    std::vector<Complex> result;
    result.reserve(signal.size() + cp_length);

    result.insert(result.end(), signal.end() - cp_length, signal.end());
    result.insert(result.end(), signal.begin(), signal.end());

    return result;
}

} // namespace prach
