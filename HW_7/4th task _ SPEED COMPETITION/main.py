from matplotlib import pyplot as plt
plt.clf()

parsedStrings = [line.strip().split() for line in open("for_each test.txt").readlines()]
x, y1, y2 = zip(*parsedStrings)
x = [int(item) for item in x]
y1 = [float(item) for item in y1]
y2 = [float(item) for item in y2]

plt.plot(x, y1, label="parallel")
plt.plot(x, y2, label="sequential")
plt.title("std::for_each sequential and parallel comparison")
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.grid(True)
plt.tight_layout()
plt.savefig("for_each sequential and parallel comparison")
plt.clf()

##################

parsedStrings = [line.strip().split() for line in open("partial_sum VS inclusive_scan.txt").readlines()]
x, y1, y2 = zip(*parsedStrings)
x = [int(item) for item in x]
y1 = [float(item) for item in y1]
y2 = [float(item) for item in y2]

plt.plot(x, y1, label="std::partial_sum")
plt.plot(x, y2, label="std::inclusive_scan")
plt.title("std::partial_sum and std::inclusive_scan comparison")
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.grid(True)
plt.tight_layout()
plt.savefig("partial_sum and inclusive_scan comparison")
plt.clf()

##################

parsedStrings = [line.strip().split() for line in open("transform_reduce VS inner_product.txt").readlines()]
x, y1, y2 = zip(*parsedStrings)
x = [int(item) for item in x]
y1 = [float(item) for item in y1]
y2 = [float(item) for item in y2]

plt.plot(x, y1, label="std::transform_reduce")
plt.plot(x, y2, label="std::inner_product")
plt.title("std::transform_reduce and std::inner_product comparison")
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.grid(True)
plt.tight_layout()
plt.savefig("transform_reduce and inner_product comparison")
plt.clf()