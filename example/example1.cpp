#include <cassert>
#include <cstdlib>
#include <utility>

#include <strict.hpp>

using namespace spp;


template <typename... Args>
constexpr OneDimBaseType auto Add(Args&&... args) {
   assert(same_size(args...));
   return (... + std::forward<Args>(args));
}


template <typename... Args>
constexpr OneDimBaseType auto MergeAbs(Args&&... args) {
   return abs(merge(std::forward<Args>(args)...));
}


// example1 introduces 1-d array classes and performs some operations on them.
int main() {
   Array1D<double> x = random(Size{5}, Low{-1.}, High{1.});
   FixedArray1D<double, 5> y = random(5, -1._sd, 1._sd);

   // Rotate elements to the right and set first element to 100.
   x = merge(100._sd, exclude(y, last));

   auto z1 = Add(x, y, x + y);
   auto z2 = MergeAbs(x, y, x + y);

   // Add(x, y, Array1D<double>(x + y)); // Won't compile, dangling temporary!!!
   // MergeAbs(x, y, Array1D<double>(x + y)); // Won't compile, dangling temporary!!!

   auto s1 = sum(Add(x, y, Array1D<double>(x + y).lval()));       // Ok, can be used as lvalue.
   auto s2 = sum(MergeAbs(x, y, Array1D<double>(x + y).lval()));  // Ok, can be used as lvalue.

   // Suppress unused variable warnings.
   []<typename... Args>([[maybe_unused]] Args&&...) {}(z1, z2, s1, s2);
   return EXIT_SUCCESS;
}
