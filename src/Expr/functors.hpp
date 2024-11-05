// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/strict_common.hpp"


namespace spp::expr {


////////////////////////////////////////////////////////////////////////////////////////////////////
struct UnaryPlus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return +x;
   }
};


struct UnaryMinus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      if constexpr(Integer<T>) {
         static_assert(SignedInteger<T>);
      }
      return -x;
   }
};


struct UnaryBitwiseNot {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return ~x;
   }
};


struct UnaryLogicalNot {
   template <Boolean T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return !x;
   }
};


struct UnaryAbs {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return abss(x);
   }
};


struct UnaryExp {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return exps(x);
   }
};


struct UnaryLog {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return logs(x);
   }
};


struct UnaryLog2 {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return log2s(x);
   }
};


struct UnaryLog10 {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return log10s(x);
   }
};


struct UnarySqrt {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return sqrts(x);
   }
};


struct UnaryCbrt {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return cbrts(x);
   }
};


struct UnarySin {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return sins(x);
   }
};


struct UnaryCos {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return coss(x);
   }
};


struct UnaryTan {
   template <Floating T>
   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return tans(x);
   }
};


template <Floating T>
struct UnaryPow {
   STRICT_CONSTEXPR_2026 explicit UnaryPow(Strict<T> p) : p_{p} {
   }

   STRICT_CONSTEXPR_2026 Strict<T> operator()(Strict<T> x) const {
      return pows(x, p_);
   }

private:
   Strict<T> p_;
};


template <Integer T>
struct UnaryFastPowInt {
   STRICT_CONSTEXPR explicit UnaryFastPowInt(Strict<T> p) : p_{p} {
   }

   template <Floating U>
   STRICT_CONSTEXPR Strict<U> operator()(Strict<U> x) const {
      return fast_pows_int(x, p_);
   }

private:
   Strict<T> p_;
};


struct UnaryInv {
   template <Floating T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x) const {
      return invs(x);
   }
};


template <Builtin T>
struct UnaryCast {
   template <Builtin U>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<U> x) const {
      return strict_cast<T>(x);
   }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
struct BinaryPlus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x + y;
   }
};


struct BinaryMinus {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x - y;
   }
};


struct BinaryMult {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x * y;
   }
};


struct BinaryDivide {
   template <Real T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x / y;
   }
};


struct BinaryModulo {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x % y;
   }
};


struct BinaryRightShift {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x << y;
   }
};


struct BinaryLeftShift {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x >> y;
   }
};


struct BinaryBitwiseAnd {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x & y;
   }
};


struct BinaryBitwiseOr {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x | y;
   }
};


struct BinaryBitwiseXor {
   template <Integer T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x ^ y;
   }
};


struct BinaryBooleanAnd {
   template <Boolean T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x && y;
   }
};


struct BinaryBooleanOr {
   template <Boolean T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x || y;
   }
};


struct BinaryBooleanXor {
   template <Boolean T>
   STRICT_CONSTEXPR Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return x ^ y;
   }
};


struct BinaryTwoProdFirst {
   template <Floating T>
   Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return two_prods(x, y).first;
   }
};


struct BinaryTwoProdSecond {
   template <Floating T>
   Strict<T> operator()(Strict<T> x, Strict<T> y) const {
      return two_prods(x, y).second;
   }
};


struct BinaryPowProdFirst {
   template <Floating T>
   Strict<T> operator()(Strict<T> x, ImplicitInt y) const {
      return pow_prods(x, y).first;
   }
};


struct BinaryPowProdSecond {
   template <Floating T>
   Strict<T> operator()(Strict<T> x, ImplicitInt y) const {
      return pow_prods(x, y).second;
   }
};


}  // namespace spp::expr

