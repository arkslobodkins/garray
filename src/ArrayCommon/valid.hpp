// Arkadijs Slobodkins, 2023


#pragma once


#include <algorithm>
#include <utility>
#include <vector>

#include "../StrictCommon/auxiliary_types.hpp"
#include "../StrictCommon/config.hpp"
#include "../StrictCommon/error.hpp"
#include "../StrictCommon/strict_literals.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"
#include "array_traits.hpp"
#include "use.hpp"


namespace spp::detail {


STRICT_CONSTEXPR StrictBool valid_index(BaseType auto const& A, index_t i) {
   return i > -1_sl && i < A.size();
}


STRICT_CONSTEXPR StrictBool valid_row(TwoDimBaseType auto const& A, index_t i) {
   return i > -1_sl && i < A.rows();
}


STRICT_CONSTEXPR StrictBool valid_col(TwoDimBaseType auto const& A, index_t j) {
   return j > -1_sl && j < A.cols();
}


STRICT_CONSTEXPR StrictBool valid_index(TwoDimBaseType auto const& A, index_t i, index_t j) {
   return valid_row(A, i) && valid_col(A, j);
}


STRICT_CONSTEXPR StrictBool semi_valid_row_col_sizes(index_t m, index_t n) {
   return !((m == 0_sl) ^ (n == 0_sl));
}


template <Builtin T>
STRICT_CONSTEXPR StrictBool valid_list2D(use::List2D<T> list) {
   if(list.size() != 0) {
      const auto nrows = list.size();
      const auto ncols = (*list.begin()).size();

      // Allow empty initializer_list.
      if(nrows == 1 && ncols == 0) {
         return true_sb;
      }

      // Number of columns is not zero to forbid nonzero rows and zero columns.
      if(ncols == 0) {
         return false_sb;
      }

      for(auto lrow : list) {
         if(lrow.size() != ncols) {
            return false_sb;
         }
      }
   }
   return true_sb;
}


template <Builtin T>
STRICT_CONSTEXPR auto list2D_row_col_sizes(use::List2D<T> list) {
   if(list.size() != 0) {
      const auto nrows = list.size();
      const auto ncols = (*list.begin()).size();

      // Allow empty initializer_list.
      if(nrows == 1 && ncols == 0) {
         return std::pair{0_sl, 0_sl};
      }

      return std::pair{to_index_t(nrows), to_index_t(ncols)};
   }
   return std::pair{0_sl, 0_sl};
}


STRICT_CONSTEXPR StrictBool valid_slice_vector(BaseType auto const& A,
                                               const std::vector<ImplicitInt>& indexes) {
   return Strict{
       std::ranges::all_of(indexes, [&A](auto i) { return bool{valid_index(A, i.get())}; })};
}


STRICT_CONSTEXPR StrictBool valid_row_slice_vector(TwoDimBaseType auto const& A,
                                                   const std::vector<ImplicitInt>& indexes) {
   return Strict{
       std::ranges::all_of(indexes, [&A](auto i) { return bool{valid_row(A, i.get())}; })};
}


STRICT_CONSTEXPR StrictBool valid_col_slice_vector(TwoDimBaseType auto const& A,
                                                   const std::vector<ImplicitInt>& indexes) {
   return Strict{
       std::ranges::all_of(indexes, [&A](auto i) { return bool{valid_col(A, i.get())}; })};
}


template <typename F>
STRICT_CONSTEXPR StrictBool valid_complement_index_vector(F f, BaseType auto const& A,
                                                          const std::vector<ImplicitInt>& indexes) {
   if(!indexes.empty()) {
      if(auto lst = indexes.size() - 1; !f(A, indexes[0].get()) || !f(A, indexes[lst].get())) {
         return false_sb;
      }
   }
   // Check if indexes are in strictly increasing order, true for ranges that are empty or of
   // length 1.
   return Strict{
       std::ranges::is_sorted(indexes, [](auto x, auto y) { return bool{x.get() <= y.get()}; })};
}


template <typename F>
STRICT_CONSTEXPR std::vector<ImplicitInt> complement_index_vector(
    F f, index_t n, BaseType auto const& A, const std::vector<ImplicitInt>& indexes) {
   ASSERT_STRICT_DEBUG(valid_complement_index_vector(f, A, indexes));

   std::vector<ImplicitInt> cmpl;
   cmpl.reserve(to_size_t(n) - indexes.size());
   for(index_t i = 0_sl, cnt = 0_sl; i < n; ++i) {
      // No UB since the order of evaluation is left to right.
      if(bool{to_size_t(cnt) < indexes.size()} && bool{i == indexes[to_size_t(cnt)].get()}) {
         ++cnt;
      } else {
         cmpl.emplace_back(i);
      }
   }
   return cmpl;
}


}  // namespace spp::detail
