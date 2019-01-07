import numpy as np
import matplotlib.pyplot as plt

data = np.loadtxt('output.txt', skiprows=0)
N = len(data)
x = range(N)
width = 5
plt.bar(x, data, width, color="black")
fig = plt.gcf()
plt.show()
