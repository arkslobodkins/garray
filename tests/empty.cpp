#include <cstdlib>

#include "test.hpp"

using namespace spp;


void empty_array_ops() {
   const Array2D<float> A;
   REQUIRE_THROW(sum(A));
   REQUIRE_THROW(prod(A));
   REQUIRE_THROW(mean(A));
   REQUIRE_THROW(min(A));
   REQUIRE_THROW(max(A));
   REQUIRE_THROW(min_index(A));
   REQUIRE_THROW(max_index(A));
   REQUIRE_THROW(all_finite(A));
   REQUIRE_THROW(has_inf(A));
   REQUIRE_THROW(has_nan(A));
   REQUIRE_THROW(dot_prod(A, A));
   REQUIRE_THROW(norm_inf(A));
   REQUIRE_THROW(norm1(A));
   REQUIRE_THROW(norm1_scaled(A));
   REQUIRE_THROW(norm2(A));
   REQUIRE_THROW(norm2_scaled(A));
   REQUIRE_THROW(norm_lp(A, 3));
   REQUIRE_THROW(norm_lp_scaled(A, 3));
   REQUIRE_THROW(polynomial(A.view1D(), 1._sf));

   REQUIRE_THROW(has_zero(A));
   REQUIRE_THROW(all_zeros(A));
   REQUIRE_THROW(all_pos(A));
   REQUIRE_THROW(all_neg(A));
   REQUIRE_THROW(all_non_pos(A));
   REQUIRE_THROW(all_non_neg(A));

   REQUIRE_THROW(none_of(A, 1._sf));
   REQUIRE_THROW(any_of(A, 1._sf));
   REQUIRE_THROW(all_of(A, 1._sf));
   REQUIRE_THROW(none_of(A, [](auto) { return true_sb; }));
   REQUIRE_THROW(all_of(A, [](auto) { return true_sb; }));
   REQUIRE_THROW(any_of(A, [](auto) { return true_sb; }));
   REQUIRE_THROW(none_of(A, A, [](auto, auto) { return true_sb; }));
   REQUIRE_THROW(all_of(A, A, [](auto, auto) { return true_sb; }));
   REQUIRE_THROW(any_of(A, A, [](auto, auto) { return true_sb; }));

   REQUIRE_NOT_THROW(blas_array(A));
   REQUIRE_NOT_THROW(in_open_range(A.view1D(), -1._sf, 1._sf));
   REQUIRE_NOT_THROW(in_closed_range(A.view1D(), -1._sf, 1._sf));
   REQUIRE_NOT_THROW(for_each(A, [](auto) {}));

   Array2D<float> B;
   REQUIRE_NOT_THROW(sort(B, [](auto, auto) { return true_sb; }));
   REQUIRE_NOT_THROW(shuffle(B));
}


void empty_expr_ops() {
   const Array2D<float> A;
   REQUIRE_NOT_THROW(generate(A.view1D(), [](auto) { return 0._sf; }));
   REQUIRE_NOT_THROW(generate(A.view1D(), A.view1D(), [](auto, auto) { return 0._sf; }));
   REQUIRE_NOT_THROW(generate(A, [](auto) { return 0._sf; }));
   REQUIRE_NOT_THROW(generate(A, A, [](auto, auto) { return 0._sf; }));

   REQUIRE_NOT_THROW(merge(A.view1D(), A.view1D()));
   REQUIRE_NOT_THROW(merge(A.view1D(), 0._sf));
   REQUIRE_NOT_THROW(merge(0._sf, A.view1D()));

   REQUIRE_NOT_THROW(sequence<float>(0));
   REQUIRE_NOT_THROW(irange(A.view1D()));

   REQUIRE_NOT_THROW(identity<float>(0));
   REQUIRE_NOT_THROW(const1D(0, Zero<float>));
   REQUIRE_NOT_THROW(const2D(0, 0, Zero<float>));

   REQUIRE_THROW(exclude(A.view1D(), Pos{0}, Count{1}));
   REQUIRE_THROW(e_unit<float>(0, 0));
   REQUIRE_THROW(linspace(0, 0._sf, 1._sf));
}


int main() {
   TEST_NON_TYPE(empty_array_ops);
   TEST_NON_TYPE(empty_expr_ops);
   return EXIT_SUCCESS;
}

