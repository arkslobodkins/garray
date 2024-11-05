// Arkadijs Slobodkins, 2023


#pragma once


#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "functors.hpp"
#include "types.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
// Unary operations.
template <OneDimBaseType Base, typename F, bool copy_delete = false>
   requires expr::UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate1D(const Base& A, F f);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator~(const Base& A);


template <OneDimBooleanBaseType Base>
STRICT_CONSTEXPR auto operator!(const Base& A);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto abs(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto exp(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log2(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log10(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sqrt(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cbrt(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sin(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cos(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto tan(const Base& A);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow(const Base& A, ValueTypeOf<Base> pw);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow_int(const Base& A, ImplicitInt pw);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto fast_pow_int(const Base& A, ImplicitInt pw);


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto inv(const Base& A);


template <Builtin T, OneDimBaseType Base>
STRICT_CONSTEXPR auto array_cast(const Base& A);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator~(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimBooleanTypeRvalue<Base>
STRICT_CONSTEXPR auto operator!(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto abs(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto exp(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto log(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto log2(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto log10(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto sqrt(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto cbrt(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto sin(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto cos(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto tan(Base&& A) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto pow(Base&& A, ValueTypeOf<Base> pw) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR_2026 auto pow_int(Base&& A, ImplicitInt pw) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR auto fast_pow_int(Base&& A, ImplicitInt pw) = delete;


template <typename Base>
   requires detail::ArrayOneDimFloatingTypeRvalue<Base>
STRICT_CONSTEXPR auto inv(Base&& A) = delete;


template <Builtin T, typename Base>
   requires detail::ArrayOneDimTypeRvalue<Base>
STRICT_CONSTEXPR auto array_cast(Base&& A) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base, typename F, bool copy_delete>
   requires expr::UnaryOperation<Base, F>
STRICT_CONSTEXPR auto generate1D(const Base& A, F f) {
   return detail::GenArrayBase1D<detail::UnaryExpr1D<Base, F, copy_delete>>{A, f};
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A) {
   return generate1D(A, expr::UnaryPlus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A) {
   return generate1D(A, expr::UnaryMinus{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator~(const Base& A) {
   return generate1D(A, expr::UnaryBitwiseNot{});
}


template <OneDimBooleanBaseType Base>
STRICT_CONSTEXPR auto operator!(const Base& A) {
   return generate1D(A, expr::UnaryLogicalNot{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto abs(const Base& A) {
   return generate1D(A, expr::UnaryAbs{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto exp(const Base& A) {
   return generate1D(A, expr::UnaryExp{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log(const Base& A) {
   return generate1D(A, expr::UnaryLog{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log2(const Base& A) {
   return generate1D(A, expr::UnaryLog2{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto log10(const Base& A) {
   return generate1D(A, expr::UnaryLog10{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sqrt(const Base& A) {
   return generate1D(A, expr::UnarySqrt{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cbrt(const Base& A) {
   return generate1D(A, expr::UnaryCbrt{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto sin(const Base& A) {
   return generate1D(A, expr::UnarySin{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto cos(const Base& A) {
   return generate1D(A, expr::UnaryCos{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto tan(const Base& A) {
   return generate1D(A, expr::UnaryTan{});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow(const Base& A, ValueTypeOf<Base> pw) {
   return generate1D(A, expr::UnaryPow{pw});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR_2026 auto pow_int(const Base& A, ImplicitInt pw) {
   return generate1D(A, expr::UnaryPow{strict_cast<BuiltinTypeOf<Base>>(pw.get())});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto fast_pow_int(const Base& A, ImplicitInt pw) {
   return generate1D(A, expr::UnaryFastPowInt{pw.get()});
}


template <OneDimFloatingBaseType Base>
STRICT_CONSTEXPR auto inv(const Base& A) {
   return generate1D(A, expr::UnaryInv{});
}


template <Builtin T, OneDimBaseType Base>
STRICT_CONSTEXPR auto array_cast(const Base& A) {
   return generate1D(A, expr::UnaryCast<T>{});
}


}  // namespace spp

