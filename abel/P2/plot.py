from matplotlib import pyplot as plt

text = """
n=2 seconds=8.74e-06
n=3 seconds=1.342e-05
n=4 seconds=2.212e-05
n=5 seconds=4.724e-05
n=6 seconds=0.00011206
n=7 seconds=0.0003011
n=8 seconds=0.00077344
n=9 seconds=0.00205368
n=10 seconds=0.00476068
n=11 seconds=0.0164881
n=12 seconds=0.0683343
n=13 seconds=0.178166
n=14 seconds=0.599244
n=15 seconds=1.62728
n=16 seconds=5.34224
n=17 seconds=21.2235
"""

result = """
n=2 : tests=35 average=1.69714e-05
n=3 : tests=36 average=2.07222e-05
n=4 : tests=38 average=2.75526e-05
n=5 : tests=36 average=4.41111e-05
n=6 : tests=31 average=9.10645e-05
n=7 : tests=29 average=0.000155448
n=8 : tests=28 average=0.000433071
n=9 : tests=36 average=0.00124283
n=10 : tests=37 average=0.00364116
n=11 : tests=29 average=0.00866135
n=12 : tests=37 average=0.0157412
n=13 : tests=47 average=0.0731416
n=14 : tests=32 average=0.187412
n=15 : tests=36 average=0.553792
n=16 : tests=13 average=2.89207
"""

ns = []
seconds = []

for line in result.split('\n'):
    if not line: continue
    ns.append(int(line.split(' ')[0].split('=')[1]))
    seconds.append(float(line.split(' ')[3].split('=')[1]))

plt.plot(ns, seconds)
plt.show()
