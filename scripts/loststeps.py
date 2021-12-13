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

n = (len(z)-1) * len(z[0])
diff = 0
lostmap = np.array([])
for i in range(len(z)-1):
      lostmap = np.append(lostmap, (z[i+1] - z[i]))
      diff += np.sum((z[i+1] - z[i]))
      print(diff)

z = z/100/32
lostmap = lostmap/100/32

print("Lost steps (avg.): ", int(diff/n))

maxdev = np.max(lostmap)
if maxdev < -np.min(lostmap):
  maxdev = -np.min(lostmap)

fig = plt.figure()
fig.set_size_inches(8, 2.5+1)
plt.title("Reproduzierbarkeitsmessung ($|\delta_{max}| = " + str(round(maxdev*1000, 2)) + "µm, \delta_{avg} = " + str(round(diff/n/100/32*1000, 2)) + "µm$)")
plt.ylabel("Abweichung zur verherigen Messung / mm")
plt.xlabel("Messung")
plt.plot([x for x in range(len(lostmap))], lostmap, linestyle="none", marker=".")
plt.grid(linestyle="--", color="darkgray")
plt.tight_layout(pad=0.5)
plt.savefig(data.replace(".txt", "-lost.png"), dpi=150)
plt.show();