import matplotlib.pyplot as plt


name = "Sobol"
max_path = 8
max_bounce = 4

config_str = name + "_" + str(max_path) + "_" + str(max_bounce) + "_"

fig, axs = plt.subplots(1, 3, dpi=150)
fig.suptitle("Dimensions Test")
fig.tight_layout()

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

axs[0].scatter(coords_x, coords_y, s=3)
axs[0].set_title("ePixel (0/1 dim)")
axs[0].set_xlim(0, 1)
axs[0].set_xlabel("0 dim")
axs[0].set_ylim(0, 1)
axs[0].set_ylabel("1 dim")
axs[0].set_box_aspect(1)


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

axs[1].scatter(coords_x, coords_y, s=3)
axs[1].set_title("eLightPoint (3/4 dim)")
axs[1].set_xlim(0, 1)
axs[1].set_xlabel("3 dim")
axs[1].set_ylim(0, 1)
axs[1].set_ylabel("4 dim")
axs[1].set_box_aspect(1)


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

axs[2].scatter(coords_x, coords_y, s=3)
axs[2].set_title("eGetRay (9/10 dim)")
axs[2].set_xlim(0, 1)
axs[2].set_xlabel("9 dim")
axs[2].set_ylim(0, 1)
axs[2].set_ylabel("10 dim")
axs[2].set_box_aspect(1)


plt.savefig(name + "/" + str(max_path) + "_" + str(max_bounce), bbox_inches='tight')
plt.show()
