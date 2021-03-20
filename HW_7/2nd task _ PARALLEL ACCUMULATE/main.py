from matplotlib import pyplot as plt
plt.clf()

parsedStrings = [line.strip().split() for line in open("results.txt").readlines()]
x, y = zip(*parsedStrings)
x = [int(item) for item in x]
y = [float(item) for item in y]

plt.plot(x, y)
plt.title("Parallel accumulating of 100.000.000 doubles")
plt.xlabel("Number of threads")
plt.ylabel("Time in microseconds")
plt.grid(True)
plt.tight_layout()
plt.savefig("Parallel accumulating")