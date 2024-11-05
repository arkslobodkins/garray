#include <cstdlib>

#include "test.hpp"


using namespace spp;


void run_stable_sum(const auto& A) {
   Strict32 x1 = stable_sum(A);
   Strict32 x2 = sum(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_semi_stable_sum(const auto& A) {
   Strict32 x1 = semi_stable_sum(A);
   Strict32 x2 = sum(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_prod(const auto& A) {
   Strict32 x1 = stable_prod(A);
   Strict32 x2 = prod(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_mean(const auto& A) {
   Strict32 x1 = stable_mean(A);
   Strict32 x2 = mean(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_dot_prod(const auto& A1, const auto& A2) {
   Strict32 x1 = stable_dot_prod(A1, A2);
   Strict32 x2 = dot_prod(array_cast<double>(A1), array_cast<double>(A2)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_semi_stable_dot_prod(const auto& A1, const auto& A2) {
   Strict32 x1 = semi_stable_dot_prod(A1, A2);
   Strict32 x2 = dot_prod(array_cast<double>(A1), array_cast<double>(A2)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_norm1(const auto& A) {
   Strict32 x1 = stable_norm1(A);
   Strict32 x2 = norm1(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_norm1_scaled(const auto& A) {
   Strict32 x1 = stable_norm1_scaled(A);
   Strict32 x2 = norm1_scaled(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_norm2(const auto& A) {
   Strict32 x1 = stable_norm2(A);
   Strict32 x2 = norm2(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_norm2_scaled(const auto& A) {
   Strict32 x1 = stable_norm2_scaled(A);
   Strict32 x2 = norm2_scaled(array_cast<double>(A)).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_norm_lp(const auto& A, ImplicitInt norm_value) {
   Strict32 x1 = stable_norm_lp(A, norm_value);
   Strict32 x2 = norm_lp(array_cast<double>(A), norm_value).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_norm_lp_scaled(const auto& A, ImplicitInt norm_value) {
   Strict32 x1 = stable_norm_lp_scaled(A, norm_value);
   Strict32 x2 = norm_lp_scaled(array_cast<double>(A), norm_value).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_polynomial(const auto& A) {
   auto z = rands<float>();

   Strict32 x1 = stable_polynomial(A, z);
   Strict32 x2 = polynomial(array_cast<double>(A), z.sd()).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_polynomial_integer(const auto& coeffs) {
   Array1D<int> powers = random(coeffs.size(), 0_si, 10_si);
   auto z = rands<float>();

   Strict32 x1 = stable_polynomial(coeffs, z, powers);
   Strict32 x2 = polynomial(array_cast<double>(coeffs), z.sd(), powers).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void run_stable_gpolynomial(const auto& coeffs) {
   Array1D<int> powers = random(coeffs.size(), 0_si, 10_si);
   Array1D<float> Z = random<float>(coeffs.size());

   Strict32 x1 = stable_gpolynomial(coeffs, Z, powers);
   Strict32 x2 = gpolynomial(array_cast<double>(coeffs), array_cast<double>(Z), powers).sf();
   ASSERT(within_tol_rel(x1, x2));
}


void stable_ops(ImplicitInt n) {
   Array1D<float> A = random<float>(n);
   Array1D<float> B = random<float>(n);

   run_stable_sum(A);
   run_semi_stable_sum(A);
   run_stable_prod(A);
   run_stable_mean(A);

   run_stable_dot_prod(A, B);
   run_semi_stable_dot_prod(A, B);

   run_stable_norm1(A);
   run_stable_norm1_scaled(A);
   run_stable_norm2(A);
   run_stable_norm2_scaled(A);

   run_stable_norm_lp(A, 3);
   run_stable_norm_lp_scaled(A, 3);
   run_stable_norm_lp(A, 4);
   run_stable_norm_lp_scaled(A, 4);

   run_stable_polynomial(A);
   run_stable_polynomial_integer(A);
   run_stable_gpolynomial(A);
}


int main() {
   TEST_NON_TYPE(stable_ops, 1000);
   return EXIT_SUCCESS;
}
