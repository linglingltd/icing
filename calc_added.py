import matplotlib.pyplot as plt
import numpy as np
import csv
import math
import sys

csv.register_dialect('csv', delimiter='\t', quoting=csv.QUOTE_NONE)
data = "x-sweep.txt"

with open(data, encoding="utf8") as f:
    reader = csv.reader(f, 'csv')
    contents = list(reader)

z = np.array(contents)
z = -np.float_(z)
z = z/100/32  # umrechnen in mm

m = np.array([])
s = np.array([])
mp = np.array([])
sp = np.array([])
x = np.array([])
for i in range(len(z[0])):
  m = np.append(m, np.mean(z[:-6,i]))
  s = np.append(s, np.std(z[:-6,i]))
  mp = np.append(mp, np.mean(z[-6:,i]))
  sp = np.append(sp, np.std(z[-6:,i]))
  x = np.append(x, i*2)

m = m - np.min(m);


for i in range(len(x)):
	print(x[i], ": (", round(m[i], 2), " +- ", round(s[i], 4), ") mm");
#for x in len(z)
#m = 

print(z)

# show hight map in 3d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
plt.title("Höhenvermessung ohne Sensorplatine (n = " + str(len(z)) + ")")
plt.ylabel("Gemessene Höhe z / mm")
plt.xlabel("Position auf der X-Achse x / mm")
#plt.errorbar(x, m, s, linestyle='None', marker='.', label='Kupfer blank')
#plt.errorbar(x, mp, sp, linestyle='None', marker='.', label='Kupfer+PCB')
plt.errorbar(x, mp-m, sp*3, linestyle='None', marker='.', label='Kupfer weggerechnet')
plt.grid(linestyle="--", color="darkgray")

plt.yticks([x/10 for x in range(0, 25)])
plt.tight_layout(pad=0.5)
plt.legend(loc="upper left")
plt.show()