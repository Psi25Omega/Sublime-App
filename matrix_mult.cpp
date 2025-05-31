#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <omp.h>

// Timing
#include <chrono>

using namespace std;

// Reading matrix from matrix.txt
vector<vector<double>> read_matrix(const string &filename) {
    vector<vector<double>> X;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return {};
    }

    string line;
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
            X.push_back(row);
        }
    }
    file.close();
    return X;
}


// Given implementation
vector<vector<double>> multiply(const vector<vector<double>> & A, const vector<vector<double>> & B)
{
    int r1 = A.size();
    int c1 = A[0].size();
    int c2 = B[0].size();
    vector<vector<double>> result(r1, vector<double>(c2, 0));

    for (int i = 0; i < r1; ++i)
    {
        for (int j = 0; j < c2; ++j)
        {
            for (int k = 0; k < c1; ++k)
            {
                result[i][j] += A[i][k]*B[k][j];
            }
        }
    }
    return result;
}

vector<vector<double>> multiply_tile(const vector<vector<double>> & A, const vector<vector<double>> & B, int tile_size) 
{
    int n = A.size();
    vector<vector<double>> C(n, vector<double>(n, 0.0));
    
    # pragma omp parallel for
    for (int i = 0; i < n; i += tile_size)
    {
        for (int k = 0; k < n; k += tile_size)
        {
            for (int j = 0; j < n; j += tile_size)
            {   
                // Tile coords
                int i_end = min(i + tile_size, n);
                int k_end = min(k + tile_size, n);
                int j_end = min(j + tile_size, n);
                
                for (int ii = i; ii < i_end; ++ii)
                {
                    for (int kk = k; kk < k_end; ++kk)
                    {
                        double a_val = A[ii][kk];
                        for (int jj = j; jj < j_end; ++jj)
                        {
                            C[ii][jj] += a_val * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
    return C;
}



int main()
{
    string filename = "matrix.txt";
    vector<vector<double>> A = read_matrix(filename);
    const int tile_size = 2;

    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<double>> result = multiply_tile(A, A, tile_size);

    cout << A.size() << ", " << A[0].size() << endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    cout << "Tile size: " << tile_size << ", Time taken: " << duration.count() << " microsec." << endl;
    return 0;
}