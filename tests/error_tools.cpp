#include <cstdlib>

#include "test.hpp"


using namespace spp;


template <Floating T>
static constexpr auto eps10 = Strict{T(10)} * constants::epsilon<T>;


template <Floating T>
static constexpr auto eps1000 = Strict{T(1000)} * constants::epsilon<T>;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
void run_within_tol_abs() {
   static_assert(within_tol_abs(One<T>, One<T> + eps10<T>));
   static_assert(not within_tol_abs(One<T>, One<T> + eps1000<T>));

   Array1D<T> x = random<T>(1000);
   ASSERT(within_tol_abs(x, x));
   ASSERT(within_tol_abs(x, x + eps10<T>));
   ASSERT(not within_tol_abs(x, x + eps1000<T>));
}


template <Floating T>
void run_within_tol_abs_fail() {
   using Array = Array1D<T>;
   REQUIRE_THROW(within_tol_abs(Array{}, Array{}));
   REQUIRE_THROW(within_tol_abs(Array(1), Array(2)));
   REQUIRE_THROW(within_tol_abs(Array(1), Array(1), -constants::epsilon<T>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
void run_within_tol_rel() {
   static_assert(within_tol_rel(One<T>, One<T> + eps10<T>));
   static_assert(not within_tol_rel(One<T>, One<T> + eps1000<T>));

   Array1D<T> x = random<T>(1000);
   ASSERT(within_tol_rel(x, x));
   ASSERT(within_tol_rel(x, x + x * eps10<T>));
   ASSERT(not within_tol_rel(x, x + x * eps1000<T>));
}


template <Floating T>
void run_within_tol_rel_fail() {
   using Array = Array1D<T>;
   REQUIRE_THROW(within_tol_rel(Array{}, Array{}));
   REQUIRE_THROW(within_tol_rel(Array(1), Array(2)));
   REQUIRE_THROW(within_tol_rel(Array(1), Array(1), -constants::epsilon<T>));
   REQUIRE_THROW(
       within_tol_rel(Array(1), Array(1), constants::epsilon<T>, -constants::epsilon<T>));

   REQUIRE_NOT_THROW(within_tol_rel(Array(1), Array(1), constants::epsilon<T>));
   REQUIRE_NOT_THROW(
       within_tol_rel(Array(1), Array(1), constants::epsilon<T>, constants::epsilon<T>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
void run_abs_error() {
   Array1D<T> x = random<T>(1000, NegOne<T>, One<T>);
   ASSERT(max_abs_error(x, x) <= constants::epsilon<T>);
   ASSERT(max_abs_error(x, x + eps10<T>) <= DefaultTol<T>::value);
   ASSERT(not(max_abs_error(x, x + eps1000<T>) <= DefaultTol<T>::value));
}


template <Floating T>
void run_abs_error_fail() {
   using Array = Array1D<T>;
   REQUIRE_THROW(max_abs_error(Array{}, Array{}));
   REQUIRE_THROW(max_abs_error(Array(1), Array(2)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
void run_rel_error() {
   Array1D<T> x = random_not0<T>(1000, NegOne<T>, One<T>);
   ASSERT(*max_rel_error(x, x) <= eps10<T>);
   ASSERT(*max_rel_error(x, x + x * eps10<T>) <= DefaultTol<T>::value);
   ASSERT(not(*max_rel_error(x, x + x * eps1000<T>) <= DefaultTol<T>::value));

   // Test return value of optional.
   ASSERT(not rel_error(Zero<T>, Zero<T>));
   ASSERT(not rel_error(Zero<T>, One<T>));
   ASSERT(not rel_error(One<T>, Zero<T>));
}


template <Floating T>
void run_rel_error_fail() {
   using Array = Array1D<T>;
   REQUIRE_THROW(max_rel_error(Array{}, Array{}));
   REQUIRE_THROW(max_rel_error(Array(1), Array(2)));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
void within_tol_abs() {
   run_within_tol_abs<T>();
   run_within_tol_abs_fail<T>();
}


template <Floating T>
void within_tol_rel() {
   run_within_tol_rel<T>();
   run_within_tol_rel_fail<T>();
}


template <Floating T>
void abs_error() {
   run_abs_error<T>();
   run_abs_error_fail<T>();
}


template <Floating T>
void rel_error() {
   run_rel_error<T>();
   run_rel_error_fail<T>();
}


template <Floating T>
consteval void constexpr_error() {
   Array1D<T> x(Size{10}, Value{One<T>});
   Array1D<T> y(Size{10}, Value{One<T>});

   ASSERT(within_tol_abs(x, y));
   ASSERT(within_tol_rel(x, y));

   ASSERT(max_abs_error(x, y) < One<T>);
   ASSERT(*max_rel_error(x, y) < One<T>);
}


int main() {
   TEST_ALL_FLOAT_TYPES(within_tol_abs);
   TEST_ALL_FLOAT_TYPES(within_tol_rel);
   TEST_ALL_FLOAT_TYPES(abs_error);
   TEST_ALL_FLOAT_TYPES(rel_error);
   TEST_ALL_FLOAT_TYPES(constexpr_error);
   return EXIT_SUCCESS;
}
