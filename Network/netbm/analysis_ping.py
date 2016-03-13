import numpy as np

FQ = pow(10, 9) * 2.6

fname = 'log_plocal.txt'
time = []
with open(fname) as f:
	for lines in f:
		if (lines[0:2] == '64'):
			time.append(float(lines.split(' ')[-2].split('=')[-1]))

speed = 56 * pow(10, -6) / np.mean(time) / pow(10, -3)

print("time: ", np.mean(time), "ms")
print("speed: ", speed, "Mbps" )
