// Simulating Linear and Spline Regression in CPP with one parameter
#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Number of iterations
const int iter = 1000;
const int alpha = 0.05;

double msecalc(vector<float> x, vector<float> y)
{
    float mse = 0;
    int n = x.size();
    for (int i = 0; i < n; i++)
    {
        mse += (x[i] - y[i])*(x[i] - y[i]);
    }
    mse /= n;
    return mse;
}

// Linear Regression
double linear_regrression(vector<float> x, vector<float> y)
{
    float m = 0, b = 0;
    int n = x.size();
    for (int i = 0; i < iter; i++)
    {
        float m_partial = 0, b_partial = 0;
        for (int j = 0; j < n; j++)
        {
            m_partial += x[j]*(x[j] - (m*x[j] + b));
            b_partial += (x[j] - (m*x[j] + b));
        }
        m -= (m_partial*alpha)/n;
        b -= (b_partial*alpha)/n;
    }
    return m, b;
}

// Spline Regression
