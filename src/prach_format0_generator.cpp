#include "zc_sequence_generator.hpp"

#include <iostream>
#include <vector>
#include <complex>
#include <fstream>
#include <cmath>
#include <algorithm>

using namespace prach;

// Вспомогательная функция: DFT
std::vector<Complex> dft(const std::vector<Complex>& x) {
    size_t N = x.size();
    std::vector<Complex> X(N, 0.0);
    const double pi = acos(-1.0);
    for (size_t k = 0; k < N; ++k) {
        for (size_t n = 0; n < N; ++n) {
            double angle = -2.0 * pi * k * n / N;
            X[k] += x[n] * Complex(cos(angle), sin(angle));
        }
    }
    return X;
}

// Вспомогательная функция: IDFT
std::vector<Complex> idft(const std::vector<Complex>& X) {
    size_t N = X.size();
    std::vector<Complex> x(N, 0.0);
    const double pi = acos(-1.0);
    for (size_t n = 0; n < N; ++n) {
        for (size_t k = 0; k < N; ++k) {
            double angle = 2.0 * pi * k * n / N;
            x[n] += X[k] * Complex(cos(angle), sin(angle));
        }
        x[n] /= static_cast<double>(N);
    }
    return x;
}

// Интерполяция через DFT/IDFT до новой длины
std::vector<Complex> interpolate_zc(const std::vector<Complex>& zc, size_t target_len) {
    size_t Nzc = zc.size(); // 839
    // Шаг 1: DFT от исходной последовательности
    auto X = dft(zc);
    
    // Шаг 2: Вставить нули в частотную область
    std::vector<Complex> X_interp(target_len, 0.0);
    // Копируем низкие частоты
    size_t half = Nzc / 2;
    for (size_t i = 0; i <= half; ++i) {
        X_interp[i] = X[i];
    }
    // Копируем высокие частоты (для циклической симметрии)
    for (size_t i = half + 1; i < Nzc; ++i) {
        X_interp[target_len - (Nzc - i)] = X[i];
    }

    // Шаг 3: IDFT → временная последовательность длиной target_len
    auto interpolated = idft(X_interp);
    return interpolated;
}

// Сохранение в бинарный файл
void save_complex_vector(const std::vector<Complex>& data, const std::string& filename) {
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        throw std::runtime_error("Cannot open file for writing: " + filename);
    }
    size_t len = data.size();
    file.write(reinterpret_cast<const char*>(&len), sizeof(len));
    file.write(reinterpret_cast<const char*>(data.data()), len * sizeof(Complex));
    file.close();
}

int main() {
    try {
        const int u = 25; // корневой индекс
        const size_t Nzc = 839;
        const size_t signal_len = 24576; // полезная часть
        const size_t cp_len = 3168;      // cyclic prefix
        const size_t gt_len = 2880;      // guard time

        std::cout << "Генерация PRACH преамбулы (формат 0, LTE)...\n";

        // 1. ZC-последовательность
        auto zc = generate_zc(u, Nzc);

        // 2. Интерполяция до 24576 отсчётов
        std::cout << "Интерполяция ZC до " << signal_len << " отсчётов...\n";
        auto signal = interpolate_zc(zc, signal_len);

        // 3. Циклический префикс = копия конца сигнала
        std::vector<Complex> cp(cp_len);
        for (size_t i = 0; i < cp_len; ++i) {
            cp[i] = signal[signal_len - cp_len + i];
        }

        // 4. Защитный интервал = нули
        std::vector<Complex> gt(gt_len, Complex(0.0, 0.0));

        // 5. Сборка полной преамбулы: [CP][signal][GT]
        std::vector<Complex> preamble;
        preamble.reserve(cp_len + signal_len + gt_len);
        preamble.insert(preamble.end(), cp.begin(), cp.end());
        preamble.insert(preamble.end(), signal.begin(), signal.end());
        preamble.insert(preamble.end(), gt.begin(), gt.end());

        std::cout << "Длина CP: " << cp_len << "\n";
        std::cout << "Длина сигнала: " << signal_len << "\n";
        std::cout << "Длина GT: " << gt_len << "\n";
        std::cout << "Общая длина: " << preamble.size() << " отсчётов\n";

        // 6. Сохранение
        save_complex_vector(preamble, "prach_format0.bin");
        std::cout << "Преамбула сохранена в prach_format0.bin\n";

    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << "\n";
        return 1;
    }
    return 0;
}
