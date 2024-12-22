#include <cstdlib>
#include <iostream>
#include <stdexcept>

#include <strict.hpp>


// example4 generates random generalized polynomials and evaluates them
// using gpolynomial and stable gpolynomial functions.
// Maximum and average relative errors are compared.
int main() {
   using namespace spp;

   Strict128 max_err{}, max_err_stable{};
   Strict128 sum_err{}, sum_err_stable{};
   constexpr auto its{10'000_si};
   Array1D<float64> C(100);

   for([[maybe_unused]]auto i : irange(its)) {
      const Strict64 x = rands(-1._SD, 1._SD);
      random(C, -1._SD, 1._SD);

      const Strict64 p = polynomial(C, x);
      const Strict64 pst = stable_polynomial(C, x);
      const Strict128 pq = polynomial(array_cast<float128>(C), x.sq());

      const auto rel = rel_error(p.sq(), pq);
      const auto rel_stable = rel_error(pst.sq(), pq);
      if(rel && rel_stable) {
         sum_err += *rel;
         max_err = maxs(max_err, *rel);
         sum_err_stable += *rel_stable;
         max_err_stable = maxs(max_err_stable, *rel_stable);
      } else {
         throw std::runtime_error{"relative error could not be computed"};
      }
   }

   format.precision(16);

   using std::cout, std::endl;
   cout << "maximum relative error using polynomial: " << max_err << endl;
   cout << "average relative error using polynomial: " << sum_err / its.sq() << endl;
   cout << "maximum relative error using stable_polynomial: " << max_err_stable << endl;
   cout << "average relative error using stable_polynomial: " << sum_err_stable / its.sq() << endl
        << endl;

   return EXIT_SUCCESS;
}
