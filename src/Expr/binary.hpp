// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "../derived1D.hpp"
#include "../derived2D.hpp"
#include "expr_traits.hpp"
#include "functors.hpp"
#include "special.hpp"
#include "types.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations.
template <BaseType Base1, BaseType Base2, typename F, bool copy_delete = false>
   requires expr::BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate(const Base1& A1, const Base2& A2, F f);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2);


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2);


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator&&(const Base1& A1, const Base2& A2);


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator||(const Base1& A1, const Base2& A2);


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base1, FloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base1, SignedIntegerBaseType Base2>
auto pow_prod(const Base1& A1, const Base2& A2);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
// Its seems that checking for the first set of conditions, namely:
// (RealBaseType<RemoveRef<Base1>> && RealBaseType<RemoveRef<Base2>>)
// is redundant, but removing it would lead to ambiguities instead of selecting
// a deleted overload in some cases, such as Array1D<double>(3) + 1._sd.


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator+(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator-(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator*(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::RealExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator/(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator%(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator<<(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator>>(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator|(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::IntegerExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::BooleanExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator&&(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::BooleanExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator||(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::BooleanExprDeleted<Base1, Base2>)
STRICT_CONSTEXPR auto operator^(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::FloatingExprDeleted<Base1, Base2>)
auto two_prod(Base1&& A1, Base2&& A2) = delete;


template <typename Base1, typename Base2>
   requires(detail::FloatingExprDeleted<Base1, Base2>)
auto pow_prod(Base1&& A1, Base2&& A2) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations(scalars on the left).
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A2);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A2);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A2);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A2);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A2);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A2);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A2);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A2);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A2);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A2);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, Base&& A2) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, Base&& A2) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
// Binary operations(scalars on the right).
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x);


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x);


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x);


////////////////////////////////////////////////////////////////////////////////////////////////////
// Deleted overloads.
template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator+(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator-(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator*(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayRealTypeRvalue<Base>
STRICT_CONSTEXPR auto operator/(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator%(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator<<(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator>>(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator&(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator|(Base&& A, ValueTypeOf<Base> x) = delete;


template <typename Base>
   requires detail::ArrayIntegerTypeRvalue<Base>
STRICT_CONSTEXPR auto operator^(Base&& A, ValueTypeOf<Base> x) = delete;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base1, BaseType Base2, typename F, bool copy_delete>
   requires expr::BinaryOperation<Base1, Base2, F>
STRICT_CONSTEXPR auto generate(const Base1& A1, const Base2& A2, F f) {
   using E = detail::BinaryExpr<Base1, Base2, F, copy_delete>;

   if constexpr(OneDimBaseType<Base1>) {
      return detail::GenArrayBase1D<E>(A1, A2, f);
   } else {
      return detail::GenArrayBase2D<E>(A1, A2, f);
   }
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator+(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryPlus{});
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator-(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryMinus{});
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator*(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryMult{});
}


template <RealBaseType Base1, RealBaseType Base2>
STRICT_CONSTEXPR auto operator/(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryDivide{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator%(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryModulo{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator<<(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryRightShift{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator>>(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryLeftShift{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator&(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBitwiseAnd{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator|(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBitwiseOr{});
}


template <IntegerBaseType Base1, IntegerBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBitwiseXor{});
}


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator&&(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBooleanAnd{});
}


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator||(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBooleanOr{});
}


template <BooleanBaseType Base1, BooleanBaseType Base2>
STRICT_CONSTEXPR auto operator^(const Base1& A1, const Base2& A2) {
   return generate(A1, A2, expr::BinaryBooleanXor{});
}


template <FloatingBaseType Base1, FloatingBaseType Base2>
auto two_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate(A1, A2, expr::BinaryTwoProdFirst{}),
                    generate(A1, A2, expr::BinaryTwoProdSecond{})};
}


template <FloatingBaseType Base1, SignedIntegerBaseType Base2>
auto pow_prod(const Base1& A1, const Base2& A2) {
   return std::pair{generate(A1, A2, expr::BinaryPowProdFirst{}),
                    generate(A1, A2, expr::BinaryPowProdSecond{})};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryPlus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryMinus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryMult{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryDivide{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryModulo{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryRightShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryLeftShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryBitwiseAnd{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryBitwiseOr{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(ValueTypeOf<Base> x, const Base& A2) {
   return generate(const1D<BuiltinTypeOf<Base>>(A2.size(), x), A2, expr::BinaryBitwiseXor{});
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <RealBaseType Base>
STRICT_CONSTEXPR auto operator+(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryPlus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator-(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryMinus{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator*(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryMult{});
}


template <RealBaseType Base>
STRICT_CONSTEXPR auto operator/(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryDivide{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator%(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryModulo{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator<<(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryRightShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator>>(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryLeftShift{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator&(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryBitwiseAnd{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator|(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryBitwiseOr{});
}


template <IntegerBaseType Base>
STRICT_CONSTEXPR auto operator^(const Base& A, ValueTypeOf<Base> x) {
   return generate(A, const1D<BuiltinTypeOf<Base>>(A.size(), x), expr::BinaryBitwiseXor{});
}


}  // namespace spp
