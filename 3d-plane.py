import matplotlib.pyplot as plt
import numpy as np
import csv
import sys

csv.register_dialect('csv', delimiter='\t', quoting=csv.QUOTE_NONE)
if(len(sys.argv) < 2):
    print("NONONONONONONO! Chose file!!");
    data = "x-sweep.txt"
else:
    data = sys.argv[1];

with open(data, encoding="utf8") as f:
    reader = csv.reader(f, 'csv')
    contents = list(reader)

# make it square
#z = np.array(contents)
#z = np.float_(z)
#mn = np.min(z)
#for i in range(len(contents[0])-len(contents)):
#  contents += [[mn] * len(contents[0])]
z = np.array(contents)
z = -np.float_(z)
z = z/100/32

	
x, y = np.meshgrid(range(0, 2*z.shape[1], 2), range(z.shape[0]))

print(z)

# show hight map in 3d
fig = plt.figure()
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x, y, z)
ax.set_ylabel("Measurement n")
ax.set_xlabel("Position on X-Axis / mm")
ax.set_zlabel("Height / mm")
plt.title('Height-Measurement over X-Axis')
plt.show()

# show hight map in 2d
plt.figure()
plt.title('Height-Measurement over X-Axis')
p = plt.imshow(z)
plt.ylabel("Measurement n")
plt.xlabel("Position on X-Axis / mm")
cb = plt.colorbar(p)
cb.ax.set_ylabel('Height / mm', rotation=90)
plt.savefig(data.replace(".txt", "-heat.png"), dpi=150)
plt.show()
