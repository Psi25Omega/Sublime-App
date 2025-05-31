import numpy as np
import pandas as pd

# Create a 128x128 matrix of random values
matrix = np.random.rand(512, 512)

# Save to a file if needed
print(matrix)

np.savetxt('matrix.txt', matrix, delimiter=',', fmt='%.6f')