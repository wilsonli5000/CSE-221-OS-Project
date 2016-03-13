### Creater: Jingyuan Li

import matplotlib.pyplot as plt
import math

fname1 = "test1.txt"
fname2 = "test2.txt"
fname3 = "test3.txt"
fname4 = "test4.txt"
fname5 = "test5.txt"
fname6 = "test6.txt"

def readfile (fname):
	file = open(fname)
	lines = 0
	base = pow(2, 10)
	itera = []
	x = []

	while 1:
		l = file.readline()
		if lines == 53:
			break
		lines += 1
		x.append(float(l))
		itera.append(math.log(base * pow(1.6, lines) / 4) )
	return itera, x

itera1, x1 = readfile(fname1)
itera2, x2 = readfile(fname2)
itera3, x3 = readfile(fname3)
itera4, x4 = readfile(fname4)
itera5, x5 = readfile(fname5)
itera6, x6 = readfile(fname6)

plt.plot(itera1, x1)
plt.plot(itera2, x2)
plt.plot(itera3, x3)
plt.plot(itera4, x4)
plt.plot(itera5, x5)
plt.plot(itera6, x6)
plt.xlim(5, 30)
plt.title('Access Latency vs Memory Size')
plt.xlabel('Log2(Array size)')
plt.ylabel('Latency (cycles)')
plt.legend(['256B', '1KB', '8KB', '32KB', '256KB', '1MB'], loc='upper left')
plt.grid()
plt.show()
