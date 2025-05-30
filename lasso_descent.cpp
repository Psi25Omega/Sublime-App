// This is a primitive descent implemention of Lasso regression.
// I think we need to normalize the features before running to get a better model. This is done by Least Angles Regression.

#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>

using namespace std;
const int iter = 1000;
const double alpha = 10e-6;

vector<double> lasso(const vector<vector<double>> & X, const vector<double> & y, double lambda)
{
    int n = X.size();
    int p = X[0].size();
    vector<double> beta(p, 0.0);
    double rss = 0.0;

    // Descent with 1000 iterations
    for (int it = 0; it < iter; it++)
    {
        double loss = 0.0;

        vector<double> gradient(p, 0.0);
        // Going through grad for each beta_i
        for(int j = 0; j < p; j++)
        {   
            // Going through X row by row
            for (int i = 0; i < n; i++)
            {
                double yhat = 0.0;
                for (int k = 0; k < p; k++)
                {
                    yhat += X[i][k]*beta[k];
                }
                double error = yhat - y[i];
                loss += error*error;
                
                if (loss < 0.1)
                {
                    return beta;
                }
                // Gradient calcs - Check notes for formula.
                gradient[j] += 2*X[i][j]*error;
            }
            rss = loss;
            if (loss < 0.1) // Stop descent at low enough loss
                {
                    cout << "RSS: " << loss << " Iter: " << it << endl;
                    return beta;
                }
            
            if (j != p-1) // Don't add for bias term
            {
                gradient[j] += lambda*beta[j]/abs(beta[j] + 1e-10);
            }
            // Descent step
            beta[j] -= alpha*gradient[j];
            // cout << "Iteration: " << it << ", Loss: " << loss << endl;
        }
        // cout << "Iter: " << it << " Loss: " << loss << endl;
    }
    cout << "RSS: " << rss << " Iter: 1000" <<  endl;
    return beta;
}

int main()
{
    ifstream file("lasso_data.dat");
    if (!file.is_open())
    {
        cerr << "Error opening file." << endl;
        return -1;
    }
    vector<vector<double>> X;
    vector<double> y;
    string line;

    // // Skip the first line (headings)
    // if (!getline(file, line)) {
    //     cerr << "File is empty or missing header." << endl;
    //     return -1;
    // }

    while (getline(file, line))
    {
        vector<double> row;

        stringstream ss(line);
        string token;

        while (getline(file, line)) 
        {
            vector<double> row;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ','))
            {
                row.push_back(stod(token));
            }
            if (!row.empty())
            {
                y.push_back(row.back()); // Last element for y
                row.pop_back();

                row.push_back(1.0); // Add 1 for intercept calculation
                X.push_back(row);
            }
        }
    }
    file.close();

    double lambda = 0.1;
    vector<double> coefficients = lasso(X, y, lambda);
    cout << "Lasso coefficients:" << endl;
    for (size_t i = 0; i < coefficients.size(); ++i)
    {
        cout << "beta[" << i << "] = " << coefficients[i] << endl;
    }
}
