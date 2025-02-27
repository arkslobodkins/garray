#include <algorithm>
#include <cstdlib>
#include <utility>

#include "test.hpp"


using namespace spp;


#define BUILTIN_TYPE_OF(A) spp::BuiltinTypeOf<decltype(A)>


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void run_constr_default() {
   ASSERT(Array1D<T>{}.size() == 0_sl);
}


template <Builtin T>
void run_constr_size(ImplicitInt n) {
   ASSERT(Array1D<T>(n).size() == n.get());
   ASSERT(Array1D<T>(Size{n.get()}).size() == n.get());
}


template <Builtin T>
void run_constr_size_value(ImplicitInt n) {
   Array1D<T> A1(n, One<T>);
   ASSERT(A1.size() == n.get());
   ASSERT(all_of(A1, One<T>));

   Array1D<T> A2(Size{n.get()}, Value{One<T>});
   ASSERT(A2.size() == n.get());
   ASSERT(all_of(A2, One<T>));
}


template <Builtin T>
void run_constr_list() {
   auto x0 = Zero<T>;
   auto x1 = One<T>;
   Array1D<T> A{x0, x0, x1, x1};
   ASSERT(A[0] == x0);
   ASSERT(A[1] == x0);
   ASSERT(A[2] == x1);
   ASSERT(A[3] == x1);
   ASSERT(A.size() == 4_sl);
}


template <Builtin T>
void run_constr_iterator(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2(A1.begin(), A1.end());
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_constr_copy(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_constr_move(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2 = A1;
   Array1D<T> A3 = std::move(A1);
   ASSERT(A2 == A3);
   ASSERT(A1.empty());
   ASSERT(A1.data() == nullptr);
}


template <Builtin T>
void run_constr_copy_other(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2 = A1.view1D();
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_constr_size_fail() {
   REQUIRE_THROW(Array1D<T> A(-1));
}


template <Builtin T>
void run_constr_iterator_fail() {
   Array1D<T> A(4);
   REQUIRE_THROW(Array1D<T>(A.end(), A.begin()));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void run_assign_value(ImplicitInt n) {
   Array1D<T> A(n);
   A = One<T>;
   ASSERT(all_of(A, One<T>));
}


template <Builtin T>
void run_assign_list() {
   auto x0 = Zero<T>;
   auto x1 = One<T>;
   Array1D<T> A(4);
   A = {x0, x0, x1, x1};
   ASSERT(A[0] == x0);
   ASSERT(A[1] == x0);
   ASSERT(A[2] == x1);
   ASSERT(A[3] == x1);
}


template <Builtin T>
void run_assign_copy(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2(n);
   A2 = A1;
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_assign_move(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2 = A1;
   Array1D<T> A3(n);
   A3 = std::move(A1);

   ASSERT(A2 == A3);
   ASSERT(A1.empty());
   ASSERT(A1.data() == nullptr);

   // self-move
   ASSERT(A2 == std::move(A2));
   ASSERT(A2 == A3);
}


template <Builtin T>
void run_assign_copy_other(ImplicitInt n) {
   Array1D<T> A1 = random<T>(n);
   Array1D<T> A2(n);
   A2 = A1.view1D();
   ASSERT(A1 == A2);
}


template <Builtin T>
void run_assign_list_fail() {
   Array1D<T> A(4);
   REQUIRE_THROW((A = {Zero<T>, Zero<T>, Zero<T>}));
}


template <Builtin T>
void run_assign_copy_fail() {
   Array1D<T> A1(4), A2(5);
   REQUIRE_THROW(A2 = A1);
}


template <Builtin T>
void run_assign_copy_other_fail() {
   Array1D<T> A1(4), A2(5);
   REQUIRE_THROW(A2 = A1(place::all));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_swap(auto X1, auto Y1) {
   auto X2 = X1;
   auto Y2 = Y1;

   X1.swap(Y1);
   ASSERT(X1 == Y2);
   ASSERT(Y1 == X2);

   // self-swap
   X1.swap(X1);
   ASSERT(X1 == Y2);
}


void run_swap_rvalue(auto X) {
   X.swap(decltype(X){});
   ASSERT(X.empty());
   ASSERT(X.size() == 0_sl);
}


void run_resize_assign(auto X1, auto X2) {
   X1.resize_and_assign(X2);
   ASSERT(X1 == X2);
}


void run_resize_assign_move(auto X1, auto X2) {
   auto X3 = X2;
   X1.resize_and_assign(std::move(X2));
   ASSERT(X1 == X3);
   ASSERT(X2.empty());
}


void run_remove(auto X, Pos pos, Count count) {
   auto Y = X;
   X.remove(pos, count);
   ASSERT(X == exclude(Y, pos, count));
}


void run_remove_front(auto X, Count count) {
   auto Y = X;
   X.remove_front(count);
   ASSERT(X == Y(place::lastN(X.size())));
}


void run_remove_back(auto X, Count count) {
   auto Y = X;
   X.remove_back(count);
   ASSERT(X == Y(place::firstN(X.size())));
}


void run_remove_last(auto X) {
   auto Y = X;
   X.remove(last);
   ASSERT(X == exclude(Y, last));
}


void run_remove_vec(auto X, std::vector<ImplicitInt> indexes) {
   auto Y = X;
   X.remove(indexes);
   ASSERT(X == Y(place::complement(indexes)));
}


template <Builtin T>
void run_resize_fail() {
   Array1D<T> A;
   REQUIRE_THROW(A.resize(-1));
   REQUIRE_NOT_THROW(A.resize(0));
}


template <Builtin T>
void run_remove_fail() {
   Array1D<T> A(10);
   REQUIRE_THROW(A.remove(0, 0));
   REQUIRE_THROW(A.remove(-1));
   REQUIRE_THROW(A.remove(0, A.size() + 1_sl));
   REQUIRE_NOT_THROW(A.remove(0, A.size()));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_insert_value(auto X, Pos pos) {
   auto z = rands<BUILTIN_TYPE_OF(X)>();
   auto Y = X;

   X.insert(pos, Value{z});
   ASSERT(exclude(X, pos) == Y);
   ASSERT(X[pos.get()] == z);
}


void run_insert_value_front(auto X) {
   auto z = rands<BUILTIN_TYPE_OF(X)>();
   auto Y = X;

   X.insert_front(z);
   ASSERT(merge(z, Y) == X);
}


void run_insert_value_back(auto X) {
   auto z = rands<BUILTIN_TYPE_OF(X)>();
   auto Y = X;

   X.insert_back(z);
   ASSERT(merge(Y, z) == X);
}


void run_insert_array(auto X, Pos pos) {
   using namespace place;
   auto Y = X;
   auto Z = X;

   X.insert(pos, Y);
   ASSERT(X == merge(Z(firstN(pos.get())), Y, Z(lastN(X.size() - Y.size() - pos.get()))));
}


void run_insert_array_front(auto X) {
   const auto Y = random<BUILTIN_TYPE_OF(X)>(X.size()).eval();
   X.insert_front(Y);
   ASSERT(merge(Y, X(place::lastN(X.size() / 2_sl))) == X);
}


void run_insert_array_back(auto X) {
   const auto Y = random<BUILTIN_TYPE_OF(X)>(X.size()).eval();
   X.insert_back(Y);
   ASSERT(merge(X(place::firstN(X.size() / 2_sl)), Y) == X);
}


template <Builtin T>
void run_insert_fail() {
   Array1D<T> A(10);
   REQUIRE_THROW(A.insert(-1, A));
   REQUIRE_THROW(A.insert(A.size() + 1_sl, A));
   REQUIRE_NOT_THROW(A.insert(A.size(), A));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void run_resize_and_assign_strong() {
   Array1D<int> A1{2_si, 2_si, 2_si};
   auto A2 = A1;
   auto A3 = A2;
   REQUIRE_THROW((A2.resize_and_assign(A1 / Array1D<int>{2_si, 2_si, 0_si}.lval())));
   ASSERT(A3 == A2);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
void array_constructor() {
   constexpr index_t n = 100_sl;

   run_constr_default<T>();
   run_constr_size<T>(n);
   run_constr_size_value<T>(n);
   run_constr_list<T>();
   run_constr_iterator<T>(n);
   run_constr_copy<T>(n);
   run_constr_move<T>(n);
   run_constr_copy_other<T>(n);

   run_constr_size_fail<T>();
   run_constr_iterator_fail<T>();
}


template <Builtin T>
void array_assignment() {
   constexpr index_t n = 100_sl;

   run_assign_value<T>(n);
   run_assign_list<T>();
   run_assign_copy<T>(n);
   run_assign_move<T>(n);
   run_assign_copy_other<T>(n);

   run_assign_list_fail<T>();
   run_assign_copy_fail<T>();
   run_assign_copy_other_fail<T>();
}


template <Builtin T>
void array_resize() {
   constexpr index_t n = 100_sl;
   const Array1D<T> A1 = random<T>(n);
   const Array1D<T> A2 = random<T>(2_sl * n);

   run_swap(A1, A2);
   run_swap_rvalue(A1);
   run_resize_assign(A1, A2);
   run_resize_assign_move(A1, A2);
}


template <Builtin T>
void array_remove() {
   constexpr index_t n = 100_sl;
   const Array1D<T> A = random<T>(n);

   run_remove(A, Pos{0}, Count{1});
   run_remove(A, Pos{0}, Count{25});
   run_remove(A, Pos{10}, Count{1});
   run_remove(A, Pos{10}, Count{25});

   run_remove_front(A, Count{10});
   run_remove_back(A, Count{10});

   run_remove_last(A);
   run_remove_vec(A, {0, 2, 5, 40, 99});

   run_resize_fail<T>();
   run_remove_fail<T>();
}


template <Builtin T>
void array_insert() {
   constexpr index_t n = 100_sl;
   const Array1D<T> A = random<T>(n);

   run_insert_value(A, Pos{0});
   run_insert_value(A, Pos{10});
   run_insert_value(A, Pos{99});
   run_insert_value_front(A);
   run_insert_value_back(A);

   run_insert_array(A, Pos{0});
   run_insert_array(A, Pos{10});
   run_insert_array(A, Pos{99});
   run_insert_array_front(A);
   run_insert_array_back(A);

   run_insert_fail<T>();
}


template <Real T>
void array_data() {
   constexpr index_t n = 100_sl;
   Array1D<T> A1(n, One<T>);
   const Array1D<T> A2(n, One<T>);

   Strict<T>* a_ptr = A1.data();
   const Strict<T>* b_ptr = A2.data();
   ASSERT(std::all_of(a_ptr, a_ptr + n.val(), [](auto x) { return Strict{x} == One<T>; }));
   ASSERT(std::all_of(b_ptr, b_ptr + n.val(), [](auto x) { return Strict{x} == One<T>; }));

   T* a_blas_ptr = A1.blas_data();
   const T* b_blas_ptr = A2.blas_data();
   ASSERT(
       std::all_of(a_blas_ptr, a_blas_ptr + n.val(), [](auto x) { return Strict{x} == One<T>; }));
   ASSERT(
       std::all_of(b_blas_ptr, b_blas_ptr + n.val(), [](auto x) { return Strict{x} == One<T>; }));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
void array_strong_guarantee() {
   run_resize_and_assign_strong();
}


////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
   TEST_ALL_TYPES(array_constructor);
   TEST_ALL_TYPES(array_assignment);
   TEST_ALL_TYPES(array_resize);
   TEST_ALL_TYPES(array_remove);
   TEST_ALL_TYPES(array_insert);
   TEST_ALL_REAL_TYPES(array_data);
   TEST_NON_TYPE(array_strong_guarantee);

   return EXIT_SUCCESS;
}
