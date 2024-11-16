#include <cstdlib>
#include <strict.hpp>

using namespace spp;


struct Z {
   int x{};
   double y{};
   double z{};
};

int main() {
   format.scientific(false).precision(4);
   Array1D<double> x(10);
   FixedArray1D<double, 10> y;
   random(x, y(place::even));
   printn(x, y);

   Array2D<double> X(3, 4);
   for(auto [i, j] : IndexRange2D{X}) {
      X(i, j) = (i + j).sd();
   }
   printn(X);


   for(auto c : ColIt{X}) {
      c = 1._sd;
   }


   random(X);
   auto Z = X + 1._sd;
   printn(X, Z, Z.view1D());
   printn(merge(X.view1D(), Z.view1D(), X.view1D()));


   return EXIT_SUCCESS;
}
