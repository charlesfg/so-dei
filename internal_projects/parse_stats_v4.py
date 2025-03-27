import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
import sys

def plot_charts(csv_file):
    df = pd.read_csv(csv_file)

    fig, axes = plt.subplots(2, 1, figsize=(16, 8), gridspec_kw={'height_ratios': [1, 1.2]})
    plt.subplots_adjust(hspace=0.4)

    # First chart: Time distributions (candlestick-like)
    difficulties = sorted(df['difficult'].unique())
    data = [df[df['difficult'] == d]['time'] for d in difficulties]

    axes[0].boxplot(data, positions=difficulties, widths=0.4, patch_artist=True, medianprops={'color':'black'})
    axes[0].set_xticks(difficulties)
    axes[0].set_xlabel('Difficulty')
    axes[0].set_ylabel('Time')
    axes[0].grid(True, which="both", linestyle="--", linewidth=0.5) 
    axes[0].set_title('Time Distribution by Difficulty (Boxplot)')

    # Second chart: Scatter plot with average and standard deviation
    markers = ['o', 's', '^', 'D', 'v', 'P', '*', 'X', '<', '>']
    colors = plt.cm.tab10.colors

    for idx, d in enumerate(difficulties):
        subset = df[df['difficult'] == d].reset_index(drop=True)
        x = np.arange(1, len(subset) + 1)
        y = subset['time']

        # Scatter points
        axes[1].scatter(x, y, label=f'Difficulty {d}', marker=markers[idx % len(markers)], color=colors[idx % len(colors)])

        # Mean and std lines
        mean_time = y.mean()
        std_time = y.std()
        axes[1].hlines(mean_time, x[0], x[-1], colors=colors[idx % len(colors)], linestyles='--', label=f'Mean D{d}')
        axes[1].fill_between(x, mean_time - std_time, mean_time + std_time, color=colors[idx % len(colors)], alpha=0.1, label=f'Std D{d}')

    axes[1].set_xlabel('Transaction ID')
    axes[1].set_ylabel('Time')
    axes[1].set_title('Transactions Time with Mean and Std by Difficulty')
    axes[1].grid(True, which="both", linestyle="--", linewidth=0.5)
    axes[1].legend()

    # Adjust layout and save figure
    plt.tight_layout()
    plt.savefig(PNG_FILE, dpi=300)

if __name__ == "__main__":
    if len(sys.argv) < 3:
        print("Usage: python plot_script.py <csv_file> <chart_file>")
        sys.exit(1)

    csv_file = sys.argv[1]
    PNG_FILE = sys.argv[2]
    plot_charts(csv_file)
