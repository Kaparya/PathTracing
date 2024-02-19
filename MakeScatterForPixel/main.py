import matplotlib.pyplot as plt

# parameters
name = "Sobol"
max_path = 8

config_str = name + "_" + str(max_path)
f = open("../TestRandom/" + config_str + ".txt", "r")
axes_names = list(map(int, f.readline().split()))
data = []
for index in range(len(axes_names)):
    data.append([])
for line in f:
    line_parsed = list(map(float, line.split()))
    for index in range(0, len(line_parsed)):
        data[index].append(line_parsed[index])


build_dimensions = [0, 1, 3, 4, 9, 10]


# plot settings
fig, axs = plt.subplots(2, len(build_dimensions), dpi=300, sharex="col", sharey="row",
                        gridspec_kw=dict(height_ratios=[1, 3],
                                         width_ratios=[3, 1] * (len(build_dimensions) // 2)))
fig.suptitle("Dimensions Test")
fig.tight_layout()

for index in range(0, len(build_dimensions), 2):
    axs[0, index + 1].set_visible(False)
    axs[0, index].set_box_aspect(1 / 3)
    axs[1, index].set_box_aspect(1)
    axs[1, index + 1].set_box_aspect(3 / 1)

    axs[1, index].scatter(data[index], data[index + 1], s=2)
    axs[1, index].set_title(f"{build_dimensions[index]}/{build_dimensions[index + 1]} dim")
    axs[1, index].set_xlim(0, 1)
    axs[1, index].set_ylim(0, 1)
    axs[1, index].set_box_aspect(1)

    axs[0, index].hist(data[index], bins=50, range=(0, 1))
    axs[0, index].set_xlabel(f"{build_dimensions[index]} dim")
    axs[0, index].set_xlim(0, 1)

    axs[1, index + 1].hist(data[index + 1], orientation="horizontal", bins=50, range=(0, 1))
    axs[1, index + 1].set_ylabel(f"{build_dimensions[index + 1]} dim")
    axs[1, index + 1].set_ylim(0, 1)

plt.savefig(config_str + ".png", bbox_inches='tight')
plt.show()
