import matplotlib.pyplot as plt
import numpy as np

columnNames = ["Standard", "Halton", "HaltonRandomDigit", "HaltonOwen", "BlueNoise"]
xticks = ["1 sample per pixel", "2 samples per pixel", "8 samples per pixel"]
data = [[6.635833,6.754809,6.754149,6.765122,6.760747],
        [3.554158,3.642643,3.347339,3.307821,3.622904],
        [1.099262,0.550382,0.561940,0.568160,1.104661]]

data = list(map(list, zip(*data)))

x = np.arange(3) * 3
width = 0.5
multiplier = 0

fig, ax = plt.subplots(layout='constrained', figsize=(15,5))

for idx, measurement in enumerate(data):
        offset = width * multiplier
        rects = ax.bar(x + offset, measurement, width, label=columnNames[idx])
        ax.bar_label(rects, padding=5)
        multiplier += 1

# Add some text for labels, title and custom x-axis tick labels, etc.
ax.set_ylabel('Mean error')
ax.set_title('MSE error on different samples per pixel (wolf image)', fontweight='bold')
ax.set_xticks(x + width * 2, xticks)
ax.set_ylim(0, 8)
ax.legend(ncols=5)

plt.savefig('mse_wolf.png')
