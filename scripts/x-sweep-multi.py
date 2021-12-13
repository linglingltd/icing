import matplotlib.pyplot as plt
import numpy as np
import csv
import math
import sys

csv.register_dialect('csv', delimiter='\t', quoting=csv.QUOTE_NONE)

datas = ["data/210826-01_XSweep-20deg.txt", "data/210826-02_XSweep-10deg.txt","data/210826-03_XSweep-0deg.txt", "data/210826-04_XSweep-neg10deg.txt", "data/210826-05_XSweep-neg20deg.txt"]
# show hight map in 3d

# show hight map in 3d
fig = plt.figure()
fig.set_size_inches(8, 3.5+1)
plt.title("Höhenvermessung der Grundplatte ($Y$ = 40mm, $n$ = 5)")
plt.ylabel("Gemessene Höhe $z$ / mm")
plt.xlabel("Position auf der X-Achse $x$ / mm")

for data in datas[::-1]:
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
	
	# Platte an den Enden ausrichten:
	#mfirst = m[0]
	#mlast = m[len(m)-1]
	#mdelta = (mlast - mfirst) / len(m)
	#for i in range(len(m)):
	#	m[i] -= mdelta*i

	print(m)
	print("Biggest deviation: ", np.max(s))
	print("Avg. deviation: ", np.mean(s))
	
	txt = data[22:-4]
	txt = txt.replace("deg", "°C")
	txt = txt.replace("neg", "-")

	plt.errorbar(x, m, s, linestyle='None', marker='.', label="$" + txt + "$ ($\sigma_{avg} = " + str(round(np.mean(s)*1000, 2)) + "µm$, $\sigma_{max} = " + str(round(np.max(s)*1000, 2)) + "µm$)")

plt.legend(loc="lower center")
plt.grid(linestyle="--", color="darkgray")
plt.tight_layout(pad=0.5)
plt.savefig(datas[0].replace(".txt", "-2dstd.png"), dpi=150)
plt.show()