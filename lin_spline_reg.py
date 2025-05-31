import numpy as np
import matplotlib.pyplot as plt

from sklearn.linear_model import LinearRegression

from sklearn.pipeline import make_pipeline
from sklearn.preprocessing import SplineTransformer

# Observation points
np.random.seed(0)
x = np.linspace(0, 10, 100)
y = 5*np.sin(np.pi*x) + np.random.normal(scale=1, size=100)
X = x[:, np.newaxis]

# Linear Regression
model_lin = LinearRegression()
model_lin.fit(X, y)
y_pred_lin = model_lin.predict(X)

# Spline Regression
spline = make_pipeline(SplineTransformer(n_knots=10, degree=3), LinearRegression())
spline.fit(X, y)
y_pred_spline = spline.predict(X)

spline1 = make_pipeline(SplineTransformer(n_knots=5, degree=3), LinearRegression())
spline1.fit(X, y)
y_pred_spline1 = spline1.predict(X)

plt.scatter(x, y, label='Data', color='blue')
plt.plot(x, y_pred_lin, color='green', linewidth=2)
plt.plot(x, y_pred_spline, color='red', linewidth=2)
plt.plot(x, y_pred_spline1, color='orange', linewidth=2)
plt.show()