from matplotlib import pyplot as plt

# for line in open("testVector.txt", 'r').readlines():
#     (xi, yi) = tuple([int(element) for element in line.strip().split()])
#     x.append(xi)
#     y.append(yi)
# x = []
# y = []
# for xi, yi in [tuple([int(el) for el in line.split()]) for line in open("testVector.txt", 'r').read().strip().split('\n')]:
#     x.append(xi)
#     y.append(yi)

xVec, yVec = zip(*[tuple(map(int, line.strip().split())) for line in open("testVector.txt").readlines()])
plt.plot(xVec, yVec)
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.savefig("Vector")

plt.clf()

xSet, ySet = zip(*[tuple(map(int, line.strip().split())) for line in open("testSet.txt").readlines()])
plt.plot(xSet, ySet)
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.savefig("Set")

plt.clf()

plt.plot(xVec, yVec)
plt.plot(xSet, ySet)
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.savefig("Vector & Set")

########################################################################################################
plt.clf()

xVec, yVec = zip(*[tuple(map(int, line.strip().split())) for line in open("testVectorWithShuffleRelease5M.txt").readlines()])
plt.plot(xVec, yVec)
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.savefig("VectorWithShuffle5MRelease")

plt.clf()

xSet, ySet = zip(*[tuple(map(int, line.strip().split())) for line in open("testSetWithShuffleRelease5M.txt").readlines()])
plt.plot(xSet, ySet)
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.savefig("SetWithShuffle5MRelease")

plt.clf()

plt.plot(xVec, yVec)
plt.plot(xSet, ySet)
plt.xlabel("Number of elements")
plt.ylabel("Time in microseconds")
plt.savefig("Vector & Set _ WithShuffle5M _ Release")