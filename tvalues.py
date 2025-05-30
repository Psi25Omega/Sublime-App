import pandas as pd
import numpy as np

file = 'data.dat'
data = pd.read_csv(file, delim_whitespace=True)
cols = data.columns[:-1]

def t_value(v, h_mu):
    n = len(v)
    mu, std = np.mean(v), np.std(v, ddof=1)
    t_value = (mu - h_mu) / (std / np.sqrt(n))

    return t_value

for col in cols:
    print(f't-value for {col}: {t_value(data[col], 0):.5f}')