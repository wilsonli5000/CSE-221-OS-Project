import numpy as np

FQ = pow(10, 9) * 2.6

fname = 'log_shut_remote.txt'
setup = []
shutdown = []
with open(fname) as f:
	for lines in f:
		if (lines.split(' ')[0] == 'setup='):
			setup.append(int(lines.split(' ')[-1]))
		if (lines.split(' ')[0] == 'shutdown'):
			shutdown.append(int(lines.split(' = ')[-1]))

setup_m = int(np.mean(setup))
setup_time = setup_m / FQ * pow(10, 3)
setup_std = np.std(setup) /FQ * pow(10, 3)

shutdown_m = int(np.mean(shutdown))
shutdown_time = shutdown_m / FQ * pow(10, 3)
shutdown_std = np.std(shutdown) / FQ * pow(10, 3)

print("setup: ", setup_m)
print("setup time: ", setup_time, "ms")
print("setup time std: ", setup_std)
print("shutdown: ", shutdown_m )
print("shutdown time: ", shutdown_time, "ms")
print("shutdown time std: ", shutdown_std)
