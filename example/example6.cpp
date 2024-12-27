#include <cassert>
#include <cstdlib>
#include <iostream>
#include <optional>
#include <strict.hpp>
#include <utility>

using namespace spp;


// example6 solves N x N linear system of equations using straightforward implementation of Jacobi
// method. Two-dimensional array class is introduced.


template <Floating T>
std::optional<std::pair<Array1D<T>, index_t>> jacobi(const Array2D<T>& A, const Array1D<T>& b,
                                                     Strict<T> tol) {
   assert(A.rows() == A.cols() && A.cols() == b.size());

   const index_t N = A.rows();
   const index_t max_its = 100_sl * N;
   Array1D<T> xprev(N);
   Array1D<T> xnext(N);

   for(auto iter : irange(max_its)) {
      for(auto i : irange(N)) {
         xnext[i] = (b[i] - dot_prod(exclude(A.row(i), i), exclude(xprev, i))) / A(i, i);
      }

      auto matrix_vector_product
          = row_reduce(A, [&xnext](auto row) { return dot_prod(row, xnext); });

      if(max_abs_error(matrix_vector_product, b) / norm_inf(b) <= tol) {
         return {std::pair{xnext, iter + 1_sl}};
      }
      xprev = xnext;
   }

   return std::nullopt;
}


int main() {
   constexpr index_t N = 100_sl;
   using T = float64;
   constexpr Strict<T> tol = Thousand<T> * constants::epsilon<T>;

   Array2D<T> A(N, N);
   Array1D<T> b(N);
   random(A, b);
   // Ensure A is diagonally dominant for convergence.
   A.diag() += row_reduce(A, [](auto row) { return sum(row); });

   auto x_opt = jacobi(A, b, tol);

   if(x_opt) {
      std::cout << "converged in " << x_opt->second << " iterations." << std::endl;
   } else {
      std::cout << "Jacobi method did not converge" << std::endl;
   }

   return EXIT_SUCCESS;
}
