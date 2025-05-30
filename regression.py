import pandas as pd
import numpy as np

iter = 1000
alpha = 0.05

file = 'regression_data.csv'
data = pd.read_csv(file)

def mse(f, y):
    return np.mean((f-y)**2)

def linear_regression(x, y, iter):
    m, b = 0, 0
    y_pred = m*x + b
    for _ in range(iter):
        mse = mse(y_pred, y)
        if mse < 0.01:
            return m, b

        m_grad = -2*np.mean(x*(y - y_pred))
        b_grad = -2*np.mean(y - y_pred)
        m -= alpha*m_grad
        b -= alpha*b_grad

        y_pred = m*x + b
    
    return m, b