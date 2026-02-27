import numpy as np
import matplotlib.pyplot as plt

amplitude = np.loadtxt('../build/corr_phase.txt')

plt.figure(figsize=(12, 6))
plt.plot(amplitude)
plt.title('Cross-Correlation Amplitude')
plt.xlabel('τ')
plt.ylabel('|R(τ)|')
plt.grid(True)
plt.savefig('correlation.png', dpi=150)
plt.show()

print(max(amplitude))