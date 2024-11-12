#include <cstdlib>
#include <utility>

#include "test.hpp"

using namespace spp;


// For these tests, sufficiently small array sizes should be used,
// otherwise constant evaluation will hit maximum step limit.
template <typename T>
consteval void array1D() {
   Array1D<T, Unaligned>{};
   Array1D<T, Unaligned>(2);
   Array1D<T, Unaligned>(Size{2});
   Array1D<T, Unaligned>(2, Zero<T>);
   Array1D<T, Unaligned>(Size{2}, Value{Zero<T>});
   Array1D<T, Unaligned>{Zero<T>};

   Array1D<T, Unaligned> A(2);
   Array1D<T, Unaligned> B = A;
   Array1D<T, Unaligned> C = const1D<T>(2, Zero<T>);
   Array1D<T, Unaligned> D(C.begin(), C.end());
   Array1D<T, Unaligned> E = std::move(A);

   E = Zero<T>;
   E = {Zero<T>, Zero<T>};
   E = B;
   E = const1D<T>(2, Zero<T>);
   E = std::move(B);

   E.swap(C);
   E.swap(std::move(C));
   E.resize_and_assign(C);
   E.resize_and_assign(std::move(C));
   E.resize(20);
   E.resize_forget(20);

   E.remove(0);
   E.remove(0, 1);
   E.remove(Pos{0});
   E.remove(Pos{0}, Count{1});
   E.remove_front();
   E.remove_front(1);
   E.remove_front(Count{1});
   E.remove_back();
   E.remove_back(1);
   E.remove_back(Count{1});
   E.remove(last);
   E.remove({0, 2, 4});

   E.insert(0, Zero<T>);
   E.insert(Pos{0}, Value(Zero<T>));
   E.insert_front(Zero<T>);
   E.insert_front(Value{Zero<T>});
   E.insert_back(Zero<T>);
   E.insert_back(Value{Zero<T>});
   E.insert(0, D);
   E.insert(Pos{0}, D);
   E.insert_front(D);
   E.insert_back(D);

   E.size();
   E.un(0);
   E.data();
}


template <typename T>
consteval void fixed_array1D() {
   FixedArray1D<T, 2>{};
   FixedArray1D<T, 2>(Zero<T>);
   FixedArray1D<T, 2>(Value{Zero<T>});
   FixedArray1D<T, 2>{Zero<T>, Zero<T>};

   FixedArray1D<T, 2> A;
   FixedArray1D<T, 2> B = A;
   FixedArray1D<T, 2> C = const1D<T>(2, Zero<T>);
   FixedArray1D<T, 2> D(C.begin(), C.end());
   FixedArray1D<T, 2> E = std::move(A);

   E = Zero<T>;
   E = {Zero<T>, Zero<T>};
   E = B;
   E = const1D<T>(2, Zero<T>);
   E = std::move(B);

   E.size();
   E.un(0);
   E.data();
}


template <typename T>
consteval void array2D() {
   Array2D<T, Unaligned>{};
   Array2D<T, Unaligned>(2, 2);
   Array2D<T, Unaligned>(Rows{2}, Cols{2});
   Array2D<T, Unaligned>(2, 2, Zero<T>);
   Array2D<T, Unaligned>(Rows{2}, Cols{2}, Value{Zero<T>});
   Array2D<T, Unaligned>{{Zero<T>, Zero<T>}, {Zero<T>, Zero<T>}};

   Array2D<T, Unaligned> A(2, 2);
   Array2D<T, Unaligned> B = A;
   Array2D<T, Unaligned> C = const2D<T>(2, 2, Zero<T>);
   Array2D<T, Unaligned> D = std::move(A);

   D = Zero<T>;
   D = {{Zero<T>, Zero<T>}, {Zero<T>, Zero<T>}};
   D = B;
   D = const2D<T>(2, 2, Zero<T>);
   D = std::move(B);

   D.swap(C);
   D.swap(std::move(C));
   D.resize_and_assign(C);
   D.resize_and_assign(std::move(C));
   D.resize(20, 20);
   D.resize_forget(20, 20);

   D.remove_rows(0, 1);
   D.remove_rows(Pos{0}, Count{1});
   D.remove_cols(0, 1);
   D.remove_cols(Pos{0}, Count{1});
   D.remove_row(0);
   D.remove_row(Pos{0});
   D.remove_row(last);
   D.remove_col(0);
   D.remove_col(Pos{0});
   D.remove_col(last);

   D.remove_rows_front(1);
   D.remove_rows_front(Count{1});
   D.remove_rows_back(1);
   D.remove_rows_back(Count{1});
   D.remove_cols_front(1);
   D.remove_cols_front(Count{1});
   D.remove_cols_back(1);
   D.remove_cols_back(Count{1});

   D.remove_rows({0, 2, 4});
   D.remove_cols({0, 2, 4});

   D.resize(2, D.cols());
   Array2D<T, Unaligned> E1(1, D.cols());
   D.insert_rows(0, E1);
   D.insert_rows(Pos{0}, E1);
   D.insert_rows_front(E1);
   D.insert_rows_back(E1);

   Array2D<T, Unaligned> E2(D.rows(), 1);
   D.insert_cols(0, E2);
   D.insert_cols(Pos{0}, E2);
   D.insert_cols_front(E2);
   D.insert_cols_back(E2);

   D.resize(2, D.cols());
   Array1D<T, Unaligned> x1(D.cols());
   D.insert_row(0, x1);
   D.insert_row(Pos{0}, x1);
   D.insert_row_front(x1);
   D.insert_row_back(x1);

   Array1D<T, Unaligned> x2(D.rows());
   D.insert_col(0, x2);
   D.insert_col(Pos{0}, x2);
   D.insert_col_front(x2);
   D.insert_col_back(x2);

   D.rows();
   D.cols();
   D.size();
   D.un(0);
   D.un(0, 0);
   D.data();
}


int main() {
   TEST_ALL_TYPES(array1D);
   TEST_ALL_TYPES(fixed_array1D);
   TEST_ALL_TYPES(array2D);
   return EXIT_SUCCESS;
}

