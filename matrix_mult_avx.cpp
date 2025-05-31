#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>
#include <string>

#include <immintrin.h>  // For AVX intrinsics

// OpenMP
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

    # pragma omp parallel for // Comment out to run without parallel processing
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

vector<vector<double>> multiply_tile_avx(const vector<vector<double>>& A, 
                                        const vector<vector<double>>& B, 
                                        int tile_size) {
    int r1 = A.size();
    int c1 = A[0].size();
    int c2 = B[0].size();
    vector<vector<double>> result(r1, vector<double>(c2, 0.0));

    // Process tiles
    # pragma omp parallel for // Comment out to run without parallel processing
    for (int i = 0; i < r1; i += tile_size) {
        for (int j = 0; j < c2; j += tile_size) {
            for (int k = 0; k < c1; k += tile_size) {
                // Tile boundaries
                int i_end = min(i + tile_size, r1);
                int j_end = min(j + tile_size, c2);
                int k_end = min(k + tile_size, c1);

                // Process within the tile
                for (int ii = i; ii < i_end; ++ii) {
                    for (int kk = k; kk < k_end; ++kk) {
                        double a_val = A[ii][kk];
                        int jj = j;

                        // Process 4 elements at a time using AVX
                        for (; jj <= j_end - 4; jj += 4) {
                            __m256d b_vec = _mm256_loadu_pd(&B[kk][jj]); // Load 4 doubles from B
                            __m256d res_vec = _mm256_loadu_pd(&result[ii][jj]); // Load current result
                            __m256d a_broadcast = _mm256_set1_pd(a_val); // Broadcast A's value
                            res_vec = _mm256_fmadd_pd(a_broadcast, b_vec, res_vec); // Fused multiply-add
                            _mm256_storeu_pd(&result[ii][jj], res_vec); // Store back
                        }

                        // Handle remaining elements (if tile_size not multiple of 4)
                        for (; jj < j_end; ++jj) {
                            result[ii][jj] += a_val * B[kk][jj];
                        }
                    }
                }
            }
        }
    }
    return result;
}

int main()
{
    string filename = "matrix.txt";
    vector<vector<double>> A = read_matrix(filename);
    const int tile_size = 64;

    auto start = std::chrono::high_resolution_clock::now();

    vector<vector<double>> result = multiply_tile_avx(A, A, tile_size);

    cout << A.size() << ", " << A[0].size() << endl;

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);

    cout << "Tile size: " << tile_size << ", Time taken: " << duration.count() << " microsec." << endl;
    return 0;
}