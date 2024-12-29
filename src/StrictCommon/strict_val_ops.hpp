// Arkadijs Slobodkins, 2023


#pragma once


#include <algorithm>
#include <cstdlib>
#include <memory>

#include "auxiliary_types.hpp"
#include "config.hpp"
#include "error.hpp"
#include "strict_literals.hpp"
#include "strict_math.hpp"
#include "strict_traits.hpp"
#include "strict_val.hpp"
#include "use.hpp"


namespace spp {


STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long int> factorial(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);
   index_t nf = n.get();
   return nf == 0_sl ? 1_sl : nf * factorial(nf - 1_sl);
}


STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long int> binom_coeff(ImplicitInt n, ImplicitInt k) {
   ASSERT_STRICT_DEBUG(k.get() > -1_sl);
   ASSERT_STRICT_DEBUG(n.get() >= k.get());
   index_t d1 = n.get() - k.get();
   index_t d2 = k.get();

   index_t prod = 1_sl;
   for(index_t f1 = d1 + 1_sl, f2 = 1_sl; f1 <= d1 + d2; ++f1, ++f2) {
      prod = prod * f1 / f2;  // Can prove that (prod * f1) is divisible by f2.
   }
   return prod;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> clamps(Strict<T> x, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   if constexpr(Floating<T>) {
      ASSERT_STRICT_DEBUG(!isnans(x));
      ASSERT_STRICT_DEBUG(!isnans(low));
      ASSERT_STRICT_DEBUG(!isnans(high));
   }
   return Strict{std::clamp(x.val(), low.val(), high.val())};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> clamps(Strict<T> x, Low<T> low, High<T> high) {
   return clamps(x, low.get(), high.get());
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> pows_int(Strict<T> x, ImplicitInt p) {
   return pows(x, strict_cast<T>(p.get()));
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> fast_pows_int(Strict<T> x, const ImplicitInt p) {
   auto res = One<T>;
   auto power = abss(p.get());
   for(;;) {
      if(strict_cast<bool>(power & 1_sl)) {
         res *= x;
      }
      power >>= 1_sl;
      if(!strict_cast<bool>(power)) {
         break;
      }
      x *= x;
   }
   return p.get() >= 0_sl ? res : invs(res);
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> exps_int(ImplicitInt p) {
   return exps(strict_cast<T>(p.get()));
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_open(Strict<T> x, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return x > low && x < high;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_open(Value<T> x, Low<T> low, High<T> high) {
   return in_open(x.get(), low.get(), high.get());
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_closed(Strict<T> x, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return x >= low && x <= high;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool in_closed(Value<T> x, Low<T> low, High<T> high) {
   return in_closed(x.get(), low.get(), high.get());
}


template <Floating T>
STRICT_NODISCARD_INLINE use::StrictPair<T> two_sums(Strict<T> x, Strict<T> y) {
   volatile T r = x.val() + y.val();
   volatile T z = r - x.val();
   volatile T rz = r - z;
   volatile T yz = y.val() - z;
   volatile T xrz = x.val() - rz;
   T s = xrz + yz;
   return use::StrictPair<T>{r, s};
}


template <Floating T>
STRICT_NODISCARD_INLINE use::StrictPair<T> two_prods(Strict<T> x, Strict<T> y) {
   auto r = x * y;
   auto s = fmas(x, y, -r);
   return use::StrictPair<T>{r, s};
}


template <Floating T>
STRICT_NODISCARD_INLINE use::StrictPair<T> pow_prods(Strict<T> x, ImplicitInt p) {
   ASSERT_STRICT_DEBUG(p.get().val() > -1);
   auto pw = p.get().val();

   if(pw == 0) {
      return use::StrictPair<T>{T(1), T(0)};

   } else if(pw == 1) {
      return use::StrictPair<T>{x, T(0)};

   } else if(pw == 2) {
      return use::StrictPair<T>{two_prods(x, x)};

   } else if(pw == 3) {
      auto [r, s] = two_prods(x, x);
      auto [rp1, sp1] = two_prods(r, x);
      return use::StrictPair<T>{rp1, x * s + sp1};

   } else if(pw == 4) {
      auto [r, s] = two_prods(x, x);
      auto [r2, s2] = two_prods(r, r);
      return use::StrictPair<T>{r2, strict_cast<T>(2) * r * s + s2};

   } else if(pw == 5) {
      auto [r, s] = two_prods(x, x);
      auto [r2, s2] = two_prods(r, r);
      auto [rp1, sp1] = two_prods(r2, x);
      return use::StrictPair<T>{rp1, strict_cast<T>(2) * x * r * s + x * s2 + sp1};

   } else {
      auto [r, s] = two_prods(x, x);
      auto n = pw / 2;
      auto sz = n - 1;

      auto temp = std::make_unique<Strict<T>[]>(static_cast<std::size_t>(2 * sz));
      Strict<T>* rtemp = temp.get();
      Strict<T>* stemp = temp.get() + sz;
      rtemp[0] = One<T>;
      rtemp[1] = r;
      for(decltype(sz) i = 2; i < sz; ++i) {
         auto [rcur, scur] = two_prods(rtemp[i - 1], r);
         rtemp[i] = rcur;
         stemp[i - 2] = scur;
      }
      auto [rn_m1, sn_m1] = two_prods(rtemp[sz - 1], r);
      stemp[sz - 2] = sn_m1;
      auto [rn, sn] = two_prods(rn_m1, r);
      stemp[sz - 1] = sn;

      if(pw % 2 == 0) {  // Even degrees.
         Strict<T> sum_loc{};
         for(decltype(sz) i = 0; i < sz; ++i) {
            sum_loc += rtemp[i] * stemp[sz - 1 - i];
         }

         return use::StrictPair<T>{rn, strict_cast<T>(n) * rn_m1 * s + sum_loc};
      } else {  // Odd degrees.
         Strict<T> sum_loc{};
         for(decltype(sz) i = 0; i < sz; ++i) {
            sum_loc += rtemp[i] * stemp[sz - 1 - i];
         }

         auto [rn_p1, sn_p1] = two_prods(rn, x);
         return use::StrictPair<T>{rn_p1, strict_cast<T>(n) * x * rn_m1 * s + x * sum_loc + sn_p1};
      }
   }
}


}  // namespace spp
