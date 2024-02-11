import matplotlib.pyplot as plt

# parameters
name = "Sobol"
max_path = 256
max_bounce = 8
scrambling_type = "OwenFinal"

config_str = name + "_" + str(max_path) + "_" + str(max_bounce) + "_"

if scrambling_type != "":
    config_str = scrambling_type + "/" + config_str



# plot settings
fig, axs = plt.subplots(2, 6, dpi=150, sharex="col", sharey="row",
                        gridspec_kw=dict(height_ratios=[1, 3],
                                         width_ratios=[3, 1, 3, 1, 3, 1]))
fig.suptitle("Dimensions Test")
fig.tight_layout()

axs[0, 1].set_visible(False)
axs[0, 0].set_box_aspect(1 / 3)
axs[1, 0].set_box_aspect(1)
axs[1, 1].set_box_aspect(3 / 1)
axs[0, 3].set_visible(False)
axs[0, 2].set_box_aspect(1 / 3)
axs[1, 2].set_box_aspect(1)
axs[1, 3].set_box_aspect(3 / 1)
axs[0, 5].set_visible(False)
axs[0, 4].set_box_aspect(1 / 3)
axs[1, 4].set_box_aspect(1)
axs[1, 5].set_box_aspect(3 / 1)



# ePixel scatter + histograms
coords_x = []
coords_y = []
data_x = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + config_str + "ePixelX_0.txt", "r")
for line in data_x:
    help = line.split()
    coords_x.append(float(help[0]))

data_y = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + config_str + "ePixelY_1.txt", "r")
for line in data_y:
    help = line.split()
    coords_y.append(float(help[0]))

axs[1, 0].scatter(coords_x, coords_y, s=3)
axs[1, 0].set_title("ePixel (0/1 dim)")
axs[1, 0].set_xlim(0, 1)
axs[1, 0].set_ylim(0, 1)
axs[1, 0].set_box_aspect(1)

axs[0, 0].hist(coords_x, bins=10, range=(0, 1))
axs[0, 0].set_xlabel("0 dim")
axs[1, 1].hist(coords_y, orientation="horizontal", bins=10, range=(0, 1))
axs[1, 1].set_ylabel("1 dim")



# eLightPoint scatter + histograms
coords_x.clear()
coords_y.clear()
data_x = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + config_str + "eLightPointX_3.txt", "r")
for line in data_x:
    help = line.split()
    coords_x.append(float(help[0]))

data_y = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + config_str + "eLightPointY_4.txt", "r")
for line in data_y:
    help = line.split()
    coords_y.append(float(help[0]))

axs[1, 2].scatter(coords_x, coords_y, s=3)
axs[1, 2].set_title("eLightPoint (3/4 dim)")
axs[1, 2].set_xlim(0, 1)
axs[1, 2].set_ylim(0, 1)
axs[1, 2].set_box_aspect(1)

axs[0, 2].hist(coords_x, bins=10, range=(0, 1))
axs[0, 2].set_xlabel("3 dim")
axs[1, 3].hist(coords_y, orientation="horizontal", bins=10, range=(0, 1))
axs[1, 3].set_ylabel("4 dim")



# eGetRay scatter + histograms
coords_x.clear()
coords_y.clear()
data_x = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + config_str + "eGetRayX_9.txt", "r")
for line in data_x:
    help = line.split()
    coords_x.append(float(help[0]))

data_y = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + config_str + "eGetRayY_10.txt", "r")
for line in data_y:
    help = line.split()
    coords_y.append(float(help[0]))

axs[1, 4].scatter(coords_x, coords_y, s=3)
axs[1, 4].set_title("eGetRay (9/10 dim)")
axs[1, 4].set_xlim(0, 1)
axs[1, 4].set_ylim(0, 1)
axs[1, 4].set_box_aspect(1)

axs[0, 4].hist(coords_x, bins=10, range=(0, 1))
axs[0, 4].set_xlabel("9 dim")
axs[1, 5].hist(coords_y, orientation="horizontal", bins=10, range=(0, 1))
axs[1, 5].set_ylabel("10 dim")

config_str = name + "/" + str(max_path) + "_" + str(max_bounce) + ".png"

if scrambling_type != "":
    config_str = scrambling_type + "/" + config_str

plt.savefig(config_str, bbox_inches='tight')
plt.show()
