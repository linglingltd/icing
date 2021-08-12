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

if(len(sys.argv) < 3):
    print("No n for increased height given. Defaulting to 1.");
    number_of_pcb_measures = 1
else:
    number_of_pcb_measures = int(sys.argv[2]);

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
  m = np.append(m, np.mean(z[:-number_of_pcb_measures,i]))
  s = np.append(s, np.std(z[:-number_of_pcb_measures,i]))
  mp = np.append(mp, np.mean(z[-number_of_pcb_measures:,i]))
  sp = np.append(sp, np.std(z[-number_of_pcb_measures:,i]))
  x = np.append(x, i*2)

#m = m - np.min(m);
#mp = mp - np.min(mp);


for i in range(len(x)):
	print(x[i], ": (", round(m[i], 2), " +- ", round(s[i], 4), ") mm");

print(z)

dif = mp-m

#normalize
#dif = dif - np.min(dif)

dif2 = np.copy(dif)

# Maybe try some filtering of the values?

# show hight map in 3d
fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
plt.title("Differenzielle Höhenvermessung ($n = " + str(len(z)-number_of_pcb_measures) + "_{Basis}/" + str(number_of_pcb_measures) + "_{Erhöhung}$)")
plt.ylabel("Gemessene Höhendifferenz $\delta_z$ / mm")
plt.xlabel("Position auf der X-Achse x / mm")
#plt.errorbar(x, m, s, linestyle='None', marker='.', label='Kupfer blank')
#plt.errorbar(x, mp, sp, linestyle='None', marker='.', label='Kupfer+PCB')
plt.errorbar(x, dif, sp, linestyle='None', marker='.', label='Unfiltered')
plt.grid(linestyle="--", color="darkgray")

plt.yticks([x/20 for x in range(0, 11)])
plt.tight_layout(pad=0.5)
plt.legend(loc="upper left")
plt.savefig(data.replace(".txt", "-delta.png"), dpi=150)
plt.show()