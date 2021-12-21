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

time_vals = np.array([])
cap_vals = np.array([])
temp_vals = np.array([])
running = True
ani = 0

serDmm = serial.Serial(timeout=1)
serLcr = serial.Serial(timeout=5)

class BackgroundTask(threading.Thread):
	def run(self,*args,**kwargs):
		global time_vals, cap_vals, temp_vals, running
		now = datetime.now()
		f = open(str(now.strftime("%Y%m%d-%H%M%S_TEST.txt")), "w")
		starttime = time.time()
		while running:
			serDmm.write(b'FETC?\r\n')
			serDmm.flush()
			dmmVal = str(serDmm.readline())
			lcrVal = ""
			newTemp = np.float_(0);
			if len(dmmVal) > 9:
				newTemp = np.float(dmmVal[2:-5])
			
			newTime = time.time() - starttime
			
			temp_vals = np.append(temp_vals, newTemp)
			time_vals = np.append(time_vals, newTime)
			
			print(newTime, '\t', newTemp, '\t', lcrVal.replace('\n', '\t'))
			f.write(str(newTime) + '\t' + str(newTemp) + '\t' + lcrVal.replace('\n', '\t') + '\n')
			f.flush()
			time.sleep(1.0 - ((time.time() - starttime) % 1.0))
		f.close()

dmmTask = BackgroundTask()

def animate(i):
	global time_vals, cap_vals, temp_vals, anim
	plt.cla()
	plt.clf()
	max = int(np.min(np.float_(np.array([len(time_vals), len(temp_vals)])))-1)
	if max < 2:
		return
	plt.title("Active Experiment(t = " + str(round(time_vals[-1])) + " s, $T_{Sensor}$ = " + str(round(temp_vals[-1],2)) + " °C)")
	plt.grid(color='lightgray', linestyle="--")
	#plt.plot(time_vals[:max], cap_vals[:max], "c")
	#plt.ylabel("Measured capacitance $C_{Sensor} / pF$")
	#plt.xlabel("Time since start of experiment $t_{exp} / s$")
	#plt.yticks([x*0.625 for x in range(0, 5)])
	#plt.ylim(0, 2.5)
	#temperature = plt.twinx()
	plt.plot(time_vals[:max], temp_vals[:max], "r")
	plt.ylabel("PCB-Temperatur $t / °C$")
	plt.yticks([x*2.5 for x in range(-2, 3)])
	#plt.set_ylim(-5, 5)
	plt.tight_layout(pad=0.5)
	
	if running == False:
		ani.event_source.stop()

def main():
	global time_vals, cap_vals, temp_vals
	global serDmm, serLcr, f, fStr, t, t2, running, ani
	fig = plt.figure()
	fig.set_size_inches(8, 3.5+1)
		
	serDmm.baudrate = 9600
	serDmm.port = "COM13"
	serDmm.open()
	
	if not serDmm.is_open:
		print("Connection to DMM failed.")
		return
	
	# make sure the input buffer is empty.
	while serDmm.in_waiting > 0:
		serDmm.read()
		
	dmmTask.start()
	sleep(1)
	
	ani = FuncAnimation(plt.gcf(), animate, interval=500)
	plt.show()
	print("Should kill now!")
	running = False
	sleep(5)

if __name__ == "__main__":
    main()
