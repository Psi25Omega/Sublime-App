import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
import math

x = np.linspace(-5, 5, 100)
y = 2*x + np.random.normal(0, 1, len(x))

perf = 2*x

y_1 = -x
y_2 = x - 3

y_3 = -x**2 + x + 1

def corr(x, y):
    return np.corrcoef(x, y)

def rse(x, y):
    return np.sqrt(np.mean((x - y) ** 2))

plt.plot(x, y, 'o', color='black', markersize=2)
plt.plot(x, y_1, label=f'Corr = {corr(y_1, y)[0,1]:.2f}, RSE = {rse(y_1, y):.2f}', color='red')
plt.plot(x, y_2, label=f'Corr = {corr(y_2, y)[0,1]:.2f}, RSE = {rse(y_2, y):.2f}', color='blue')
plt.plot(x, y_3, label=f'Corr = {corr(y_3, y)[0,1]:.2f}, RSE = {rse(y_3, y):.2f}', color='green')
plt.plot(x, perf, color='orange', linestyle='--')

plt.legend()

plt.show()