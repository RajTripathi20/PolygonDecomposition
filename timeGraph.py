import matplotlib.pyplot as plt
import numpy as np
import fileinput

xpoints=[]
y2points=[]
y3points=[]
y4points=[]
algorithmPoints=[]

for line in fileinput.input(files ='Outputs/TvsN.txt'):
    temp = line.split()

    xpoints.append(int(temp[0]))
    y2points.append(1*int(temp[0])**2)
    y3points.append(1*int(temp[0])**3)
    y4points.append(1*int(temp[0])**4)
    algorithmPoints.append(int(temp[1]))

plt.plot(xpoints, algorithmPoints,label="ALgorithm")
plt.plot(xpoints, y2points,label="O(n^2)")
plt.plot(xpoints, y3points,label="O(n^3)")
# plt.plot(xpoints, y4points,label="O(n^4)")
plt.xlabel("Number of vertices")
plt.ylabel("Time(microseconds)")
plt.legend()
plt.show()