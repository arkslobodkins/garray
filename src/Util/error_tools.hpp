// Arkadijs Slobodkins, 2023


#pragma once


#include <limits>
#include <optional>

#include "../ArrayCommon/array_auxiliary.hpp"
#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../array_ops.hpp"


namespace spp {


template <Floating T>
struct DefaultTol {
   static constexpr Strict<T> value{100 * std::numeric_limits<T>::epsilon()};
};


#ifdef STRICT_QUAD_PRECISION
template <>
struct DefaultTol<float128> {
   static constexpr Strict value{100 * FLT128_EPSILON};
};
#endif


template <Floating T>
STRICT_CONSTEXPR_INLINE StrictBool within_tol_abs(Strict<T> x, Strict<T> y,
                                                  Strict<T> tol = DefaultTol<T>::value) {
   ASSERT_STRICT_DEBUG(tol >= Zero<T>);
   return abss(x - y) <= tol;
}


template <Floating T>
STRICT_CONSTEXPR_INLINE StrictBool within_tol_rel(Strict<T> x, Strict<T> y,
                                                  Strict<T> tol = DefaultTol<T>::value,
                                                  Strict<T> near_zero = DefaultTol<T>::value) {
   ASSERT_STRICT_DEBUG(tol >= Zero<T>);
   ASSERT_STRICT_DEBUG(near_zero >= Zero<T>);

   auto abs1 = abss(x);
   auto abs2 = abss(y);
   if(maxs(abs1, abs2) <= near_zero) {
      return true_sb;
   }
   return abss(x - y) / maxs(abs1, abs2) <= tol;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <FloatingBaseType Base1, FloatingBaseType Base2>
STRICT_CONSTEXPR StrictBool within_tol_abs(const Base1& A1, const Base2& A2,
                                           ValueTypeOf<Base1> tol
                                           = DefaultTol<RealTypeOf<Base1>>::value) {
   static_assert(same_dimension<Base1, Base2>());
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   return all_of(A1, A2, [tol](const auto& x, const auto& y) { return within_tol_abs(x, y, tol); });
}


template <FloatingBaseType Base1, FloatingBaseType Base2>
STRICT_CONSTEXPR StrictBool within_tol_rel(const Base1& A1, const Base2& A2,
                                           ValueTypeOf<Base1> tol
                                           = DefaultTol<RealTypeOf<Base1>>::value,
                                           ValueTypeOf<Base1> near_zero
                                           = DefaultTol<RealTypeOf<Base1>>::value) {
   static_assert(same_dimension<Base1, Base2>());
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   return all_of(A1, A2, [tol, near_zero](const auto& x, const auto& y) {
      return within_tol_rel(x, y, tol, near_zero);
   });
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Floating T>
STRICT_CONSTEXPR Strict<T> abs_error(Strict<T> approx, Strict<T> exact) {
   return abss(approx - exact);
}


template <Floating T>
STRICT_CONSTEXPR std::optional<Strict<T>> rel_error(Strict<T> approx, Strict<T> exact) {
   if(approx == Zero<T> || exact == Zero<T>) {
      return {std::nullopt};
   }
   return {abss((approx - exact) / exact)};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <FloatingBaseType Base1, FloatingBaseType Base2>
STRICT_CONSTEXPR ValueTypeOf<Base1> max_abs_error(const Base1& approx, const Base2& exact) {
   static_assert(same_dimension<Base1, Base2>());
   ASSERT_STRICT_DEBUG(!approx.empty());
   ASSERT_STRICT_DEBUG(same_size(approx, exact));
   return norm_inf(approx - exact);
}


template <FloatingBaseType Base1, FloatingBaseType Base2>
STRICT_NODISCARD_CONSTEXPR std::optional<ValueTypeOf<Base1>> max_rel_error(const Base1& approx,
                                                                           const Base2& exact) {
   static_assert(same_dimension<Base1, Base2>());
   ASSERT_STRICT_DEBUG(!approx.empty());
   ASSERT_STRICT_DEBUG(same_size(approx, exact));

   if(has_zero(approx) || has_zero(exact)) {
      return {std::nullopt};
   }
   return {norm_inf((approx - exact) / exact)};
}


}  // namespace spp
