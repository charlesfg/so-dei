import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

def plot_charts_per_difficulty(csv_file):
    df = pd.read_csv(csv_file)
    difficulties = sorted(df['difficult'].unique())

    markers = ['o', 's', '^', 'D', 'v', 'P', '*', 'X', '<', '>']
    colors = plt.cm.tab10.colors

    for idx, d in enumerate(difficulties):
        subset = df[df['difficult'] == d].reset_index(drop=True)

        fig, axes = plt.subplots(2, 1, figsize=(16, 8), gridspec_kw={'height_ratios': [1, 1.2]})
        plt.subplots_adjust(hspace=0.4)

        # First chart: Boxplot (candlestick-like)
        axes[0].boxplot(subset['time'], widths=0.3, patch_artist=True, medianprops={'color':'black'})
        axes[0].set_xticks([1])
        axes[0].set_xticklabels([f'Difficulty {d}'])
        axes[0].set_ylabel('Time')
        axes[0].grid(True, which="both", linestyle="--", linewidth=0.5) 
        axes[0].set_title(f'Time Distribution for Difficulty {d}')

        # Second chart: Scatter plot with mean and std deviation
        x = np.arange(1, len(subset) + 1)
        y = subset['time']

        axes[1].scatter(x, y, label=f'Transactions (D{d})', marker=markers[idx % len(markers)], color=colors[idx % len(colors)])

        mean_time = y.mean()
        std_time = y.std()

        axes[1].hlines(mean_time, x[0], x[-1], colors='red', linestyles='--', label='Mean')
        axes[1].fill_between(x, mean_time - std_time, mean_time + std_time, color='red', alpha=0.1, label='Std Dev')

        axes[1].set_xlabel('Transaction ID')
        axes[1].set_ylabel('Time')
        axes[1].grid(True, which="both", linestyle="--", linewidth=0.5) 
        axes[1].set_title(f'Transaction Times with Mean and Std Dev (Difficulty {d})')
        axes[1].legend()

        plt.tight_layout()
        plt.savefig(PNG_FILE.format(d), dpi=300)
        

if __name__ == "__main__":
    if len(sys.argv) < 2:
        print("Usage: python plot_script.py <csv_file> <file_patter>")
        sys.exit(1)

    csv_file = sys.argv[1]
    PNG_FILE = sys.argv[2] + "_{}.png"
    plot_charts_per_difficulty(csv_file)
