import matplotlib.pyplot as plt
import numpy as np

columnNames = ["Standard", "Halton", "HaltonRandomDigit", "HaltonOwen", "BlueNoise"]
xticks = ["8 sample per pixel", "64 samples per pixel", "128 samples per pixel"]
data = [[105.019266, 100.016115, 99.004761, 99.548095, 105.561819],
        [23.114900, 19.210052, 20.379466, 20.009835, 23.676063],
        [13.270534, 11.040521, 10.860608, 10.716254, 13.700408]]

data = list(map(list, zip(*data)))

x = np.arange(3) * 3
width = 0.5
multiplier = 0

fig, ax = plt.subplots(layout='constrained', figsize=(12, 5))

for idx, measurement in enumerate(data):
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=columnNames[idx])
    ax.bar_label(rects, padding=5)
    multiplier += 1

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('MSE error', fontsize=14)
ax.set_title('MSE error on different samples per pixel (cornell box image)', fontweight='bold', fontsize=14)
ax.set_xticks(x + width * 2, xticks, fontsize=14)
ax.set_ylim(0, 120)
ax.legend(ncols=5)

plt.savefig('mse_cornell.png')
