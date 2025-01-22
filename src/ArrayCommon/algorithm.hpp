// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/config.hpp"
#include "../StrictCommon/strict_literals.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"
#include "array_traits.hpp"
#include "use.hpp"


namespace spp::detail {


template <BaseType Base, typename F>
STRICT_CONSTEXPR_INLINE void apply0(Base& A, F f) {
   for(index_t i = 0_sl; i < A.size(); ++i) {
      f(i);
   }
}


template <BaseType Base1, BaseType Base2, typename F>
STRICT_CONSTEXPR_INLINE void apply1(Base1& A1, [[maybe_unused]] const Base2& A2, F f) {
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      f(i);
   }
}


template <typename It, BaseType Base>
STRICT_CONSTEXPR_INLINE void copy(It b, It e, Base& A) {
   for(index_t count = 0_sl; b != e; ++b) {
      A.un(count++) = *b;
   }
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR_INLINE void copy(use::List1D<BuiltinTypeOf<Base>> L, Base& A) {
   copy(L.begin(), L.end(), A);
}


template <Builtin T, TwoDimBaseType Base>
STRICT_CONSTEXPR_INLINE void copy(use::List2D<T> L, Base& A) {
   StrictInt r{};
   for(auto lrow : L) {
      StrictInt c{};
      for(auto lcol : lrow) {
         A.un(r, c++) = lcol;
      }
      ++r;
   }
}


template <BaseType Base1, BaseType Base2>
STRICT_CONSTEXPR_INLINE void copy(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2) {
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      A2.un(i) = A1.un(i);
   }
}


template <ArrayTwoDimType Base1, ArrayTwoDimType Base2>
STRICT_CONSTEXPR_INLINE void copy(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2) {
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      A2.un(i) = A1.un(i);
   }
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR_INLINE void copy(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2) {
   for(index_t i = 0_sl; i < A1.rows(); ++i) {
      for(index_t j = 0_sl; j < A1.cols(); ++j) {
         A2.un(i, j) = A1.un(i, j);
      }
   }
}


template <BaseType Base1, BaseType Base2>
STRICT_CONSTEXPR_INLINE void copyn(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2,
                                   index_t n) {
   for(index_t i = 0_sl; i < n; ++i) {
      A2.un(i) = A1.un(i);
   }
}


template <BaseType Base1, BaseType Base2>
STRICT_CONSTEXPR_INLINE void copyn(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2,
                                   index_t start1, index_t start2, index_t n) {
   for(index_t i = 0_sl; i < n; ++i) {
      A2.un(start2 + i) = A1.un(start1 + i);
   }
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR_INLINE void copy_rows(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2,
                                       index_t n) {
   for(index_t i = 0_sl; i < n; ++i) {
      for(index_t j = 0_sl; j < A1.cols(); ++j) {
         A2.un(i, j) = A1.un(i, j);
      }
   }
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR_INLINE void copy_rows(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2,
                                       index_t start1, index_t start2, index_t n) {
   for(index_t i = 0_sl; i < n; ++i) {
      for(index_t j = 0_sl; j < A1.cols(); ++j) {
         A2.un(i + start2, j) = A1.un(i + start1, j);
      }
   }
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR_INLINE void copy_cols(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2,
                                       index_t n) {
   for(index_t i = 0_sl; i < A1.rows(); ++i) {
      for(index_t j = 0_sl; j < n; ++j) {
         A2.un(i, j) = A1.un(i, j);
      }
   }
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2>
STRICT_CONSTEXPR_INLINE void copy_cols(const Base1& STRICT_RESTRICT A1, Base2& STRICT_RESTRICT A2,
                                       index_t start1, index_t start2, index_t n) {
   for(index_t i = 0_sl; i < A1.rows(); ++i) {
      for(index_t j = 0_sl; j < n; ++j) {
         A2.un(i, j + start2) = A1.un(i, j + start1);
      }
   }
}


template <BaseType Base>
STRICT_CONSTEXPR_INLINE void fill(ValueTypeOf<Base> val, Base& A) {
   for(index_t i = 0_sl; i < A.size(); ++i) {
      A.un(i) = val;
   }
}


}  // namespace spp::detail

