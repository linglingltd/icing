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

z = z - np.min(z)

x, y = np.meshgrid(range(0, z.shape[1]), range(z.shape[0]))

print(z)

# show hight map in 3d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x*2, y, z)
ax.set_ylabel("Measurement n")
ax.set_xlabel("Position on X-Axis / mm")
ax.set_zlabel("Height / mm")
plt.title('Height-Measurement over X-Axis')
plt.show()

# show hight map in 2d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
plt.title('Height-Measurement over X-Axis')
p = plt.imshow(z, interpolation='none', extent=[0,122,10,0])
plt.ylabel("Measurement n")
plt.xlabel("Position on X-Axis / mm")
cb = plt.colorbar(p)
cb.ax.set_ylabel('Height / mm', rotation=90)
plt.tight_layout(pad=0.5)
plt.savefig(data.replace(".txt", "-heat.png"), dpi=150)
plt.show()
