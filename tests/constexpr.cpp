#include <cstdlib>
#include <utility>

#include "test.hpp"

using namespace spp;


template <typename T>
consteval void array1D() {
   Array1D<T, Unaligned>{};
   Array1D<T, Unaligned>(10);
   Array1D<T, Unaligned>(Size{10});
   Array1D<T, Unaligned>(10, Zero<T>);
   Array1D<T, Unaligned>(Size{10}, Value{Zero<T>});
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
   E.resize(100);
   E.resize_forget(100);

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


int main() {
   TEST_ALL_TYPES(array1D);
   TEST_ALL_TYPES(fixed_array1D);
   return EXIT_SUCCESS;
}

