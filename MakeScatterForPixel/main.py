
import matplotlib.pyplot as plt

coords_x = []
coords_y = []

data = open("/Users/kaparya/HSE/PathTracing/WorkingProject/RaysOriginsForPixel_halton_1000.txt", "r")
for line in data:
    help = line.split()
    coords_x.append(float(help[0]))
    coords_y.append(float(help[1]))


plt.scatter(coords_x, coords_y, s=1)
plt.xlim(0, 1)
plt.ylim(0, 1)

plt.show()
