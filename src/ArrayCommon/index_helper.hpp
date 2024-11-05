//// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "../StrictCommon/auxiliary_types.hpp"
#include "../StrictCommon/common_traits.hpp"
#include "../StrictCommon/config.hpp"
#include "../StrictCommon/strict_literals.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"
#include "array_traits.hpp"


namespace spp::detail {


template <typename T>
class IndexWrapper {
public:
   IndexWrapper(T i) = delete;
};


template <typename T>
   requires SignedInteger<T> || SameAs<T, StrictInt> || SameAs<T, StrictLong>
         || SameAs<T, ImplicitInt>
class IndexWrapper<T> {
   ImplicitInt i_;

public:
   STRICT_CONSTEXPR_INLINE IndexWrapper(T i) : i_{i} {
   }

   STRICT_CONSTEXPR_INLINE index_t get([[maybe_unused]] OneDimBaseType auto const& A) const {
      return i_.get();
   }

   STRICT_CONSTEXPR_INLINE index_t get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return i_.get();
   }

   STRICT_CONSTEXPR_INLINE index_t get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return i_.get();
   }
};


template <typename T>
   requires SameAs<T, Last>
class IndexWrapper<T> {
   index_t i_;

public:
   STRICT_CONSTEXPR_INLINE IndexWrapper(T lst) : i_{lst.get()} {
   }

   STRICT_CONSTEXPR_INLINE index_t get(OneDimBaseType auto const& A) const {
      return A.size() - i_ - 1_sl;
   }

   STRICT_CONSTEXPR_INLINE index_t get_row(TwoDimBaseType auto const& A) const {
      return A.rows() - i_ - 1_sl;
   }

   STRICT_CONSTEXPR_INLINE index_t get_col(TwoDimBaseType auto const& A) const {
      return A.cols() - i_ - 1_sl;
   }
};


STRICT_CONSTEXPR_INLINE index_t index_helper(OneDimBaseType auto const& A, auto i) {
   return IndexWrapper{i}.get(A);
}


STRICT_CONSTEXPR_INLINE index_t index_row_helper(TwoDimBaseType auto const& A, auto i) {
   return IndexWrapper{i}.get_row(A);
}


STRICT_CONSTEXPR_INLINE index_t index_col_helper(TwoDimBaseType auto const& A, auto j) {
   return IndexWrapper{j}.get_col(A);
}


STRICT_CONSTEXPR_INLINE std::pair<index_t, index_t> index_row_col_helper(
    TwoDimBaseType auto const& A, auto i, auto j) {
   return {IndexWrapper{i}.get_row(A), IndexWrapper{j}.get_col(A)};
}


}  // namespace spp::detail

