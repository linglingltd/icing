import matplotlib.pyplot as plt
import numpy as np
import csv
import sys

csv.register_dialect('csv', delimiter='\t', quoting=csv.QUOTE_NONE)
if(len(sys.argv) < 3):
    print("Usage: ", sys.argv[0], " [EMPTY TABLE FILE] [ICED TABLE FILE]");
    quit()

cap = "??"
if(len(sys.argv) >= 4):
    cap = sys.argv[3]

with open(sys.argv[1], encoding="utf8") as f:
    reader = csv.reader(f, 'csv')
    contents = list(reader)
    zempty = np.array(contents)
    zempty = -np.float_(zempty)
    zempty = zempty/100/32


with open(sys.argv[2], encoding="utf8") as f:
    reader = csv.reader(f, 'csv')
    contents = list(reader)
    ziced = np.array(contents)
    ziced = -np.float_(ziced)
    ziced = ziced/100/32
	
# make it square
#z = np.array(contents)
#z = np.float_(z)
#mn = np.min(z)
#for i in range(len(contents[0])-len(contents)):
#  contents += [[mn] * len(contents[0])]

z = ziced - zempty
z = z - np.min(z)

x, y = np.meshgrid(range(0, z.shape[1]), range(z.shape[0]))

print(z)

# show hight map in 3d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
ax = fig.add_subplot(111, projection='3d')
ax.plot_surface(x*5, y*5, z)
ax.set_ylabel("Measurement n")
ax.set_xlabel("Position on X-Axis / mm")
ax.set_zlabel("Height / mm")
plt.title('Height-Measurement over X-Axis')
plt.show()

# show hight map in 2d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
plt.title('Height-Measurement over X-Axis ($C = ' + str(cap) + " nF$)")
p = plt.imshow(z, interpolation='none', extent=[55,100,45,10])
plt.ylabel("Position on Y-Axis / mm")
plt.xlabel("Position on X-Axis / mm")
cb = plt.colorbar(p)
cb.ax.set_ylabel('Height / mm', rotation=90)
plt.tight_layout(pad=0.5)
plt.savefig(sys.argv[2].replace(".txt", "-heat.png"), dpi=150)
plt.show()
