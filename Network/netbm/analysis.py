import numpy as np

FQ = pow(10, 9) * 2.6

fname = 'log_remote.txt'
time = []
with open(fname) as f:
	for lines in f:
		if (lines[0:3] == 'RTT' and len(lines) <= 30):
			time.append(int(lines.split(' = ')[-1][:-2]))

cycle = int(np.mean(time))
time = cycle / FQ * pow(10, 3)
speed = 56 * pow(10, -6) / time / pow(10, -3)

print("cycle: ", cycle)
print("time: ", time, "ms")
print("speed: ", speed, "Mbps" )
