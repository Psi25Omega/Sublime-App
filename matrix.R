library(Rcpp)
sourceCpp("matrix_basic_r.cpp")

# Example usage
A <- matrix(rnorm(1024*1024), 1024, 1024)
B <- matrix(rnorm(1024*1024), 1024, 1024)

result <- multiply_tile_avx(A, B)
print(dim(result))