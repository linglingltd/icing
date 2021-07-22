import matplotlib.pyplot as plt
import numpy as np
import csv
import math
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

z = np.array(contents)
z = -np.float_(z)
z = z/100/32  # umrechnen in mm

m = np.array([])
s = np.array([])
x = np.array([])
for i in range(len(z[0])):
  m = np.append(m, np.mean(z[:,i]))
  s = np.append(s, np.std(z[:,i]))
  x = np.append(x, i*2)

m = m - np.min(m);

for i in range(len(x)):
	print(x[i], ": (", round(m[i], 2), " +- ", round(s[i], 4), ") mm");
#for x in len(z)
#m = 

print(m)
print("Biggest deviation: ", np.max(s))
print("Avg. deviation: ", np.mean(s))

# show hight map in 3d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
plt.title("Höhenvermessung ohne Sensorplatine (n = " + str(len(z)) + ", $\sigma_{avg}$ = " + str(round(np.mean(s), 4)) + "mm, $\sigma_{max}$ = " + str(round(np.max(s), 4)) + "mm)")
plt.ylabel("Gemessene Höhe z / mm")
plt.xlabel("Position auf der X-Achse x / mm")
plt.errorbar(x, m, s, linestyle='None', marker='.')
plt.grid(linestyle="--", color="darkgray")
plt.tight_layout(pad=0.5)
plt.show()
plt.savefig(data.replace(".txt", "-2dstd.png"), dpi=150)