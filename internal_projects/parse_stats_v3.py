import re
import sys
import matplotlib.pyplot as plt
import pandas as pd
import os
import numpy as np

# Define CSV file path
CSV_FILE = "parsed_data_v32.csv"
PNG_FILE = "difficulty_performance_v32.png"

# Sample log content (replace with actual file reading if needed)

with open(sys.argv[1]) as f:
    log_content = f.read()

# Regular expressions to extract relevant data
difficulty_pattern = re.compile(r"PoW, difficulty (\d+)")
elapsed_time_pattern = re.compile(r"Elapsed Time: ([\d.]+) seconds")
operations_pattern = re.compile(r"Operations: (\d+)")

# Lists to store extracted data
data = []

# Parsing log content
difficulty_matches = difficulty_pattern.findall(log_content)
elapsed_time_matches = elapsed_time_pattern.findall(log_content)
operations_matches = operations_pattern.findall(log_content)

for i in range(len(difficulty_matches)):
    difficulty = int(difficulty_matches[i])
    elapsed_time = float(elapsed_time_matches[i])
    operations = int(operations_matches[i])

    data.append([difficulty, elapsed_time, operations])

# Convert to DataFrame
df_new = pd.DataFrame(data, columns=["Difficulty", "Elapsed Time (s)", "Operations"])

# Check if CSV already exists
if os.path.exists(CSV_FILE):
    df_existing = pd.read_csv(CSV_FILE)

    # Merge data while avoiding duplicates
    df_combined = pd.concat([df_existing, df_new]).drop_duplicates().reset_index(drop=True)
else:
    df_combined = df_new

# Save updated data to CSV
df_combined.to_csv(CSV_FILE, index=False)

# Sorting by difficulty for better visualization
df_combined = df_combined.sort_values(by="Difficulty")

# Generate unique colors for each difficulty
unique_difficulties = sorted(df_combined["Difficulty"].unique())
colors = plt.cm.get_cmap("tab10", len(unique_difficulties))

# Create figure and side-by-side plots
fig, axes = plt.subplots(1, 2, figsize=(14, 5))

# Plot: Difficulty vs Elapsed Time
for i, difficulty in enumerate(unique_difficulties):
    subset = df_combined[df_combined["Difficulty"] == difficulty]
    axes[0].scatter(subset["Difficulty"], subset["Elapsed Time (s)"], color=colors(i), label=f"Difficulty {difficulty}")

axes[0].set_xlabel("Difficulty")
axes[0].set_ylabel("Elapsed Time (s)")
axes[0].set_title("Difficulty vs Elapsed Time")
axes[0].legend()
axes[0].grid(True, which="both", linestyle="--", linewidth=0.5)
axes[0].set_xticks(unique_difficulties)  # Ensure x-ticks are in one-interval steps

# Plot: Difficulty vs Operations
for i, difficulty in enumerate(unique_difficulties):
    subset = df_combined[df_combined["Difficulty"] == difficulty]
    axes[1].scatter(subset["Difficulty"], subset["Operations"], color=colors(i), label=f"Difficulty {difficulty}")

axes[1].set_xlabel("Difficulty")
axes[1].set_ylabel("Operations")
axes[1].set_title("Difficulty vs Operations")
axes[1].legend()
axes[1].grid(True, which="both", linestyle="--", linewidth=0.5)
axes[1].set_xticks(unique_difficulties)  # Ensure x-ticks are in one-interval steps

# Adjust layout and save figure
plt.tight_layout()
plt.savefig(PNG_FILE, dpi=300)
# plt.show()

print(f"Plot saved as {PNG_FILE}")
