#include <Rcpp.h>
#include <immintrin.h>
#include <vector>
#include <omp.h>

using namespace Rcpp;

// Convert R matrix (column-major) to row-major vector<vector<double>>
std::vector<std::vector<double>> convert_to_row_major(NumericMatrix mat) {
    int nrow = mat.nrow();
    int ncol = mat.ncol();
    std::vector<std::vector<double>> result(nrow, std::vector<double>(ncol));
    for (int i = 0; i < nrow; ++i) {
        for (int j = 0; j < ncol; ++j) {
            result[i][j] = mat(i, j);
        }
    }
    return result;
}

// Convert row-major vector<vector<double>> to R matrix (column-major)
NumericMatrix convert_to_column_major(const std::vector<std::vector<double>>& mat) {
    int nrow = mat.size();
    int ncol = (nrow > 0) ? mat[0].size() : 0;
    NumericMatrix result(nrow, ncol);
    for (int i = 0; i < nrow; ++i) {
        for (int j = 0; j < ncol; ++j) {
            result(i, j) = mat[i][j];
        }
    }
    return result;
}

// AVX-optimized matrix multiplication (original code)
std::vector<std::vector<double>> multiply_tile_avx(
    const std::vector<std::vector<double>>& A,
    const std::vector<std::vector<double>>& B,
    int tile_size
) {
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

// [[Rcpp::export]]
NumericMatrix rcpp_multiply_tile_avx(NumericMatrix A, NumericMatrix B, int tile_size) {
    // Convert R matrices to row-major C++ matrices
    std::vector<std::vector<double>> A_row_major = convert_to_row_major(A);
    std::vector<std::vector<double>> B_row_major = convert_to_row_major(B);
    
    // Perform multiplication
    auto result = multiply_tile_avx(A_row_major, B_row_major, tile_size);
    
    // Convert result to R matrix (column-major)
    return convert_to_column_major(result);
}
