import pandas as pd
import matplotlib.pyplot as plt
import glob
import re
import numpy as np
import os


def get_color_map(unique_curves):
    cmap = plt.cm.tab10  # tab10, tab20, Set1, Set2, Set3, Dark2, Paired
    colors = {}
    for i, curve_name in enumerate(sorted(unique_curves)):
        colors[curve_name] = cmap(i % cmap.N)
    return colors


N_STYLES = {
    839: {'linestyle': '-', 'marker': 'o', 'markersize': 6},
    1024: {'linestyle': '-', 'marker': 's', 'markersize': 6},
    2048: {'linestyle': '-', 'marker': '^', 'markersize': 6},
}

plt.figure(figsize=(14, 10))

data_dir = os.path.join(os.path.dirname(__file__), 'data')
files = glob.glob(os.path.join(data_dir, 'roc_N*_noisevar_*.csv'))

if not files:
    files = glob.glob('data/roc_N*_noisevar_*.csv')
if not files:
    files = glob.glob('roc_N*_noisevar_*.csv')

print(f"Found {len(files)} files")

curves_info = []
for file in sorted(files):
    basename = os.path.basename(file)
    match = re.search(r'roc_N(\d+)_noisevar_([\d\.]+)\.csv', basename)
    if not match:
        continue

    N = int(match.group(1))
    noise_var = float(match.group(2))

    if N not in N_STYLES:
        continue

    curves_info.append((N, noise_var, file))

print(f"Processing {len(curves_info)} valid curves")

curve_ids = [f"N={N}, σ²={noise_var:.0f}" for N, noise_var, _ in curves_info]
color_map = get_color_map(curve_ids)

plotted_combinations = set()

for N, noise_var, file in curves_info:
    df = pd.read_csv(file)
    df = df[(df['pfa'] > 0) & (df['pfa'] <= 1)]

    if df.empty:
        print(f"Warning: Empty dataframe for {file}")
        continue

    df = df.sort_values('pfa')

    style = N_STYLES[N]

    label = f"N={N}, σ²={noise_var:.0f}"

    if (N, noise_var) not in plotted_combinations:
        plotted_combinations.add((N, noise_var))
    else:
        label = None

    color = color_map[label if label else f"N={N}, σ²={noise_var:.0f}"]

    plt.plot(df['pfa'], df['pd'],
             linestyle=style['linestyle'],
             marker=style['marker'],
             color=color,
             label=label,
             linewidth=2,
             markersize=style['markersize'],
             markevery=1)

plt.xlabel('PFA (Probability of False Alarm)', fontsize=14, fontweight='bold')
plt.ylabel('PD (Probability of Detection)', fontsize=14, fontweight='bold')
plt.title('ROC Curves for Different Preamble Lengths and Noise Variance',
          fontsize=16, fontweight='bold')

handles, labels = plt.gca().get_legend_handles_labels()
if handles:
    plt.legend(fontsize=10, loc='lower right', framealpha=0.95,
               fancybox=True, shadow=True, ncol=2)
else:
    print("Warning: No handles for legend")

plt.grid(True, which='both', linestyle=':', alpha=0.7, linewidth=0.5)

plt.xscale('linear')
plt.yscale('linear')
plt.ylim(0, 1.05)

plt.tight_layout()

output_path = os.path.join(data_dir if 'data_dir' in locals() else '.', 'roc_curves_all.png')
plt.savefig(output_path, dpi=300, bbox_inches='tight')
print(f"Saved plot to {output_path}")

plt.show()