#include <cstdlib>
#include "../src/strict_array.hpp"

using namespace strict_array;

int main()
{
   Array<double> x = random<double>(Size{10}, Low{-1.}, High{1.});
   Array<double> y = random<double>(Size{10}, Low{-1.}, High{1.});
   Array<double> z = x + y;

   auto slice = within_cond(z, [](auto x) { return x > 0.; } );
   if(slice) {
      *slice *= 100.;
   }

   z = e_unit<double>(0, 10);
   z = sequence<double>(Size{10}, Start{-10.}, Incr{1.});
   z = {-5., -4., -3., -2., -1., 0., 1., 2., 3., 4.};
   z[seqN(0, 5)] = 5. * x[seqN(0, 5)] + 10. * y[seqN(0, 5)];
   z = abs(z);

   auto all_pos = all_positive(z);
   auto all_neg = all_negative(z);
   auto all_greater = all_satisfy(z, [](auto x) { return x > -0.5; });
   auto any_greater = any_satisfy(z, [](auto x) { return x > -0.5; });

   std::cout << z << std::endl;
   print(z, "z");
   std::cout << std::endl;

   return EXIT_SUCCESS;
}
