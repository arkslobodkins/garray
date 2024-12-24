// Arkadijs Slobodkins, 2023


#pragma once


#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "../derived2D.hpp"
#include "functors.hpp"
#include "types.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
// Unary operations.
template <BaseType Base, typename F, bool copy_delete = false>
   requires expr::UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate(const Base& A, F f);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator~(const Base& A);


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator!(const Base& A);


template <RealBaseType Base>
STRICT_CONSTEXPR auto abs(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto exp(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log2(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log10(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sqrt(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cbrt(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sin(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cos(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto tan(const Base& A);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow(const Base& A, ValueTypeOf<Base> pw);


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow_int(const Base& A, ImplicitInt pw);


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto fast_pow_int(const Base& A, ImplicitInt pw);


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto inv(const Base& A);


template <Builtin T, BaseType Base>
STRICT_CONSTEXPR auto array_cast(const Base& A);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator~(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator!(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto abs(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto exp(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto log(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto log2(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto log10(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto sqrt(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto cbrt(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto sin(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto cos(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto tan(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto pow(Base&& A, ValueTypeOf<Base> pw) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto pow_int(Base&& A, ImplicitInt pw) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR auto fast_pow_int(Base&& A, ImplicitInt pw) = delete;


template <typename Base>
   requires detail::ArrayFloatingTypeRvalue<Base>
STRICT_CONSTEXPR auto inv(Base&& A) = delete;


template <Builtin T, typename Base>
   requires detail::ArrayTypeRvalue<Base>
STRICT_CONSTEXPR auto array_cast(Base&& A) = delete;


namespace detail {


template <BaseType Base, typename F>
   requires expr::UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate_random(const Base& A, F f, ValueTypeOf<Base> low,
                                      ValueTypeOf<Base> high) {
   using E = detail::RandUnaryExpr<Base, F>;

   if constexpr(OneDimBaseType<Base>) {
      return StrictArrayBase1D<E>{A, f, low, high};
   } else {
      return StrictArrayBase2D<E>{A, f, low, high};
   }
}


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F, bool copy_delete>
   requires expr::UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate(const Base& A, F f) {
   using E = detail::UnaryExpr<Base, F, copy_delete>;

   if constexpr(OneDimBaseType<Base>) {
      return StrictArrayBase1D<E>{A, f};
   } else {
      return StrictArrayBase2D<E>{A, f};
   }
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A) {
   return generate(A, expr::UnaryPlus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A) {
   return generate(A, expr::UnaryMinus{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator~(const Base& A) {
   return generate(A, expr::UnaryBitwiseNot{});
}


template <BooleanBaseType Base>
STRICT_CONSTEXPR auto operator!(const Base& A) {
   return generate(A, expr::UnaryLogicalNot{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto abs(const Base& A) {
   return generate(A, expr::UnaryAbs{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto exp(const Base& A) {
   return generate(A, expr::UnaryExp{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log(const Base& A) {
   return generate(A, expr::UnaryLog{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log2(const Base& A) {
   return generate(A, expr::UnaryLog2{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log10(const Base& A) {
   return generate(A, expr::UnaryLog10{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sqrt(const Base& A) {
   return generate(A, expr::UnarySqrt{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cbrt(const Base& A) {
   return generate(A, expr::UnaryCbrt{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sin(const Base& A) {
   return generate(A, expr::UnarySin{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cos(const Base& A) {
   return generate(A, expr::UnaryCos{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto tan(const Base& A) {
   return generate(A, expr::UnaryTan{});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow(const Base& A, ValueTypeOf<Base> pw) {
   return generate(A, expr::UnaryPow{pw});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow_int(const Base& A, ImplicitInt pw) {
   return generate(A, expr::UnaryPow{strict_cast<BuiltinTypeOf<Base>>(pw.get())});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto fast_pow_int(const Base& A, ImplicitInt pw) {
   return generate(A, expr::UnaryFastPowInt{pw.get()});
}


template <FloatingBaseType Base>
STRICT_CONSTEXPR auto inv(const Base& A) {
   return generate(A, expr::UnaryInv{});
}


template <Builtin T, BaseType Base>
STRICT_CONSTEXPR auto array_cast(const Base& A) {
   return generate(A, expr::UnaryCast<T>{});
}


}  // namespace spp

