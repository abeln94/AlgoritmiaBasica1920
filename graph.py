import numpy as np
from matplotlib import pyplot as plt

data = np.loadtxt("output.csv")

Ns = data[:, 0]
times_our = data[:, 1]
times_std = data[:, 2]

pol = np.polyfit(Ns, times_our, 1)
Ev = lambda x: pol[1] + pol[0] * x

print("{}+x*{}".format(pol[1], pol[0]))
print("5000 ->", Ev(5000))

plt.plot(Ns, times_our)
plt.plot(Ns, times_std, color='g')
plt.plot(Ns, [Ev(x) for x in Ns], color='r')  # green: O(n)
plt.show()
