#include <Rcpp.h>
using namespace Rcpp;

// [[Rcpp::export]]
NumericMatrix rcpp_matrix_mult(NumericMatrix A, NumericMatrix B) {
  int n = A.nrow();
  NumericMatrix C(n, n);
  for (int i = 0; i < n; ++i)
    for (int j = 0; j < n; ++j)
      for (int k = 0; k < n; ++k)
        C(i, j) += A(i, k) * B(k, j);
  return C;
}
