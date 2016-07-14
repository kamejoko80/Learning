import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D

x = np.arange(0, 5, 0.1);
y = np.sin(x)
plt.plot(x, y)
plt.show()

fig = plt.figure()
ax = Axes3D(fig)
ax.scatter(x, x, x, c = 'r')
plt.show()
