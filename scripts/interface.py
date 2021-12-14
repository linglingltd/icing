import threading
import time
import serial
import sched
import sys
import numpy as np
from time import sleep
from datetime import date, datetime, timedelta
from itertools import count
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
import os
import csv

csv.register_dialect('csv', delimiter='\t', quoting=csv.QUOTE_NONE)

# some constants
z0 = 50
e = 1

# some global variables
#			time		 capavg		  caps         maxTime
cap_vals = [[],[],[],0]
temp_vals = [np.array([]),np.array([])]
freqs = np.array([])
running = True
ani = 0
starttime = 0
fileprefix = datetime.now().strftime("data/%Y%m%d-")
fig = 0
ax = 0
serDmm = serial.Serial(timeout=1)
serLcr = serial.Serial(timeout=5)



def smoothie(coarse, grid=8):
	if (len(coarse) == 0):
		return np.array([])
	res = [coarse[0]]
	for i in range(1,len(coarse)):
		if i-grid < 0:
			s = 0
		else:
			s = i - grid
		res += [np.mean(coarse[s:i])]
	return np.array(res)


def s2pToCapSpec(fname):
	freq = np.array([])
	cs = []
	with open(fname, encoding="utf8") as f:
		reader = csv.reader(f, 'csv')
		contents = list(reader)

		s2p = np.array(contents[2:-1])
		s2p = np.float_(s2p)
		freq = s2p[:,0]
		s11 = s2p[:,1] + s2p[:,2] * 1j

		z11 = z0 * (e + s11) / (e - s11)

		c = - 1 / (2 * 3.1415 * s2p[:,0]*1000 * z11.imag)

		c *= 1e12
		freq = s2p[:,0]
		
		return freq, np.mean(c[335:]), c


class Multimeter(threading.Thread):
	def run(self,*args,**kwargs):
		global temp_vals, running, starttime, dmmStatus
		dmmStatus = 1
		now = datetime.now()
		while running:
			# Request value from DMM (using Keysight U1242B)
			serDmm.write(b'FETC?\r\n')
			serDmm.flush()
			dmmVal = str(serDmm.readline())
			lcrVal = ""
			newTemp = np.float_(0);
			if len(dmmVal) > 9:
				newTemp = np.float(dmmVal[2:-5])
			
			newTime = int(time.time() - starttime)
			
			# Add time and temp to the corresponding lists.
			temp_vals[1] = np.append(temp_vals[1], newTemp)
			temp_vals[0] = np.append(temp_vals[0], newTime)
			
			# Sleep for a second
			time.sleep(1.0 - ((time.time() - starttime) % 1.0))
		dmmStatus = 2


class Netzwerkanalyzer(threading.Thread):
	def run(self,*args,**kwargs):
		global cap_vals, freq, running, starttime, fileprefix, nwaStatus
		global freqs
		nwaStatus = 1
		exists = 0
		while running:
			if not os.path.exists("data/default.s2p"):
				# idle
				exists = 0
			elif exists == 0 and os.path.exists("data/default.s2p"):
				exists = 1
			elif os.path.exists("data/default.s2p"):
				newTime = int(time.time() - starttime)
				os.rename("data/default.s2p", fileprefix+str(newTime-1)+"default.s2p")
				freqs, cap, caps = s2pToCapSpec(fileprefix+str(newTime-1)+"default.s2p")
				cap_vals[3] = newTime-1
				cap_vals[2] += [caps]
				cap_vals[1] += [cap]
				cap_vals[0] += [newTime-1]
				#print(cap_vals)
				
			# Sleep for a second
			time.sleep(1.0 - ((time.time() - starttime) % 1.0))
		nwaStatus = 2


class Datenschaufel(threading.Thread):
	def run(self,*args,**kwargs):
		global temp_vals, cap_vals, running, starttime, datStatus
		datStatus = 1
		writtenTemp = 0
		writtenCap = 0
		now = datetime.now()
		f = open(str(now.strftime(fileprefix+"DATA.txt")), "w")
		while running:
			if(len(cap_vals) > 0 and cap_vals[3]-writtenCap > 1):
				while(writtenCap < len(cap_vals[2])-1):
					f.write(str(temp_vals[0][writtenTemp]) + '\t' + str(temp_vals[1][writtenTemp]) + '\t' + ("%0.3f" % cap_vals[1][writtenCap]))
					for el in cap_vals[2][writtenCap]:
						f.write('\t' + "%0.3f" % el)
					
					f.write('\n')
					writtenTemp += 1
					if(writtenTemp == cap_vals[0][writtenCap+1]):
						writtenCap += 1

				# flush, so it is written for sure.
				f.flush()
				print("Saved up to " + str(writtenTemp-1))
			
			# Sleep for 10 seconds
			time.sleep(10.0 - ((time.time() - starttime) % 10.0))
		f.close()
		datStatus = 2

dmmTask = Multimeter()
nwaTask = Netzwerkanalyzer()
datTask = Datenschaufel()

dmmStatus = 0
nwaStatus = 0
datStatus = 0

def animate(i):
	global cap_vals, temp_vals, anim, ax
	global freqs
	maxTemp = len(temp_vals[0]) if len(temp_vals[0]) < len(temp_vals[1]) else len(temp_vals[1])
	maxTemp -= 1
	if maxTemp < 2:
		return
	maxCap = len(cap_vals[0]) if len(cap_vals[0]) < len(cap_vals[1]) else len(cap_vals[1])
	# just for safety, if the last row is not completely written.
	maxCap -= 1
	plt.suptitle("Active Experiment(t = " + str(round(temp_vals[0][-1])) + " s, $T_{Sensor}$ = " + str(round(temp_vals[1][-1],2)) + " °C)")
	
	
	# plot NOCH FREI.
	ax[0][1].clear()
	ax[0][1].grid(color='lightgray', linestyle="--")


	# plot temperature over time
	ax[0][0].clear()
	ax[0][0].grid(color='lightgray', linestyle="--")
	if maxTemp > 900:
		ax[0][0].plot(temp_vals[0][maxTemp-900:maxTemp], temp_vals[1][maxTemp-900:maxTemp], "r")
	else:
		ax[0][0].plot(temp_vals[0][:maxTemp], temp_vals[1][:maxTemp], "r")
	ax[0][0].set_ylabel("PCB-Temperatur $t / °C$")



	if(len(cap_vals[1]) > 0):
		# plot capacitance over time
		ax[1][0].clear()
		ax[1][0].grid(color='lightgray', linestyle="--")
		if maxCap > 900:
			ax[1][0].plot(cap_vals[0][maxCap-900:maxCap], cap_vals[1][maxCap-900:maxCap], "r")
		else:
			ax[1][0].plot(cap_vals[0][:maxCap], cap_vals[1][:maxCap], "r")
		ax[1][0].set_yticks([x*2.5 for x in range(0,7)])
		ax[1][0].set_ylabel("Sensor-Kapazität $C_{Sensor} / pF$")
		ax[1][0].set_ylim()


		# plot capacitance over frequency
		ax[1][1].clear()
		ax[1][1].grid(color='lightgray', linestyle="--")
		ax[1][1].plot(freqs, smoothie(cap_vals[2][maxCap]), "r")
		ax[1][1].set_yticks([x*2.5 for x in range(0,7)])
		ax[1][1].set_xticks([x*100 for x in range(1,11)])
		ax[1][1].set_xlim(100,1000)
	
	ax[1][1].set_ylabel("Sensor-Kapazität $C_{Sensor} / pF$")
	ax[1][1].set_xlabel("Anregungsfrequenz $f / kHz$")


	# pack and ship. :)
	plt.tight_layout(pad=0.5)

	
	_cap = 0 if (len(cap_vals[1]) < 1) else cap_vals[1][maxCap]
	_temp = 0 if (len(temp_vals[1]) < 1) else temp_vals[1][maxTemp]
	_time = 0 if (len(temp_vals[0]) < 1) else temp_vals[0][maxTemp]
	print(_time, '\t', _temp, '\t', "%0.3f" % _cap)

	
	if running == False:
		ani.event_source.stop()


def main():
	global time_vals, cap_vals, temp_vals, starttime
	global serDmm, serLcr, f, fStr, t, t2, running, ani
	global fig, ax
	fig, ax = plt.subplots(2,2)
	fig.set_size_inches(8, 3.5+1)

	if(not os.path.exists("data/")):
		print("data-Directory missing. Creating...")
		os.mkdir("data")

	serDmm.baudrate = 9600
	serDmm.port = "COM9"
	serDmm.open()
	
	if not serDmm.is_open:
		print("Connection to DMM failed.")
		return
	
	# make sure the input buffer is empty.
	while serDmm.in_waiting > 0:
		serDmm.read()
	
	starttime = time.time()
	dmmTask.start()
	nwaTask.start()
	datTask.start()
	sleep(1)
	
	ani = FuncAnimation(plt.gcf(), animate, interval=1000)
	plt.show()
	print("Waiting for background threads to exit.")
	running = False
	
	dmm, nwa, dat = dmmStatus, nwaStatus, datStatus
	i = 0
	while(i < 15 and dmm+nwa+dat != 6):
		sleep(1)
		i += 1
		if(dmm != dmmStatus and dmmStatus == 2): 
			print("DMM stopped.")
			dmm = dmmStatus
		if(nwa != nwaStatus and nwaStatus == 2): 
			print("NWA stopped.")
			nwa = nwaStatus
		if(dat != datStatus and datStatus == 2): 
			print("DAT stopped.")
			dat = datStatus

	if(dmm+nwa+dat != 6):
		print("Some process fucked up. Attempting to commit suicide.")

if __name__ == "__main__":
    main()
