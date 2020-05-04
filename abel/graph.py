import numpy as np
from matplotlib import pyplot as plt

data = np.loadtxt("output.csv")

digits = data[:, 0]
Ns = data[:, 1]
times_our = data[:, 2]
times_std = data[:, 3]

for digit in range(1, 9 + 1):
    indexes = digits == digit
    plt.subplot(3, 3, digit)
    plt.title('{} digit{}'.format(digit, '' if digit == 1 else 's'))

    if not any(indexes):
        continue

    pol = np.polyfit(Ns[indexes], times_our[indexes], 1)
    Ev = lambda x: pol[1] + pol[0] * x

    print("{}+x*{}".format(pol[1], pol[0]))
    m = (max(Ns[indexes]) + min(Ns[indexes])) / 2
    print(m, "->", Ev(m))

    plt.plot(Ns[indexes], times_our[indexes])  # blue: our
    plt.plot(Ns[indexes], [Ev(x) for x in Ns[indexes]], color='r')  # red: our fit (line)
    plt.plot(Ns[indexes], times_std[indexes], color='g')  # green: their
plt.tight_layout()
plt.show()
