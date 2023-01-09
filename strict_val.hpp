#pragma once

#if __cplusplus < 202002L
   #error requires c++20 or higher
#else

#include <cmath>
#include <iomanip>
#include <limits>

#include "strict_concepts.hpp"
#include "strict_error.hpp"

#if defined (__INTEL_COMPILER) || defined (__INTEL_LLVM_COMPILER)
#include <mathimf.h>
#endif

namespace strict_array {

template<RealType T> struct StrictVal;
using Strict32 = StrictVal<float32>;
using Strict64 = StrictVal<float64>;
#ifdef STRICT_QUADRUPLE_PRECISION
using Strict128 = StrictVal<float128>;
#endif

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
struct StrictVal
{
public:
   using value_type = T;
   explicit constexpr StrictVal() = default;
   constexpr StrictVal(const StrictVal &) = default;
   constexpr StrictVal & operator=(const StrictVal &) & = default;

   template<RealType U> constexpr inline StrictVal(U val);
   template<RealType U> constexpr inline StrictVal & operator=(U val) &;
   template<RealType U> constexpr inline operator U () const; // safe conversion
   template<RealType U> constexpr inline U convert() const;   // conversion chosen by the user;

   [[nodiscard]] constexpr StrictVal operator+() const { return *this; }
   [[nodiscard]] constexpr StrictVal operator-() const { return StrictVal{-T{*this}}; }
   constexpr StrictVal operator++() { ++x; return *this; }
   constexpr StrictVal operator--() { --x; return *this; }
   constexpr StrictVal operator++(int) { StrictVal old{x}; ++x; return old; }
   constexpr StrictVal operator--(int) { StrictVal old{x}; --x; return old; }

   constexpr inline StrictVal operator+=(StrictVal strict_val);
   constexpr inline StrictVal operator-=(StrictVal strict_val);
   constexpr inline StrictVal operator*=(StrictVal strict_val);
   constexpr inline StrictVal operator/=(StrictVal strict_val);
   template<IntegerType U = T> constexpr inline StrictVal operator%=(StrictVal strict_val);
   template<IntegerType U = T> constexpr inline StrictVal operator<<=(StrictVal strict_val);
   template<IntegerType U = T> constexpr inline StrictVal operator>>=(StrictVal strict_val);

   template<RealType U> constexpr inline StrictVal operator+=(U val);
   template<RealType U> constexpr inline StrictVal operator-=(U val);
   template<RealType U> constexpr inline StrictVal operator*=(U val);
   template<RealType U> constexpr inline StrictVal operator/=(U val);
   template<IntegerType U = T> constexpr inline StrictVal operator%=(U val);
   template<IntegerType U = T> constexpr inline StrictVal operator<<=(U val);
   template<IntegerType U = T> constexpr inline StrictVal operator>>=(U val);

private:
   T x{};
};

template<RealType T> constexpr inline StrictVal<T> operator+=(T & val, StrictVal<T> strict_val);
template<RealType T> constexpr inline StrictVal<T> operator-=(T & val, StrictVal<T> strict_val);
template<RealType T> constexpr inline StrictVal<T> operator*=(T & val, StrictVal<T> strict_val);
template<RealType T> constexpr inline StrictVal<T> operator/=(T & val, StrictVal<T> strict_val);
template<IntegerType T> constexpr inline StrictVal<T> operator%=(T & val, StrictVal<T> strict_val);
template<IntegerType T> constexpr inline StrictVal<T> operator<<=(T & val, StrictVal<T> strict_val);
template<IntegerType T> constexpr inline StrictVal<T> operator>>=(T & val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator+(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator-(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator*(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator/(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator%(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator<<(StrictVal<T> v1, StrictVal<T> v2);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator>>(StrictVal<T> v1, StrictVal<T> v2);

template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator+(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator-(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator*(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator/(StrictVal<T> strict_val, T val);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator%(StrictVal<T> strict_val, T val);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator<<(StrictVal<T> strict_val, T val);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator>>(StrictVal<T> strict_val, T val);

template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator+(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator-(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator*(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> operator/(T val, StrictVal<T> strict_val);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator%(T val, StrictVal<T> strict_val);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator<<(T val, StrictVal<T> strict_val);
template<IntegerType T> [[nodiscard]] constexpr inline StrictVal<T> operator>>(T val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline bool operator==(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(StrictVal<T> v1, StrictVal<T> v2);

template<RealType T> [[nodiscard]] constexpr inline bool operator==(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(StrictVal<T> strict_val, T val);

template<RealType T> [[nodiscard]] constexpr inline bool operator==(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator<=(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator>=(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline bool operator!=(T val, StrictVal<T> strict_val);

template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> abs(StrictVal<T> v);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> min(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> max(StrictVal<T> v1, StrictVal<T> v2);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> min(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> max(T val, StrictVal<T> strict_val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> min(StrictVal<T> strict_val, T val);
template<RealType T> [[nodiscard]] constexpr inline StrictVal<T> max(StrictVal<T> strict_val, T val);

template<StandardFloatingType T> [[nodiscard]] constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v);
template<StandardFloatingType T> [[nodiscard]] inline bool isfinite_s(StrictVal<T> v);

template<NotQuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
#ifdef STRICT_QUADRUPLE_PRECISION
template<QuadType T> [[nodiscard]] constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2);
template<QuadType T> [[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v);
template<QuadType T> [[nodiscard]] inline bool isfinite_s(StrictVal<T> v);

template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val);
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
constexpr inline StrictVal<T>::StrictVal(U val) : x{val}
{
   static_assert(SameType<T, U>);
}

template<RealType T> template<RealType U>
constexpr inline StrictVal<T> & StrictVal<T>::operator=(U val) &
{
   static_assert(SameType<T, U>);
   x = val;
   return *this;
}

template<RealType T> template<RealType U>
constexpr inline StrictVal<T>::operator U () const
{
   static_assert(SameType<T, U>);
   return x;
}

template<RealType T> template<RealType U>
constexpr inline U StrictVal<T>::convert() const
{
   return static_cast<U>(x);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator+=(StrictVal<T> strict_val)
{ x += strict_val.x; return *this; }

template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator-=(StrictVal<T> strict_val)
{ x -= strict_val.x; return *this; }

template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator*=(StrictVal<T> strict_val)
{ x *= strict_val.x; return *this; }

template<RealType T>
constexpr inline StrictVal<T> StrictVal<T>::operator/=(StrictVal<T> strict_val)
{ x /= strict_val.x; return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator%=(StrictVal<T> strict_val)
{ x %= strict_val.x; return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator<<=(StrictVal<T> strict_val)
{ x <<= strict_val.x; return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator>>=(StrictVal<T> strict_val)
{ x >>= strict_val.x; return *this; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> template<RealType U>
constexpr inline StrictVal<T> StrictVal<T>::operator+=(U val)
{ static_assert(SameType<T, U>); x += val; return *this; }

template<RealType T> template<RealType U>
constexpr inline StrictVal<T> StrictVal<T>::operator-=(U val)
{ static_assert(SameType<T, U>); x -= val; return *this; }

template<RealType T> template<RealType U>
constexpr inline StrictVal<T> StrictVal<T>::operator*=(U val)
{ static_assert(SameType<T, U>); x *= val; return *this; }

template<RealType T> template<RealType U>
constexpr inline StrictVal<T> StrictVal<T>::operator/=(U val)
{ static_assert(SameType<T, U>); x /= val; return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator%=(U val)
{ static_assert(SameType<T, U>); x %= val; return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator<<=(U val)
{ static_assert(SameType<T, U>); x <<= val; return *this; }

template<RealType T> template<IntegerType U>
constexpr inline StrictVal<T> StrictVal<T>::operator>>=(U val)
{ static_assert(SameType<T, U>); x >>= val; return *this; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline StrictVal<T> operator+=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val += T{strict_val}}; }

template<RealType T> constexpr inline StrictVal<T> operator-=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val -= T{strict_val}}; }

template<RealType T> constexpr inline StrictVal<T> operator*=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val *= T{strict_val}}; }

template<RealType T> constexpr inline StrictVal<T> operator/=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val /= T{strict_val}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator%=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val %= T{strict_val}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator<<=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val <<= T{strict_val}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator>>=(T & val, StrictVal<T> strict_val)
{ return StrictVal<T>{val >>= T{strict_val}}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline StrictVal<T> operator+(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T{v1} + T{v2}}}; }

template<RealType T> constexpr inline StrictVal<T> operator-(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T{v1} - T{v2}}}; }

template<RealType T> constexpr inline StrictVal<T> operator*(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T{v1} * T{v2}}}; }

template<RealType T> constexpr inline StrictVal<T> operator/(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T{v1} / T{v2}}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator%(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T{v1} % T{v2}}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator<<(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T(v1) << T{v2}}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator>>(StrictVal<T> v1, StrictVal<T> v2)
{ return StrictVal<T>{T{T{v1} >> T{v2}}}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline StrictVal<T> operator+(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} + val}}; }

template<RealType T> constexpr inline StrictVal<T> operator-(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} - val}}; }

template<RealType T> constexpr inline StrictVal<T> operator*(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} * val}}; }

template<RealType T> constexpr inline StrictVal<T> operator/(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} / val}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator%(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} % val}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator<<(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} << val}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator>>(StrictVal<T> strict_val, T val)
{ return StrictVal<T>{T{T{strict_val} >> val}}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline StrictVal<T> operator+(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val + T{strict_val}}}; }

template<RealType T> constexpr inline StrictVal<T> operator-(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val - T{strict_val}}}; }

template<RealType T> constexpr inline StrictVal<T> operator*(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val * T{strict_val}}}; }

template<RealType T> constexpr inline StrictVal<T> operator/(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val / T{strict_val}}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator%(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val % T{strict_val}}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator<<(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val << T{strict_val}}}; }

template<IntegerType T> constexpr inline StrictVal<T> operator>>(T val, StrictVal<T> strict_val)
{ return StrictVal<T>{T{val >> T{strict_val}}}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline bool operator==(StrictVal<T> v1, StrictVal<T> v2)
{ return T{v1} == T{v2}; }

template<RealType T> constexpr inline bool operator<(StrictVal<T> v1, StrictVal<T> v2)
{ return T{v1} < T{v2}; }

template<RealType T> constexpr inline bool operator>(StrictVal<T> v1, StrictVal<T> v2)
{ return T{v1} > T{v2}; }

template<RealType T> constexpr inline bool operator<=(StrictVal<T> v1, StrictVal<T> v2)
{ return T{v1} <= T{v2}; }

template<RealType T> constexpr inline bool operator>=(StrictVal<T> v1, StrictVal<T> v2)
{ return T{v1} >= T{v2}; }

template<RealType T> constexpr inline bool operator!=(StrictVal<T> v1, StrictVal<T> v2)
{ return T{v1} != T{v2}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline bool operator==(StrictVal<T> strict_val, T val)
{ return T{strict_val} == val; }

template<RealType T> constexpr inline bool operator<(StrictVal<T> strict_val, T val)
{ return T{strict_val} < val; }

template<RealType T> constexpr inline bool operator>(StrictVal<T> strict_val, T val)
{ return T{strict_val} > val; }

template<RealType T> constexpr inline bool operator<=(StrictVal<T> strict_val, T val)
{ return T{strict_val} <= val; }

template<RealType T> constexpr inline bool operator>=(StrictVal<T> strict_val, T val)
{ return T{strict_val} >= val; }

template<RealType T> constexpr inline bool operator!=(StrictVal<T> strict_val, T val)
{ return T{strict_val} != val; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline bool operator==(T val, StrictVal<T> strict_val)
{ return val == T{strict_val}; }

template<RealType T> constexpr inline bool operator<(T val, StrictVal<T> strict_val)
{ return val < T{strict_val}; }

template<RealType T> constexpr inline bool operator>(T val, StrictVal<T> strict_val)
{ return val > T{strict_val}; }

template<RealType T> constexpr inline bool operator<=(T val, StrictVal<T> strict_val)
{ return val <= T{strict_val}; }

template<RealType T> constexpr inline bool operator>=(T val, StrictVal<T> strict_val)
{ return val >= T{strict_val}; }

template<RealType T> constexpr inline bool operator!=(T val, StrictVal<T> strict_val)
{ return val != T{strict_val}; }

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<RealType T> constexpr inline StrictVal<T> abs(StrictVal<T> v)
{ return T{v} > T{0} ? v : -v; }

template<RealType T> constexpr inline StrictVal<T> min(StrictVal<T> v1, StrictVal<T> v2)
{ return v1 < v2 ? v1 : v2; }

template<RealType T> constexpr inline StrictVal<T> max(StrictVal<T> v1, StrictVal<T> v2)
{ return v1 > v2 ? v1 : v2; }

template<RealType T> constexpr inline StrictVal<T> min(T val, StrictVal<T> strict_val)
{ return val < strict_val ? StrictVal<T>{val} : strict_val; }

template<RealType T> constexpr inline StrictVal<T> max(T val, StrictVal<T> strict_val)
{ return val > strict_val ? StrictVal<T>{val} : strict_val; }

template<RealType T> constexpr inline StrictVal<T> min(StrictVal<T> strict_val, T val)
{ return strict_val < val ? strict_val : StrictVal<T>{val}; }

template<RealType T> constexpr inline StrictVal<T> max(StrictVal<T> strict_val, T val)
{ return strict_val > val ? strict_val : StrictVal<T>{val}; }

template<StandardFloatingType T> constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = std::fma(T{v1}, T{v2}, T{-r});
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}

template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v)
{ return StrictVal<T>{std::exp(T{v})}; }

template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v)
{ return StrictVal<T>{std::sqrt(T{v})}; }

template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v)
{ return StrictVal<T>{std::sin(T{v})}; }

template<StandardFloatingType T> [[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v)
{ return StrictVal<T>{std::cos(T{v})}; }

template<StandardFloatingType T> [[nodiscard]] inline bool isfinite_s(StrictVal<T> v)
{
   // std::isfinite was not giving the correct result when compiled with one of the
   // oneAPI versions.
   #if defined (__INTEL_COMPILER) || defined (__INTEL_LLVM_COMPILER)
   return finite(T{v});
   #else
   return std::isfinite(T{v});
   #endif
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template<NotQuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   int num_digits{};
   if(SameType<T, float>)            num_digits = std::numeric_limits<float>::digits10 + 1;
   else if(SameType<T, double>)      num_digits = std::numeric_limits<double>::digits10 + 1;
   else if(SameType<T, long double>) num_digits = std::numeric_limits<long double>::digits10 + 1;
   else                              num_digits = int(std::cout.precision());

   os << std::setprecision(num_digits) << T{strict_val};
   return os;
}

#ifdef STRICT_QUADRUPLE_PRECISION
template<QuadType T> constexpr inline auto two_prod(StrictVal<T> v1, StrictVal<T> v2)
{
   auto r = v1 * v2;
   auto s = fmaq(T{v1}, T{v2}, T{-r});
   return std::pair<StrictVal<T>, StrictVal<T>>{r, s};
}

template<QuadType T> [[nodiscard]] inline StrictVal<T> exps(StrictVal<T> v)
{ return StrictVal<T>{expq(T{v})}; }

template<QuadType T> [[nodiscard]] inline StrictVal<T> sqrts(StrictVal<T> v)
{ return StrictVal<T>{sqrtq(T{v})}; }

template<QuadType T> [[nodiscard]] inline StrictVal<T> sins(StrictVal<T> v)
{ return StrictVal<T>{sinq(T{v})}; }

template<QuadType T> [[nodiscard]] inline StrictVal<T> coss(StrictVal<T> v)
{ return StrictVal<T>{cosq(T{v})}; }

template<QuadType T> [[nodiscard]] inline bool isfinite_s(StrictVal<T> v)
{ return finiteq(T{v}); }

template<QuadType T> std::ostream & operator<<(std::ostream & os, StrictVal<T> strict_val)
{
   int width = 39;
   char buf[128];
   quadmath_snprintf(buf, sizeof(buf), "%+-#*.32Qe", width, T{strict_val});
   os << buf;
   return os;
}
#endif

}

#endif
