// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "expr_traits.hpp"
#include "functors.hpp"
#include "types.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations.
template <OneDimBaseType Base1, OneDimBaseType Base2, typename F, bool copy_delete = false>
   requires expr::BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate1D(const Base1& A1, const Base2& A2, F f);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2);


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2);


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <OneDimBooleanBaseType Base1, OneDimBooleanBaseType Base2>
STRICT_CONSTEXPR auto operator&&(const Base1& A1, const Base2& A2);


template <OneDimBooleanBaseType Base1, OneDimBooleanBaseType Base2>
STRICT_CONSTEXPR auto operator||(const Base1& A1, const Base2& A2);


template <OneDimBooleanBaseType Base1, OneDimBooleanBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <OneDimFloatingBaseType Base1, OneDimFloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2);


template <OneDimFloatingBaseType Base1, OneDimSignedIntegerBaseType Base2>
auto pow_prod(const Base1& A1, const Base2& A2);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
// Its seems that checking for the first set of conditions, namely:
// requires(OneDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>)
// is redundant, but removing it would lead to ambiguities instead of selecting
// a deleted overload in some cases, such as Array1D<double>(3) + 1._sd.


template <typename Base1, typename Base2>
   requires(detail::OneDimRealExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator+(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimRealExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator-(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimRealExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator*(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimRealExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator/(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimIntegerExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator%(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimIntegerExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator<<(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimIntegerExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator>>(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimIntegerExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimIntegerExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator|(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimIntegerExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimBooleanExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator&&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimBooleanExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator||(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimBooleanExprRvalues<Base1, Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimFloatingExprRvalues<Base1, Base2>)
auto two_prod(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::OneDimFloatingExprRvalues<Base1, Base2>)
auto pow_prod(Base1&& A1, Base2&& A2) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations(scalars on the left).
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A2);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A2);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A2);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A2);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A2);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, Base&& A2) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations(scalars on the right).
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x);


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x);


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator*(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator/(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator%(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator<<(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator>>(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator|(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayOneDimIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(Base&& A, ValueTypeOf<Base> x) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base1, OneDimBaseType Base2, typename F, bool copy_delete>
   requires expr::BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate1D(const Base1& A1, const Base2& A2, F f) {
   return detail::GenArrayBase1D<detail::BinaryExpr1D<Base1, Base2, F, copy_delete>>(A1, A2, f);
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryPlus{});
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryMinus{});
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryMult{});
}


template <OneDimRealBaseType Base1, OneDimRealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryDivide{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryModulo{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryRightShift{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryLeftShift{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryBitwiseAnd{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryBitwiseOr{});
}


template <OneDimIntegerBaseType Base1, OneDimIntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryBitwiseXor{});
}


template <OneDimBooleanBaseType Base1, OneDimBooleanBaseType Base2>
STRICT_CONSTEXPR auto operator&&(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryBooleanAnd{});
}


template <OneDimBooleanBaseType Base1, OneDimBooleanBaseType Base2>
STRICT_CONSTEXPR auto operator||(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryBooleanOr{});
}


template <OneDimBooleanBaseType Base1, OneDimBooleanBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate1D(A1, A2, expr::BinaryBooleanXor{});
}


template <OneDimFloatingBaseType Base1, OneDimFloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate1D(A1, A2, expr::BinaryTwoProdFirst{}),
                    generate1D(A1, A2, expr::BinaryTwoProdSecond{})};
}


template <OneDimFloatingBaseType Base1, OneDimSignedIntegerBaseType Base2>
auto pow_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate1D(A1, A2, expr::BinaryPowProdFirst{}),
                    generate1D(A1, A2, expr::BinaryPowProdSecond{})};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryPlus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryMinus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryMult{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryDivide{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryModulo{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryRightShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryLeftShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryBitwiseAnd{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryBitwiseOr{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A2) {
   return generate1D(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryBitwiseXor{});
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryPlus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryMinus{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryMult{});
}


template <OneDimRealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryDivide{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryModulo{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryRightShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryLeftShift{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryBitwiseAnd{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryBitwiseOr{});
}


template <OneDimIntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x) {
   return generate1D(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryBitwiseXor{});
}


}  // namespace spp
