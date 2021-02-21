from matplotlib import pyplot as plt
plt.clf()

xC, yC1 = zip(*[tuple(map(int, line.strip().split())) for line in open("hashForDoubleV1_Collisions.txt").readlines()])
xC, yC2 = zip(*[tuple(map(int, line.strip().split())) for line in open("hashForDoubleV2_Collisions.txt").readlines()])


plt.plot(xC, yC1, label="Hash function V1")
plt.plot(xC, yC2, label="Hash function V2")
plt.xlabel("Number of hashed objects")
plt.ylabel("Number of Collisions")
plt.legend()
plt.grid(True)
plt.savefig("Collisions")
plt.clf()

xD, yD1 = zip(*[tuple(map(int, line.strip().split())) for line in open("hashForDoubleV1_Duplications.txt").readlines()])

plt.plot(xD, yD1)
plt.xlabel("Codes")
plt.ylabel("Number of Duplications")
plt.grid(True)
plt.savefig("Duplications in HF V1")
plt.clf()

xD, yD2 = zip(*[tuple(map(int, line.strip().split())) for line in open("hashForDoubleV2_Duplications.txt").readlines()])

plt.plot(xD, yD2)
plt.xlabel("Codes")
plt.ylabel("Number of Duplications")
plt.grid(True)
plt.savefig("Duplications in HF V2")
plt.clf()