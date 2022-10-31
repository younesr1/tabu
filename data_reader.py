import numpy as np

distances = np.genfromtxt('A5-Distance.csv', delimiter=',')
flow = np.genfromtxt('A5-Flow.csv', delimiter=',')

print(distances)
print(flow)

print(np.sum(np.multiply(distances,flow)))
