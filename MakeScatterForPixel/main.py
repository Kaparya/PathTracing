import matplotlib.pyplot as plt


def ReadInformationForPixel(cur_index: int) -> None:
    global axs, build_dimensions, max_path
    x, y = map(int, f.readline().split())
    axs[2 * cur_index, 2].set_title(f"row = {x}, col = {y}", fontweight="bold")
    f.readline()

    data = []
    for index in range(dimensions_number):
        data.append([])
    for row in range(max_path):
        line_parsed = list(map(float, f.readline().split()))
        for index in range(0, len(line_parsed)):
            data[index].append(line_parsed[index])

    for index in range(0, len(build_dimensions)):
        axs[2 * cur_index, 2 * index + 1].set_visible(False)
        axs[2 * cur_index, 2 * index].set_box_aspect(1 / 3)
        axs[2 * cur_index + 1, 2 * index].set_box_aspect(1)
        axs[2 * cur_index + 1, 2 * index + 1].set_box_aspect(3 / 1)

        axs[2 * cur_index + 1, 2 * index].scatter(data[build_dimensions[index][0]], data[build_dimensions[index][1]], s=point_size)
        axs[2 * cur_index + 1, 2 * index].set_xlim(0, 1)
        axs[2 * cur_index + 1, 2 * index].set_ylim(0, 1)
        axs[2 * cur_index + 1, 2 * index].set_box_aspect(1)
        axs[2 * cur_index + 1, 2 * index].xaxis.set_label_position('top')
        axs[2 * cur_index + 1, 2 * index].set_xlabel(f"{build_dimensions[index][0]} dim")
        axs[2 * cur_index + 1, 2 * index].yaxis.set_label_position('right')
        axs[2 * cur_index + 1, 2 * index].set_ylabel(f"{build_dimensions[index][1]} dim", rotation=270, labelpad=15)

        axs[2 * cur_index, 2 * index].hist(data[build_dimensions[index][0]], bins=50, range=(0, 1))
        axs[2 * cur_index, 2 * index].set_xlim(0, 1)

        axs[2 * cur_index + 1, 2 * index + 1].hist(data[build_dimensions[index][1]], orientation="horizontal", bins=50, range=(0, 1))
        axs[2 * cur_index + 1, 2 * index + 1].set_ylim(0, 1)




# parameters
name = "Halton"
max_path = 8
dimensions_number = 12
build_dimensions = [[0, 1], [3, 4], [9, 10]]



config_str = "Results/" + name + "_" + str(max_path)
f = open("../TestRandom/" + config_str + ".txt", "r")

point_size = float(1)
if max_path == 256:
    point_size = 0.5

number_of_pixels = int(f.readline())

# plot settings
fig, axs = plt.subplots(2 * number_of_pixels, len(build_dimensions) * 2, dpi=300, sharex="col", sharey="row",
                        gridspec_kw=dict(height_ratios=[1, 3] * number_of_pixels,
                                         width_ratios=([3, 1] * len(build_dimensions))), figsize=(5, 15))
fig.tight_layout(h_pad=1.5, w_pad=1)

for pixel in range(number_of_pixels):
    ReadInformationForPixel(pixel)

plt.savefig(config_str + ".png", bbox_inches='tight')
plt.show()
