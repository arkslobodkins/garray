// Arkadijs Slobodkins, 2023


#pragma once


#include <cmath>

#include "config.hpp"
#include "error.hpp"
#include "strict_traits.hpp"
#include "strict_val.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> signs(Strict<T> x) {
   return strict_cast<T>((T(0) < T{x}) - (T{x} < T(0)));
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> abss(Strict<T> x) {
   if constexpr(UnsignedInteger<T>) {
      return x;
   } else {
      return x > Zero<T> ? x : -x;
   }
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> mins(Strict<T> x, Strict<T> y) {
   return x < y ? x : y;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> maxs(Strict<T> x, Strict<T> y) {
   return x > y ? x : y;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> invs(Strict<T> x) {
   return One<T> / x;
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> exps(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::exp(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{expq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> logs(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::log(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{logq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> log2s(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::log2(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{log2q(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> log10s(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::log10(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{log10q(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> sqrts(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::sqrt(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{sqrtq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> cbrts(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::cbrt(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{cbrtq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> sins(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::sin(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{sinq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> coss(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::cos(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{cosq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> tans(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::tan(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{tanq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> floors(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::floor(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{floorq(T{x})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> ceils(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::ceil(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{ceilq(T{x})};
   }
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isinfs(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return StrictBool{std::isinf(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      // Convert to bool first because int is returned.
      return StrictBool{static_cast<bool>(isinfq(T{x}))};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isnans(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return StrictBool{std::isnan(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      // Convert to bool first because int is returned.
      return StrictBool{static_cast<bool>(isnanq(T{x}))};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 StrictBool isfinites(Strict<T> x) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return StrictBool{std::isfinite(T{x})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      // Convert to bool first because int is returned.
      return StrictBool{static_cast<bool>(finiteq(T{x}))};
   }
#endif
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> squares(Strict<T> x) {
   return x * x;
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> cubes(Strict<T> x) {
   return x * x * x;
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> pows(Strict<T> x, Strict<T> pw) {
   ASSERT_STRICT_DEBUG(T{pw} >= 0);
   T res{1}, pr{T{pw}}, xr{T{x}};
   for(;;) {
      if(pr & 1) {
         res *= xr;
      }
      pr >>= 1;
      if(!pr) {
         break;
      }
      xr *= xr;
   }
   return Strict{res};
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2026 Strict<T> pows(Strict<T> x, Strict<T> pw) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::pow(T{x}, T{pw})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{powq(T{x}, T{pw})};
   }
#endif
}


template <Floating T>
STRICT_NODISCARD_CONSTEXPR_INLINE_2023 Strict<T> fmas(Strict<T> x, Strict<T> y, Strict<T> z) {
#ifdef STRICT_QUAD_PRECISION
   if constexpr(StandardFloating<T>) {
#endif
      return Strict<T>{std::fma(T{x}, T{y}, T{z})};
#ifdef STRICT_QUAD_PRECISION
   } else {
      return Strict<T>{fmaq(T{x}, T{y}, T{z})};
   }
#endif
}


}  // namespace spp

