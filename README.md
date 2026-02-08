# LTE PRACH Preamble Detection & Generation Toolkit

This project implements **generation and analysis of PRACH preambles** according to the 3GPP LTE specification (TS 36.211), with a focus on **high performance** and **accurate channel emulation** for research and development of preamble detection algorithms.

This is an **academic project**. It is not intended for production use, and may not fully comply with all aspects of the 3GPP LTE standard. Use for learning, experimentation, and algorithm prototyping only.

## Key Components

### 1. PRACH Preamble Generator (Implemented)
- Supports ZC sequence lengths \(N_{\mathrm{ZC}} = 839\) (standard formats) and \(139\) (short preambles).
- Generates sequences based on root index \(u\) and cyclic shift \(v\).
- Adds cyclic prefix (CP) according to preamble format (0–3).
- Direct time-domain generation (TODO).

### 2. Fast DFT of ZC Sequences
- Implements the analytical formula from Beyme & Leung, *Electronics Letters*, 2009.
- Complexity: **\(\mathcal{O}(P)\)** vs. \(\mathcal{O}(P \log P)\) with FFT.

## Technology Stack

| Category       | Technologies |
|----------------|--------------|
| Language       | C++17        |
| Build System   | CMake 3.14+  |
| Testing        | Google Test  |
| FFT Library    | FFTW3        |

## TODO

### Implement simple examples

### Detection Algorithm & Simulation Model
- [ ] **a.** Add additive white Gaussian noise for a given SNR.  
- [ ] **b.** Implement preamble detection algorithm.  
- [ ] **c.** Evaluate false alarm and miss detection probabilities.  
- [ ] **d.** Plot probability of correct detection vs. SNR.  
- [ ] **e.** Estimate time-of-arrival of the preamble.

### Performance Under Frequency Offset
- [ ] **a.** Analyze mathematically how frequency offset affects correlation peak.  
- [ ] **b.** Plot detection probability under various frequency offsets.  
- [ ] **c.** Investigate timing estimation bias caused by frequency offset.  
- [ ] **d.** Explore frequency offset compensation for preamble formats with two repeated sequences.

### Multi-User Scenario Evaluation
- [ ] **a.** Evaluate false alarm probability when multiple preambles (different roots/shifts) are present.  
- [ ] **b.** Plot detection probability for two simultaneously transmitted preambles.

## Getting Started

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
ctest --output-on-failure
