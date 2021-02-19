from matplotlib import pyplot as plt
plt.clf()

xVec, yVec = zip(*[tuple(map(int, line.strip().split())) for line in open("numberOfCollisions_v2.txt").readlines()])
plt.plot(xVec, yVec)
plt.xlabel("Number of hashed objects")
plt.ylabel("Number of Collisions")
plt.savefig("Collisions_2")