import matplotlib.pyplot as plt
import numpy as np

columnNames = ["Standard", "Halton", "HaltonRandomDigit", "HaltonOwen", "BlueNoise"]
xticks = ["8 sample per pixel", "64 samples per pixel", "128 samples per pixel"]
data = [[0.039672,0.040891,0.039511,0.039486,0.038944],
        [0.020621,0.019722,0.021030,0.020824,0.020163],
        [0.016679,0.015688,0.015828,0.015793,0.016766]]

data = list(map(list, zip(*data)))

x = np.arange(3) * 3
width = 0.5
multiplier = 0

fig, ax = plt.subplots(layout='constrained', figsize=(14, 6))

for idx, measurement in enumerate(data):
    offset = width * multiplier
    rects = ax.bar(x + offset, measurement, width, label=columnNames[idx])
    ax.bar_label(rects, padding=5)
    multiplier += 1

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Mean value of error', fontsize=14)
ax.set_title('FLIP error on different samples per pixel (cornell box image)', fontweight='bold', fontsize=14)
ax.set_xticks(x + width * 2, xticks, fontsize=14)
ax.set_ylim(0, 0.045)
ax.legend(ncols=5)

plt.savefig('flip_cornell.png')
