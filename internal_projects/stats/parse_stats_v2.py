import re
import sys
import matplotlib.pyplot as plt
import pandas as pd
import os
import numpy as np

# Define CSV file path
CSV_FILE = "parsed_data.csv"
PNG_FILE = "performance_study.png"

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

id = 1;
for i in range(len(difficulty_matches)):
    elapsed_time = float(elapsed_time_matches[i])
    operations = int(operations_matches[i])

    data.append([id, elapsed_time, operations])
    id += 1

# Convert to DataFrame

df  = pd.DataFrame(data, columns=["ID", "Elapsed Time", "Operations"])

# Compute statistics
elapsed_mean = df["Elapsed Time"].mean()
elapsed_std = df["Elapsed Time"].std()
operations_mean = df["Operations"].mean()
operations_std = df["Operations"].std()

# Plot Elapsed Time Scatter Plot
plt.figure(figsize=(8, 16))

plt.subplot(2, 1, 1)
plt.scatter(df["ID"], df["Elapsed Time"], color='blue', label="Elapsed Time")
plt.axhline(elapsed_mean, color='red', linestyle='dashed', label=f"Mean: {elapsed_mean:.4f}")
plt.axhline(elapsed_mean + elapsed_std, color='green', linestyle='dotted', label=f"Std Dev: {elapsed_std:.4f}")
plt.axhline(elapsed_mean - elapsed_std, color='green', linestyle='dotted')
plt.xlabel("ID")
plt.ylabel("Elapsed Time (seconds)")
plt.title("Elapsed Time per Entry")
plt.legend()

# Plot Operations Scatter Plot
plt.subplot(2, 1, 2)
plt.scatter(df["ID"], df["Operations"], color='purple', label="Operations")
plt.axhline(operations_mean, color='red', linestyle='dashed', label=f"Mean: {operations_mean:.0f}")
plt.axhline(operations_mean + operations_std, color='green', linestyle='dotted', label=f"Std Dev: {operations_std:.0f}")
plt.axhline(operations_mean - operations_std, color='green', linestyle='dotted')
plt.xlabel("ID")
plt.ylabel("Operations")
plt.title("Operations per Entry")
plt.legend()

plt.tight_layout()
# plt.show()

plt.tight_layout()
plt.savefig(PNG_FILE, dpi=300)