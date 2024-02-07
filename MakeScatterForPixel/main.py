
import matplotlib.pyplot as plt

coords_x = []
coords_y = []

gen = 2

if gen == 0:
    x_file_name = "ePixelX_0.txt"
    y_file_name = "ePixelY_1.txt"
elif gen == 1:
    x_file_name = "eLightPointX_3.txt"
    y_file_name = "eLightPointY_4.txt"
else:
    x_file_name = "eGetRayX_9.txt"
    y_file_name = "eGetRayY_10.txt"

data_x = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + x_file_name, "r")
for line in data_x:
    help = line.split()
    coords_x.append(float(help[0]))

data_y = open("/Users/kaparya/HSE/PathTracing/WorkingProject/DimensionsTest/" + y_file_name, "r")
for line in data_y:
    help = line.split()
    coords_y.append(float(help[0]))

plt.scatter(coords_x, coords_y, s=3)
plt.xlim(0, 1)
plt.ylim(0, 1)

plt.show()