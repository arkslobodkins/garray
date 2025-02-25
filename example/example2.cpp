#include <cstdlib>
#include <strict.hpp>

using namespace spp;

void f(){}

template <typename T> concept Good
    =  SameAs<int, BuiltinTypeOf<T>>;

// example 2 puts more emphasis on slicing features of 1-d array class.
int main() {
   Array1D<double> A = random(Size{10}, Low{-1._sd}, High{1._sd});
   std::cout << A.remove(last - 1) << std::endl;

   auto slice_1A = A(seq(First{0}, Endmost{2}));
   auto slice_1B = A(seq(0, 2));

   auto slice_2A = A(seqN(First{0}, Size{3}));
   auto slice_2B = A(seqN(0, 3));

   auto slice_3A = A(place::even)(place::even);
   auto slice_3B = A(place::skipN(4));

   auto slice_4 = A(place::reverse) *= 2._sd;
   auto slice_5 = A({0, 2, 4}) = A({6, 6, 6});

   auto slice_6 = in_closed_range(A, Low{-0.5}, High{0.5});
   A.remove(slice_6.get_slice());

   auto dst1 = norm2(A(place::even));
   // Generate evaluates on the fly, no memory allocation is performed.
   auto dst2 = norm2(generate(A(place::even), [](auto x) { return x > 0._sd ? x : squares(x); }));

   // Suppress unused variable warnings.
   []<typename... Args>([[maybe_unused]] Args&&...) {}(slice_1A,
                                                       slice_1B,
                                                       slice_2A,
                                                       slice_2B,
                                                       slice_3A,
                                                       slice_3B,
                                                       slice_4,
                                                       slice_5,
                                                       slice_6,
                                                       dst1,
                                                       dst2);

//   std::cout << expr::UnaryOperation<int, decltype(f)> << std::endl;
   std::cout << Good<int> << std::endl;
   return EXIT_SUCCESS;
}

