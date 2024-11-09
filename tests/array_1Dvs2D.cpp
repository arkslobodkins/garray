#include <algorithm>
#include <cstdlib>
#include <tuple>

#include "test.hpp"


using namespace spp;


void run_sum(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(sum(A1), sum(A2)));
}


void run_prod(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(prod(A1), prod(A2)));
}


void run_mean(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(mean(A1), mean(A2)));
}


void run_min(const auto& A1, const auto& A2) {
   ASSERT(min(A1) == min(A2));
}


void run_max(const auto& A1, const auto& A2) {
   ASSERT(max(A1) == max(A2));
}


void run_min_index(const auto& A1, const auto& A2) {
   ASSERT(min_index(A1).second == std::get<2>(min_index(A2)));
}


void run_max_index(const auto& A1, const auto& A2) {
   ASSERT(max_index(A1).second == std::get<2>(max_index(A2)));
}


void run_all_finite(const auto& A1, const auto& A2) {
   ASSERT(all_finite(A1) == all_finite(A2));
}


void run_has_inf(const auto& A1, const auto& A2) {
   ASSERT(has_inf(A1) == has_inf(A2));
}


void run_has_nan(const auto& A1, const auto& A2) {
   ASSERT(has_nan(A1) == has_nan(A2));
}


void run_dot_prod(const auto& A1, const auto& B1, const auto& A2, const auto& B2) {
   ASSERT(within_tol_rel(dot_prod(A1, B1), dot_prod(A2, B2)));
}


void run_norm_inf(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(norm_inf(A1), norm_inf(A2)));
}


void run_norm1(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(norm1(A1), norm1(A2)));
}


void run_norm1_scaled(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(norm1_scaled(A1), norm1_scaled(A2)));
}


void run_norm2(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(norm2(A1), norm2(A2)));
}


void run_norm2_scaled(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(norm2_scaled(A1), norm2_scaled(A2)));
}


void run_norm_lp(const auto& A1, const auto& A2, ImplicitInt norm_value) {
   ASSERT(within_tol_rel(norm_lp(A1, norm_value), norm_lp(A2, norm_value)));
}


void run_norm_lp_scaled(const auto& A1, const auto& A2, ImplicitInt norm_value) {
   ASSERT(within_tol_rel(norm_lp_scaled(A1, norm_value), norm_lp_scaled(A2, norm_value)));
}


void run_has_zero(const auto& A1, const auto& A2) {
   ASSERT(has_zero(A1) == has_zero(A2));
}


void run_all_zeros(const auto& A1, const auto& A2) {
   ASSERT(all_zeros(A1) == all_zeros(A2));
}


void run_all_pos(const auto& A1, const auto& A2) {
   ASSERT(all_pos(A1) == all_pos(A2));
}


void run_all_neg(const auto& A1, const auto& A2) {
   ASSERT(all_neg(A1) == all_neg(A2));
}


void run_all_non_pos(const auto& A1, const auto& A2) {
   ASSERT(all_non_pos(A1) == all_non_pos(A2));
}


void run_all_non_neg(const auto& A1, const auto& A2) {
   ASSERT(all_non_neg(A1) == all_non_neg(A2));
}


void run_blas_array(const auto& A1, const auto& A2) {
   const auto A1_ptr = blas_array(A1);
   const auto A2_ptr = blas_array(A2);
   ASSERT(std::equal(A1_ptr.get(), A1_ptr.get() + A1.size().val(), A2_ptr.get()));
}


void run_sort(const auto& A1, const auto& A2) {
   auto B1 = A1;
   auto B2 = A2;
   sort_increasing(B1);
   sort_increasing(B2);
   ASSERT(within_tol_rel(B1, B2.view1D()));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_stable_sum(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_sum(A1), stable_sum(A2)));
}


void run_semi_stable_sum(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(semi_stable_sum(A1), semi_stable_sum(A2)));
}


void run_stable_prod(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_prod(A1), stable_prod(A2)));
}


void run_stable_mean(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_mean(A1), stable_mean(A2)));
}


void run_stable_dot_prod(const auto& A1, const auto& B1, const auto& A2, const auto& B2) {
   ASSERT(within_tol_rel(stable_dot_prod(A1, B1), stable_dot_prod(A2, B2)));
}


void run_semi_stable_dot_prod(const auto& A1, const auto& B1, const auto& A2, const auto& B2) {
   ASSERT(within_tol_rel(semi_stable_dot_prod(A1, B1), semi_stable_dot_prod(A2, B2)));
}


void run_stable_norm1(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_norm1(A1), stable_norm1(A2)));
}


void run_stable_norm1_scaled(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_norm1_scaled(A1), stable_norm1_scaled(A2)));
}


void run_stable_norm2(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_norm2(A1), stable_norm2(A2)));
}


void run_stable_norm2_scaled(const auto& A1, const auto& A2) {
   ASSERT(within_tol_rel(stable_norm2_scaled(A1), stable_norm2_scaled(A2)));
}


void run_stable_norm_lp(const auto& A1, const auto& A2, ImplicitInt norm_value) {
   ASSERT(within_tol_rel(stable_norm_lp(A1, norm_value), stable_norm_lp(A2, norm_value)));
}


void run_stable_norm_lp_scaled(const auto& A1, const auto& A2, ImplicitInt norm_value) {
   ASSERT(within_tol_rel(stable_norm_lp_scaled(A1, norm_value),
                         stable_norm_lp_scaled(A2, norm_value)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_max_abs_error(const auto& A1, const auto& B1, const auto& A2, const auto& B2) {
   ASSERT(within_tol_rel(max_abs_error(A1, B1), max_abs_error(A2, B2)));
}


void run_max_rel_error(const auto& A1, const auto& B1, const auto& A2, const auto& B2) {
   ASSERT(within_tol_rel(*max_rel_error(A1, B1), *max_rel_error(A2, B2)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void ops(ImplicitInt m, ImplicitInt n) {
   Array2D<float> A2 = random<float>(m, n);
   Array2D<float> B2 = random<float>(m, n);

   Array1D<float> A1 = A2.view1D();
   Array1D<float> B1 = B2.view1D();

   run_sum(A1, A2);
   run_prod(A1, A2);
   run_mean(A1, A2);

   run_min(A1, A2);
   run_max(A1, A2);
   run_min_index(A1, A2);
   run_max_index(A1, A2);

   run_all_finite(A1, A2);
   run_has_inf(A1, A2);
   run_has_nan(A1, A2);

   run_dot_prod(A1, B1, A2, B2);

   run_norm_inf(A1, A2);
   run_norm1(A1, A2);
   run_norm1_scaled(A1, A2);
   run_norm2(A1, A2);
   run_norm2_scaled(A1, A2);
   run_norm_lp(A1, A2, 3);
   run_norm_lp_scaled(A1, A2, 3);

   Array2D<float> C2(m, n);
   Array1D<float> C1 = C2.view1D();
   run_has_zero(C1, C2);
   run_all_zeros(C1, C2);
   run_all_pos(C1, C2);
   run_all_neg(C1, C2);
   run_all_non_pos(C1, C2);
   run_all_non_neg(C1, C2);

   run_blas_array(A1, A2);
   run_sort(A1, A2);
}


void stable_ops(ImplicitInt m, ImplicitInt n) {
   Array2D<float> A2 = random<float>(m, n);
   Array2D<float> B2 = random<float>(m, n);

   Array1D<float> A1 = A2.view1D();
   Array1D<float> B1 = B2.view1D();

   run_stable_sum(A1, A2);
   run_semi_stable_sum(A1, A2);
   run_stable_prod(A1, A2);
   run_stable_mean(A1, A2);

   run_stable_dot_prod(A1, B1, A2, B2);
   run_semi_stable_dot_prod(A1, B1, A2, B2);

   run_stable_norm1(A1, A2);
   run_stable_norm1_scaled(A1, A2);
   run_stable_norm2(A1, A2);
   run_stable_norm2_scaled(A1, A2);

   run_stable_norm_lp(A1, A2, 3);
   run_stable_norm_lp_scaled(A1, A2, 3);
   run_stable_norm_lp(A1, A2, 4);
   run_stable_norm_lp_scaled(A1, A2, 4);
}


void error_tools(ImplicitInt m, ImplicitInt n) {
   Array2D<float> A2 = random_not0<float>(m, n);
   Array2D<float> B2 = random_not0<float>(m, n);

   Array1D<float> A1 = A2.view1D();
   Array1D<float> B1 = B2.view1D();

   run_max_abs_error(A1, B1, A2, B2);
   run_max_rel_error(A1, B1, A2, B2);
}


int main() {
   TEST_NON_TYPE(ops, 20, 10);
   TEST_NON_TYPE(stable_ops, 20, 10);
   TEST_NON_TYPE(error_tools, 20, 10);
   return EXIT_SUCCESS;
}

