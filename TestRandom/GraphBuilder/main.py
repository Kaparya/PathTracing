import numpy as np
import matplotlib.pyplot as plt
import jax
import jax.numpy as jnp
import random


def plotFFT(pattern):
    fig, axs = plt.subplots(1, 3, figsize=(15, 10))
    axs[0].imshow(pattern)
    axs[1].set_title('Periodogram')
    im = axs[1].imshow(np.abs(np.fft.fftshift(np.fft.fft2(pattern - np.mean(pattern)) / pattern.shape[0])), vmin=0.0,
                       vmax=0.85)
    fig.colorbar(im, ax=axs[1], fraction=0.046, pad=0.04)
    axs[2].set_title('Log periodogram')
    eps = 1e-12
    im = axs[2].imshow(np.log10(abs(np.fft.fftshift(np.fft.fft2(pattern - np.mean(pattern)) / pattern.shape[0])) + eps),
                       vmin=-4, vmax=1)
    fig.colorbar(im, ax=axs[2], fraction=0.046, pad=0.04)
    plt.savefig('../Results/BlueNoiseTexture.png', bbox_inches='tight')
    # plt.show()


data = np.zeros((128, 128), dtype=np.float32)
input_file = open("../GraphBuilder/data.txt", "r")
line_index = 0
for line in input_file:

    cur_values = list(map(float, line.split()))
    for index in range(len(cur_values)):
        data[line_index, index] = cur_values[index]

    line_index += 1

input_file.close()

plotFFT(data)
