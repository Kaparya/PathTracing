import matplotlib.pyplot as plt


def ReadInformationForPixel(cur_index: int) -> None:
    global axs, build_dimensions, MAX_SAMPLE
    x, y = map(int, f.readline().split())
    axs[2 * cur_index, 2].set_title(f"row = {x}, col = {y}", fontweight="bold")
    f.readline()

    data = []
    for index in range(dimensions_number):
        data.append([])
    for row in range(MAX_SAMPLE):
        line_parsed = list(map(float, f.readline().split()))
        for index in range(0, len(line_parsed)):
            data[index].append(line_parsed[index])

    for index in range(0, len(build_dimensions)):
        axs[2 * cur_index, 2 * index + 1].set_visible(False)
        axs[2 * cur_index, 2 * index].set_box_aspect(1 / 3)
        axs[2 * cur_index + 1, 2 * index].set_box_aspect(1)
        axs[2 * cur_index + 1, 2 * index + 1].set_box_aspect(3 / 1)

        axs[2 * cur_index + 1, 2 * index].scatter(data[build_dimensions[index][0]], data[build_dimensions[index][1]],
                                                  s=1.5)
        axs[2 * cur_index + 1, 2 * index].set_xlim(0, 1)
        axs[2 * cur_index + 1, 2 * index].set_ylim(0, 1)
        axs[2 * cur_index + 1, 2 * index].set_box_aspect(1)
        axs[2 * cur_index + 1, 2 * index].xaxis.set_label_position('top')
        axs[2 * cur_index + 1, 2 * index].set_xlabel(f"{build_dimensions[index][0]} dim")
        axs[2 * cur_index + 1, 2 * index].yaxis.set_label_position('right')
        axs[2 * cur_index + 1, 2 * index].set_ylabel(f"{build_dimensions[index][1]} dim", rotation=270, labelpad=15)

        axs[2 * cur_index, 2 * index].hist(data[build_dimensions[index][0]], bins=50, range=(0, 1))
        axs[2 * cur_index, 2 * index].set_xlim(0, 1)

        axs[2 * cur_index + 1, 2 * index + 1].hist(data[build_dimensions[index][1]], orientation="horizontal", bins=50,
                                                   range=(0, 1))
        axs[2 * cur_index + 1, 2 * index + 1].set_ylim(0, 1)


# parameters
# name = "Standard"
# name = "Halton"
# name = "HaltonRandomDigit"
# name = "HaltonOwen"
name = "BlueNoise"
# MAX_SAMPLE = 8
# MAX_SAMPLE = 16
MAX_SAMPLE = 256
build_dimensions = [[0, 1], [3, 4], [9, 10]]

config_str = "Results/" + name + "_" + str(MAX_SAMPLE)
f = open("../TestRandom/" + config_str + ".txt", "r")

number_of_pixels, dimensions_number = map(int, f.readline().split())

# plot settings
fig, axs = plt.subplots(2 * number_of_pixels, len(build_dimensions) * 2, dpi=300, sharex="col", sharey="row",
                        gridspec_kw=dict(height_ratios=[1, 3] * number_of_pixels,
                                         width_ratios=([3, 1] * len(build_dimensions))),
                        figsize=(10 / 3 * len(build_dimensions), 3 * number_of_pixels))
fig.tight_layout(h_pad=1.5, w_pad=1)

for pixel in range(number_of_pixels):
    ReadInformationForPixel(pixel)

plt.savefig(config_str + ".png", bbox_inches='tight')
plt.show()

# def ReadInformationForPixel(cur_index: int) -> None:
#     global axs, number_of_points
#
#     pixel_coords = f.readline()
#
#     axs[cur_index // 4][cur_index % 4].set_title(pixel_coords)
#
#     x_coords = []
#     y_coords = []
#     colors_1 = []
#
#     for i in range(number_of_points):
#         x, y, z = map(float, f.readline().split())
#         x_coords.append(x)
#         y_coords.append(y)
#         colors_1.append(z)
#
#     # plot settings
#     for a, b, c in zip(x_coords, y_coords, colors_1):
#         axs[cur_index // 4][cur_index % 4].scatter(a, b, color=(1 - c, 0, c), s=2)
#     axs[cur_index // 4][cur_index % 4].set_xlim(0, 1)
#     axs[cur_index // 4][cur_index % 4].set_ylim(0, 1)
#
#
# # parameters
# name = "BlueNoise"
# number_of_pixels = 12
# number_of_points = 256
#
# config_str = "Results/" + name + "_" + str(number_of_pixels) + "_" + str(number_of_points)
# f = open("../TestRandom/" + config_str + ".txt", "r")
#
# fig, axs = plt.subplots(number_of_pixels // 4, number_of_pixels // 3, dpi=300,
#                         figsize=(number_of_pixels, number_of_pixels // 4 * 3))
#
# for pixel in range(number_of_pixels):
#     ReadInformationForPixel(pixel)
#
# plt.tight_layout()
# plt.savefig(config_str + ".png")
# plt.show()
