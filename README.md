# LTE PRACH Preamble Generation & Detection Toolkit

This project implements generation and analysis of PRACH preambles according to the 3GPP LTE specification (TS 36.211) for research and development of preamble detection algorithms.

This is an academic project. Use for learning, experimentation, and algorithm prototyping only.

## Key Components
TODO

## Technology Stack

| Category       | Technologies |
|----------------|--------------|
| Language       | C++17        |
| Build System   | CMake 3.14+  |
| Testing        | Google Test  |
| FFT Library    | FFTW3        |

## TODO

## Getting Started

### Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install cmake build-essential libfftw3-dev python3 python3-pip

# Python dependencies for plotting
pip3 install pandas matplotlib numpy
```

### Build

```bash
git clone https://github.com/kamilSharipov/LTE-PRACH-preamble-detection-algorithm-.git
cd LTE-PRACH-preamble-detection-algorithm-

# Build (Release mode)
mkdir build && cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
make -j$(nproc)

# Run example
./example

# Run tests
ctest --output-on-failure --verbose
```
### ROC Experiment
```bash
cd build

# Run ROC experiment (generates CSV data files)
make run_roc

# Generate ROC plots from existing data
make plot_roc
```