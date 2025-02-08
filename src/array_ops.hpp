// Arkadijs Slobodkins, 2023


#pragma once


#include <algorithm>
#include <concepts>
#include <iterator>
#include <memory>
#include <random>
#include <tuple>
#include <utility>
#include <vector>

#include "ArrayCommon/array_auxiliary.hpp"
#include "ArrayCommon/array_traits.hpp"
#include "Expr/expr.hpp"
#include "StrictCommon/strict_common.hpp"
#include "derived1D.hpp"


// Generic functions that work on Arrays, SliceArrays, and their expression templates.
namespace spp {


namespace detail {


template <typename Base, typename F> concept ForCallable
    = std::invocable<F, ValueTypeOf<Base>>
   && SameAs<void, decltype(std::declval<F>()(ValueTypeOf<Base>{}))>;


template <typename Base, typename F> concept CallableArgs1
    = std::invocable<F, ValueTypeOf<Base>>
   && SameAs<StrictBool, decltype(std::declval<F>()(ValueTypeOf<Base>{}))>;


template <typename Base1, typename Base2, typename F> concept CallableArgs2
    = std::invocable<F, ValueTypeOf<Base1>, ValueTypeOf<Base2>>
   && SameAs<StrictBool, decltype(std::declval<F>()(ValueTypeOf<Base1>{}, ValueTypeOf<Base2>{}))>;


template <typename Base, typename F> concept SortableArgs
    = std::invocable<F, ValueTypeOf<Base>, ValueTypeOf<Base>>
   && SameAs<StrictBool, decltype(std::declval<F>()(ValueTypeOf<Base>{}, ValueTypeOf<Base>{}))>;


}  // namespace detail


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> sum(const Base& A, ValueTypeOf<Base> empty_default = {});


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> prod(const Base& A, ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> mean(const Base& A, ValueTypeOf<Base> empty_default = {});


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> min(const Base& A, ValueTypeOf<Base> empty_default = {});


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> max(const Base& A, ValueTypeOf<Base> empty_default = {});


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR std::pair<index_t, ValueTypeOf<Base>> min_index(
    const Base& A, std::pair<index_t, ValueTypeOf<Base>> empty_default = {-1_sl, {}});


template <TwoDimRealBaseType Base>
STRICT_CONSTEXPR std::tuple<index_t, index_t, ValueTypeOf<Base>> min_index(
    const Base& A,
    std::tuple<index_t, index_t, ValueTypeOf<Base>> empty_default = {-1_sl, -1_sl, {}});


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR std::pair<index_t, ValueTypeOf<Base>> max_index(
    const Base& A, std::pair<index_t, ValueTypeOf<Base>> empty_default = {-1_sl, {}});


template <TwoDimRealBaseType Base>
STRICT_CONSTEXPR std::tuple<index_t, index_t, ValueTypeOf<Base>> max_index(
    const Base& A,
    std::tuple<index_t, index_t, ValueTypeOf<Base>> empty_default = {-1_sl, -1_sl, {}});


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool all_finite(const Base& A, StrictBool empty_default = true_sb);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_inf(const Base& A, StrictBool empty_default = false_sb);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_nan(const Base& A, StrictBool empty_default = false_sb);


template <RealBaseType Base1, RealBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR ValueTypeOf<Base1> dot_prod(const Base1& A1, const Base2& A2,
                                             ValueTypeOf<Base1> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> norm_inf(const Base& A, ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> norm1(const Base& A, ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> norm1_scaled(const Base& A,
                                                ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm2(const Base& A, ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm2_scaled(const Base& A,
                                                     ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm_lp(const Base& A, ImplicitInt lp,
                                                ValueTypeOf<Base> empty_default = {});


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm_lp_scaled(const Base& A, ImplicitInt lp,
                                                       ValueTypeOf<Base> empty_default = {});


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> polynomial(const Base& coeffs, ValueTypeOf<Base> x,
                                              ValueTypeOf<Base> empty_default = {});


template <OneDimRealBaseType Base1, OneDimSignedIntegerBaseType Base2>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base1> polynomial(const Base1& coeffs, ValueTypeOf<Base1> x,
                                                    const Base2& powers,
                                                    ValueTypeOf<Base1> empty_default = {});


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2, OneDimSignedIntegerBaseType Base3>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base1> gpolynomial(const Base1& coeffs, const Base2& X,
                                                     const Base3& powers,
                                                     ValueTypeOf<Base1> empty_default = {});


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool has_zero(const Base& A, StrictBool empty_default = false_sb);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_zeros(const Base& A, StrictBool empty_default = false_sb);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_pos(const Base& A, StrictBool empty_default = false_sb);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_neg(const Base& A, StrictBool empty_default = false_sb);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_pos(const Base& A, StrictBool empty_default = false_sb);


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_neg(const Base& A, StrictBool empty_default = false_sb);


template <BaseType Base>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, ValueTypeOf<Base> x,
                                    StrictBool empty_default = false_sb);


template <BaseType Base>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, Value<RealTypeOf<Base>> x,
                                    StrictBool empty_default = false_sb);


template <BaseType Base, typename F>
   requires detail::CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, F f, StrictBool empty_default = false_sb);


template <BaseType Base1, BaseType Base2, typename F>
   requires(detail::CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool none_of(const Base1& A1, const Base2& A2, F f,
                                    StrictBool empty_default = false_sb);


template <BaseType Base>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, ValueTypeOf<Base> x,
                                   StrictBool empty_default = false_sb);


template <BaseType Base>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, Value<RealTypeOf<Base>> x,
                                   StrictBool empty_default = false_sb);


template <BaseType Base, typename F>
   requires detail::CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, F f, StrictBool empty_default = false_sb);


template <BaseType Base1, BaseType Base2, typename F>
   requires(detail::CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool any_of(const Base1& A1, const Base2& A2, F f,
                                   StrictBool empty_default = false_sb);


template <BaseType Base>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, ValueTypeOf<Base> x,
                                   StrictBool empty_default = false_sb);


template <BaseType Base>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, Value<RealTypeOf<Base>> x,
                                   StrictBool empty_default = false_sb);


template <BaseType Base, typename F>
   requires detail::CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, F f, StrictBool empty_default = false_sb);


template <BaseType Base1, BaseType Base2, typename F>
   requires(detail::CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool all_of(const Base1& A1, const Base2& A2, F f,
                                   StrictBool empty_default = false_sb);


template <RealBaseType Base>
STRICT_CONSTEXPR_2023 std::unique_ptr<RealTypeOf<Base>[]> blas_array(const Base& A);


// Returns a slice array of all elements that are inside the interval (low, high).
// Only applies to one-dimensional types.
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high);


// Same as above.
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, Low<RealTypeOf<Base>> low,
                                    High<RealTypeOf<Base>> high);


// Returns a slice array of all elements that are inside the interval [low, high]
// Only applies to one-dimensional types.
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high);


// Same as above.
template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, Low<RealTypeOf<Base>> low,
                                      High<RealTypeOf<Base>> high);


// Returns a slice array of all elements that evaluate true for f.
// Only applies to one-dimensional types.
template <typename Base, typename F>
   requires(OneDimRealBaseType<RemoveRef<Base>> && detail::CallableArgs1<Base, F>
            && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_cond_range(Base&& A, F f);


template <typename Base, typename F>
   requires(detail::ForCallable<Base, F>)
STRICT_CONSTEXPR void for_each(Base&& A, F f);


template <typename Base, typename F>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && detail::SortableArgs<Base, F>
            && !detail::ArrayRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort(Base&& A, F f);


template <typename Base>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && !detail::ArrayRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_increasing(Base&& A);


template <typename Base>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && !detail::ArrayRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_decreasing(Base&& A);


template <typename Base>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && !detail::ArrayRealTypeRvalue<Base>)
void shuffle(Base&& A);


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> sum(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   ValueTypeOf<Base> s = A.un(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      s += A.un(i);
   }
   return s;
}


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> prod(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   auto p = A.un(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      p *= A.un(i);
   }
   return p;
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> mean(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return sum(A) / value_type_cast<Base>(A.size());
}


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> min(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   auto min_elem = A.un(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      min_elem = mins(A.un(i), min_elem);
   }
   return min_elem;
}


template <RealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> max(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   auto max_elem = A.un(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      max_elem = maxs(A.un(i), max_elem);
   }
   return max_elem;
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR std::pair<index_t, ValueTypeOf<Base>> min_index(
    const Base& A, std::pair<index_t, ValueTypeOf<Base>> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   std::pair<index_t, ValueTypeOf<Base>> min = {0_sl, A.un(0)};
   for(index_t i = 1_sl; i < A.size(); ++i) {
      if(auto xi = A.un(i); xi < min.second) {
         min = {i, xi};
      }
   }
   return min;
}


template <TwoDimRealBaseType Base>
STRICT_CONSTEXPR std::tuple<index_t, index_t, ValueTypeOf<Base>> min_index(
    const Base& A, std::tuple<index_t, index_t, ValueTypeOf<Base>> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   std::tuple<index_t, index_t, ValueTypeOf<Base>> min = {0_sl, 0_sl, A.un(0, 0)};
   for(index_t i = 0_sl; i < A.rows(); ++i) {
      for(index_t j = 0_sl; j < A.cols(); ++j) {
         if(auto xij = A.un(i, j); xij < std::get<2>(min)) {
            min = {i, j, xij};
         }
      }
   }
   return min;
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR std::pair<index_t, ValueTypeOf<Base>> max_index(
    const Base& A, std::pair<index_t, ValueTypeOf<Base>> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   std::pair<index_t, ValueTypeOf<Base>> max = {0_sl, A.un(0)};
   for(index_t i = 1_sl; i < A.size(); ++i) {
      if(auto xi = A.un(i); xi > max.second) {
         max = {i, xi};
      }
   }
   return max;
}


template <TwoDimRealBaseType Base>
STRICT_CONSTEXPR std::tuple<index_t, index_t, ValueTypeOf<Base>> max_index(
    const Base& A, std::tuple<index_t, index_t, ValueTypeOf<Base>> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   std::tuple<index_t, index_t, ValueTypeOf<Base>> max = {0_sl, 0_sl, A.un(0, 0)};
   for(index_t i = 0_sl; i < A.rows(); ++i) {
      for(index_t j = 0_sl; j < A.cols(); ++j) {
         if(auto xij = A.un(i, j); xij > std::get<2>(max)) {
            max = {i, j, xij};
         }
      }
   }
   return max;
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool all_finite(const Base& A, StrictBool empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return all_of(A, [](auto x) { return isfinites(x); });
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_inf(const Base& A, StrictBool empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return any_of(A, [](auto x) { return isinfs(x); });
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2023 StrictBool has_nan(const Base& A, StrictBool empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return any_of(A, [](auto x) { return isnans(x); });
}


template <RealBaseType Base1, RealBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR ValueTypeOf<Base1> dot_prod(const Base1& A1, const Base2& A2,
                                             ValueTypeOf<Base1> empty_default) {
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   if(A1.empty()) {
      return empty_default;
   }
   return sum(A1 * A2);
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> norm_inf(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return max(abs(A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> norm1(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return sum(abs(A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> norm1_scaled(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return mean(abs(A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm2(const Base& A, ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return sqrts(dot_prod(A, A));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm2_scaled(const Base& A,
                                                     ValueTypeOf<Base> empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return norm2(A) / sqrts(value_type_cast<Base>(A.size()));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm_lp(const Base& A, ImplicitInt lp,
                                                ValueTypeOf<Base> empty_default) {
   ASSERT_STRICT_DEBUG(lp.get() > 0_sl);
   if(A.empty()) {
      return empty_default;
   }
   ValueTypeOf<Base> s = sum(abs(pow_int(A, lp.get())));
   return pows(s, invs(value_type_cast<Base>(lp.get())));
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base> norm_lp_scaled(const Base& A, ImplicitInt lp,
                                                       ValueTypeOf<Base> empty_default) {
   ASSERT_STRICT_DEBUG(lp.get() > 0_sl);
   if(A.empty()) {
      return empty_default;
   }
   return norm_lp(A, lp)
        / pows(value_type_cast<Base>(A.size()), invs(value_type_cast<Base>(lp.get())));
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR ValueTypeOf<Base> polynomial(const Base& coeffs, ValueTypeOf<Base> x,
                                              ValueTypeOf<Base> empty_default) {
   if(coeffs.empty()) {
      return empty_default;
   }
   auto s = coeffs[last];
   for(index_t i = coeffs.size() - 2_sl; i >= 0_sl; --i) {
      s = coeffs.un(i) + s * x;
   }
   return s;
}


template <OneDimRealBaseType Base1, OneDimSignedIntegerBaseType Base2>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base1> polynomial(const Base1& coeffs, ValueTypeOf<Base1> x,
                                                    const Base2& powers,
                                                    ValueTypeOf<Base1> empty_default) {
   ASSERT_STRICT_DEBUG(same_size(coeffs, powers));
   if(coeffs.empty()) {
      return empty_default;
   }
   ASSERT_STRICT_DEBUG(all_non_neg(powers));
   auto X = const1D(Size{coeffs.size()}, Value{x});
   return gpolynomial(coeffs, X, powers);
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2, OneDimSignedIntegerBaseType Base3>
STRICT_CONSTEXPR_2026 ValueTypeOf<Base1> gpolynomial(const Base1& coeffs, const Base2& X,
                                                     const Base3& powers,
                                                     ValueTypeOf<Base1> empty_default) {
   ASSERT_STRICT_DEBUG(same_size(coeffs, X, powers));
   if(coeffs.empty()) {
      return empty_default;
   }
   ASSERT_STRICT_DEBUG(all_non_neg(powers));

   ValueTypeOf<Base1> z{};
   for(index_t i = 0_sl; i < X.size(); ++i) {
      z += coeffs.un(i) * pows_int(X.un(i), powers.un(i));
   }
   return z;
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool has_zero(const Base& A, StrictBool empty_default) {
   auto is_zero = []<Real T>(const Strict<T>& x) { return x == Zero<T>; };
   return any_of(A, is_zero, empty_default);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_zeros(const Base& A, StrictBool empty_default) {
   auto is_zero = []<Real T>(const Strict<T>& x) { return x == Zero<T>; };
   return all_of(A, is_zero, empty_default);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_pos(const Base& A, StrictBool empty_default) {
   auto is_positive = []<Real T>(const Strict<T>& x) { return x > Zero<T>; };
   return all_of(A, is_positive, empty_default);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_neg(const Base& A, StrictBool empty_default) {
   auto is_negative = []<Real T>(const Strict<T>& x) { return x < Zero<T>; };
   return all_of(A, is_negative, empty_default);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_pos(const Base& A, StrictBool empty_default) {
   auto is_non_positive = []<Real T>(const Strict<T>& x) { return x <= Zero<T>; };
   return all_of(A, is_non_positive, empty_default);
}


template <RealBaseType Base>
STRICT_CONSTEXPR StrictBool all_non_neg(const Base& A, StrictBool empty_default) {
   auto is_non_negative = []<Real T>(const Strict<T>& x) { return x >= Zero<T>; };
   return all_of(A, is_non_negative, empty_default);
}


template <BaseType Base>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, ValueTypeOf<Base> x, StrictBool empty_default) {
   return none_of(A, [x](auto y) { return y == x; }, empty_default);
}


template <BaseType Base>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, Value<RealTypeOf<Base>> x,
                                    StrictBool empty_default) {
   return none_of(A, x.get(), empty_default);
}


template <BaseType Base, typename F>
   requires detail::CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool none_of(const Base& A, F f, StrictBool empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   return !any_of(A, f);
}


template <BaseType Base1, BaseType Base2, typename F>
   requires(detail::CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool none_of(const Base1& A1, const Base2& A2, F f,
                                    StrictBool empty_default) {
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   if(A1.empty()) {
      return empty_default;
   }
   return !any_of(A1, A2, f);
}


template <BaseType Base>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, ValueTypeOf<Base> x, StrictBool empty_default) {
   return any_of(A, [x](auto y) { return y == x; }, empty_default);
}


template <BaseType Base>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, Value<RealTypeOf<Base>> x,
                                   StrictBool empty_default) {
   return any_of(A, x.get(), empty_default);
}


template <BaseType Base, typename F>
   requires detail::CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool any_of(const Base& A, F f, StrictBool empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   for(index_t i = 0_sl; i < A.size(); ++i) {
      if(f(A.un(i))) {
         return true_sb;
      }
   }
   return false_sb;
}


template <BaseType Base1, BaseType Base2, typename F>
   requires(detail::CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool any_of(const Base1& A1, const Base2& A2, F f,
                                   StrictBool empty_default) {
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   if(A1.empty()) {
      return empty_default;
   }
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      if(f(A1.un(i), A2.un(i))) {
         return true_sb;
      }
   }
   return false_sb;
}


template <BaseType Base>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, ValueTypeOf<Base> x, StrictBool empty_default) {
   return all_of(A, [x](auto y) { return y == x; }, empty_default);
}


template <BaseType Base>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, Value<RealTypeOf<Base>> x,
                                   StrictBool empty_default) {
   return all_of(A, x.get(), empty_default);
}


template <BaseType Base, typename F>
   requires detail::CallableArgs1<Base, F>
STRICT_CONSTEXPR StrictBool all_of(const Base& A, F f, StrictBool empty_default) {
   if(A.empty()) {
      return empty_default;
   }
   for(index_t i = 0_sl; i < A.size(); ++i) {
      if(!f(A.un(i))) {
         return false_sb;
      }
   }
   return true_sb;
}


template <BaseType Base1, BaseType Base2, typename F>
   requires(detail::CallableArgs2<Base1, Base2, F> && same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool all_of(const Base1& A1, const Base2& A2, F f,
                                   StrictBool empty_default) {
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   if(A1.empty()) {
      return empty_default;
   }
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      if(!f(A1.un(i), A2.un(i))) {
         return false_sb;
      }
   }
   return true_sb;
}


template <RealBaseType Base>
STRICT_CONSTEXPR_2023 std::unique_ptr<RealTypeOf<Base>[]> blas_array(const Base& A) {
   if(A.empty()) {
      return nullptr;
   }

   using real_type = RealTypeOf<Base>;
   auto blas_array = std::make_unique<real_type[]>(to_size_t(A.size()));
   auto* ptr = blas_array.get();
   for(index_t i = 0_sl; i < A.size(); ++i) {
      ptr[i.val()] = A.un(i).val();
   }
   return blas_array;
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return in_cond_range(std::forward<Base>(A), [low, high](auto x) { return x > low && x < high; });
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_open_range(Base&& A, Low<RealTypeOf<Base>> low,
                                    High<RealTypeOf<Base>> high) {
   return in_open_range(std::forward<Base>(A), low.get(), high.get());
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   return in_cond_range(std::forward<Base>(A),
                        [low, high](auto x) { return x >= low && x <= high; });
}


template <typename Base>
   requires(OneDimRealBaseType<RemoveRef<Base>> && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_closed_range(Base&& A, Low<RealTypeOf<Base>> low,
                                      High<RealTypeOf<Base>> high) {
   return in_closed_range(std::forward<Base>(A), low.get(), high.get());
}


template <typename Base, typename F>
   requires(OneDimRealBaseType<RemoveRef<Base>> && detail::CallableArgs1<Base, F>
            && !detail::ArrayOneDimRealTypeRvalue<Base>)
STRICT_CONSTEXPR auto in_cond_range(Base&& A, F f) {
   std::vector<ImplicitInt> indexes;
   std::ranges::copy_if(irange(A), std::back_inserter(indexes), [&](auto i) { return f(A.un(i)); });
   return A(indexes);
}


// !IsConst is not used since A might not be modified.
// Removed NonConstBaseType requirement so that for each can be
// called for expression templates as well or other constant objects.
template <typename Base, typename F>
   requires(detail::ForCallable<Base, F>)
STRICT_CONSTEXPR void for_each(Base&& A, F f) {
   for(index_t i = 0_sl; i < A.size(); ++i) {
      f(A.un(i));
   }
}


template <typename Base, typename F>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && detail::SortableArgs<Base, F>
            && !detail::ArrayRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort(Base&& A, F f) {
   std::sort(A.begin(), A.end(), f);
}


template <typename Base>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && !detail::ArrayRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_increasing(Base&& A) {
   std::sort(A.begin(), A.end(), [](const auto& a, const auto& b) { return a < b; });
}


template <typename Base>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && !detail::ArrayRealTypeRvalue<Base>)
STRICT_CONSTEXPR void sort_decreasing(Base&& A) {
   std::sort(A.begin(), A.end(), [](const auto& a, const auto& b) { return a > b; });
}


template <typename Base>
   requires(detail::NonConstBaseType<RemoveRef<Base>> && !detail::ArrayRealTypeRvalue<Base>)
void shuffle(Base&& A) {
   std::mt19937 g{std::random_device{}()};
   std::shuffle(A.begin(), A.end(), g);
}


}  // namespace spp
