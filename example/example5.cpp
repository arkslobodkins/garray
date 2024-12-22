#include <cstdlib>
#include <stdexcept>
#include <tuple>

#include <strict.hpp>

using namespace spp;


// example5 solves first order ODE y' = y, y(t0) = y0, for any t0 and y0
// using trapezoidal method. Exact solution is y(t) = y0/exp(t0) exp(t).


consteval auto ode_init() {
   constexpr StrictLong nsteps = pows(2_sl, 24_sl);
   constexpr Strict128 h = invs(nsteps.sq());
   constexpr Strict128 t_init{0.Q};
   constexpr Strict128 y_init{1.Q};  // y(t_init), t_init is not necessarily 0.
   return std::tuple{nsteps, h, t_init, y_init};
}


Array1D<float128> ode_solve(StrictLong nsteps, Strict128 h, Strict128 t_init, Strict128 y_init) {
   ASSERT_STRICT_DEBUG(nsteps > 0_sl);
   ASSERT_STRICT_DEBUG(h > 0._sq);

   Array1D<float128> y(nsteps + 1_sl);
   y[0] = y_init;

   // Trapezoidal method.
   for(const auto i : irange_m1(y)) {
      y[i + 1_sl] = (y[i] + y[i] * h / 2._sq) / (1._sq - h / 2._sq);
   }
   return y;
}


Array1D<float128> ode_exact(StrictLong nsteps, Strict128 h, Strict128 t_init, Strict128 y_init) {
   ASSERT_STRICT_DEBUG(nsteps > 0_sl);
   ASSERT_STRICT_DEBUG(h > 0._sq);

   Array1D<float128> y(nsteps + 1_sl);
   Strict128 c = y_init / exps(t_init);

#pragma omp parallel for default(none) shared(c, nsteps, h, t_init, y)
   for(auto i = 0L; i < nsteps.val() + 1; ++i) {
      auto t_cur = t_init + strict_cast<float128>(i) * h;
      y[i] = c * exps(t_cur);
   }
   return y;
}


int main() {
   config_info();

   auto [nsteps, h, t_init, y_init] = ode_init();
   auto y = ode_solve(nsteps, h, t_init, y_init);
   auto y_exact = ode_exact(nsteps, h, t_init, y_init);

   if(auto rel_error = max_rel_error(y, y_exact)) {
      std::cout << "maximum relative error: " << *rel_error << std::endl;
   } else {
      throw std::runtime_error{"maximum relative error could not be computed"};
   }

   return EXIT_SUCCESS;
}
