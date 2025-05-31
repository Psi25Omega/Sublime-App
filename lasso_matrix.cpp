#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <Eigen/Dense>
#include <Eigen/Core>

using namespace std;
using namespace Eigen;

const int iter = 1000;
const double alpha = 1e-5; 

VectorXd lasso(const MatrixXd& X, const VectorXd& y, double lambda)
{
    int n = X.rows();
    int p = X.cols();
    VectorXd beta = VectorXd::Zero(p);

    for (int it = 0; it < iter; ++it) {
        VectorXd yhat = X * beta;
        VectorXd error = yhat - y;
        double loss = error.squaredNorm();

        if (loss < 0.1) {
            cout << "RSS: " << loss << " Iter: " << it << endl;
            return beta;
        }

        VectorXd gradient = 2*X.transpose()*error;

        for (int j = 0; j < p - 1; ++j) {
            gradient(j) += lambda*((beta(j) >= 0) ? 1 : -1);
        }

        beta -= alpha*gradient; // Entire matrix in one step
    }
    cout << "RSS: " << (X * beta - y).squaredNorm() << " Iter: " << iter << endl;
    return beta;
}

int main() {
    ifstream file("lasso_data.dat");
    if (!file.is_open()) {
        cerr << "Error opening file." << endl;
        return -1;
    }

    // Skip the first line (headings)
    string heads;
    if (!getline(file, heads)) {
        cerr << "File is empty or missing header." << endl;
        return -1;
    }

    vector<vector<double>> data;
    string line;
    while (getline(file, line)) {
        vector<double> row;
        stringstream ss(line);
        string token;
        while (getline(ss, token, ',')) {
            row.push_back(stod(token)); // string to double
        }
        if (!row.empty()) data.push_back(row);
    }
    file.close();

    int n = data.size();
    int p = data[0].size(); // Last column is y
    MatrixXd X(n, p);
    VectorXd y(n);

    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < p - 1; ++j)
        {
            X(i, j) = data[i][j];
        }
        X(i, p - 1) = 1.0; // Intercept
        y(i) = data[i][p - 1];
    }

    double lambda = 1;
    VectorXd beta = lasso(X, y, lambda);

    cout << "Lasso coefficients:" << endl;
    for (int i = 0; i < beta.size(); ++i) {
        cout << "beta[" << i << "] = " << beta(i) << endl;
    }
    return 0;
}
