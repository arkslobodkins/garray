// Arkadijs Slobodkins, 2023


#pragma once


#include <iterator>
#include <type_traits>
#include <utility>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "array_base1D.hpp"
#include "array_base2D.hpp"
#include "derived1D.hpp"
#include "fixed_array_base2D.hpp"
#include "iterator.hpp"
#include "slice.hpp"
#include "slicearray_base2D.hpp"


namespace spp {


namespace detail {


template <TwoDimNonConstBaseType Base>
class StrictArrayMutable2D;


template <typename Base>
class StrictArray2D;


}  // namespace detail


template <Builtin T, AlignmentFlag AF = Aligned>
using Array2D = detail::StrictArray2D<detail::ArrayBase2D<T, AF>>;


template <Builtin T, ImplicitIntStatic M, ImplicitIntStatic N>
using FixedArray2D = detail::StrictArray2D<detail::FixedArrayBase2D<T, M, N>>;


namespace detail {


template <TwoDimBaseType Base>
class STRICT_NODISCARD StrictArrayBase2D : public Base {
public:
   using size_type = index_t;
   using typename Base::builtin_type;
   using typename Base::value_type;

   // Unhide Base member functions.
   using Base::cols;
   using Base::rows;

   using Base::Base;
   STRICT_NODISCARD_CONSTEXPR StrictArrayBase2D(const StrictArrayBase2D&) = default;
   STRICT_NODISCARD_CONSTEXPR StrictArrayBase2D(StrictArrayBase2D&&) = default;
   STRICT_CONSTEXPR StrictArrayBase2D& operator=(const StrictArrayBase2D&) = delete;
   STRICT_CONSTEXPR StrictArrayBase2D& operator=(const auto&) = delete;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator()(Index1 i, Index2 j) {
      auto [ih, jh] = index_row_col_helper(*this, i, j);
      ASSERT_STRICT_RANGE_DEBUG(valid_index(*this, ih, jh));
      return Base::un(ih, jh);
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator()(Index1 i, Index2 j) const {
      auto [ih, jh] = index_row_col_helper(*this, i, j);
      ASSERT_STRICT_RANGE_DEBUG(valid_index(*this, ih, jh));
      return Base::un(ih, jh);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i) {
      return Base::un(i);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i) const {
      return Base::un(i);
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(Index1 i, Index2 j) {
      return Base::un(index_row_helper(*this, i), index_col_helper(*this, j));
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(Index1 i, Index2 j) const {
      return Base::un(index_row_helper(*this, i), index_col_helper(*this, j));
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) at(Index1 i, Index2 j) {
      auto [ih, jh] = index_row_col_helper(*this, i, j);
      ASSERT_STRICT_RANGE_ALWAYS(valid_index(*this, ih, jh));
      return Base::un(ih, jh);
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) at(Index1 i, Index2 j) const {
      auto [ih, jh] = index_row_col_helper(*this, i, j);
      ASSERT_STRICT_RANGE_ALWAYS(valid_index(*this, ih, jh));
      return Base::un(ih, jh);
   }

#ifdef __cpp_multidimensional_subscript
   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator[](Index1 i, Index2 j) {
      return this->operator()(i, j);
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator[](Index1 i, Index2 j) const {
      return this->operator()(i, j);
   }
#endif

   STRICT_CONSTEXPR StrictBool empty() const {
      return Base::size() == 0_sl;
   }

   STRICT_CONSTEXPR index_t size_m1() const {
      return Base::size() - 1_sl;
   }

   STRICT_CONSTEXPR index_t rows_m1() const {
      return Base::rows() - 1_sl;
   }

   STRICT_CONSTEXPR index_t cols_m1() const {
      return Base::cols() - 1_sl;
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR auto begin() & {
      if constexpr(NonConstBaseType<Base>) {
         return Iterator{*this, 0_sl};
      } else {
         return ConstIterator{*this, 0_sl};
      }
   }

   STRICT_CONSTEXPR auto begin() const& {
      return ConstIterator{*this, 0_sl};
   }

   STRICT_CONSTEXPR auto end() & {
      if constexpr(NonConstBaseType<Base>) {
         return Iterator{*this, Base::size()};
      } else {
         return ConstIterator{*this, Base::size()};
      }
   }

   STRICT_CONSTEXPR auto end() const& {
      return ConstIterator{*this, Base::size()};
   }

   STRICT_CONSTEXPR auto cbegin() const& {
      return this->begin();
   }

   STRICT_CONSTEXPR auto cend() const& {
      return this->end();
   }

   STRICT_CONSTEXPR auto rbegin() & {
      return std::reverse_iterator{this->end()};
   }

   STRICT_CONSTEXPR auto rbegin() const& {
      return std::reverse_iterator{this->end()};
   }

   STRICT_CONSTEXPR auto rend() & {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_CONSTEXPR auto rend() const& {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_CONSTEXPR auto crbegin() const& {
      return std::reverse_iterator{this->rbegin()};
   }

   STRICT_CONSTEXPR auto crend() const& {
      return std::reverse_iterator{this->rend()};
   }

   // Disallow pointers to temporaries to reduce the risk of dangling iterators.
   STRICT_CONSTEXPR auto begin() && = delete;
   STRICT_CONSTEXPR auto begin() const&& = delete;
   STRICT_CONSTEXPR auto end() && = delete;
   STRICT_CONSTEXPR auto end() const&& = delete;
   STRICT_CONSTEXPR auto cbegin() const&& = delete;
   STRICT_CONSTEXPR auto cend() const&& = delete;

   STRICT_CONSTEXPR auto rbegin() && = delete;
   STRICT_CONSTEXPR auto rbegin() const&& = delete;
   STRICT_CONSTEXPR auto rend() && = delete;
   STRICT_CONSTEXPR auto rend() const&& = delete;
   STRICT_CONSTEXPR auto crbegin() const&& = delete;
   STRICT_CONSTEXPR auto crend() const&& = delete;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <IndexType Index>
   STRICT_CONSTEXPR auto row(Index i) & {
      auto ih = index_row_helper(*this, i);
      ASSERT_STRICT_DEBUG(valid_row(*this, ih));
      auto first = ih * Base::cols();
      return this->view1D()(seq{first, first + Base::cols() - 1_sl, 1});
   }

   template <IndexType Index>
   STRICT_CONSTEXPR auto col(Index j) & {
      auto jh = index_col_helper(*this, j);
      ASSERT_STRICT_DEBUG(valid_col(*this, jh));
      auto first = jh;
      auto last = first + (Base::rows() - 1_sl) * Base::cols();
      return this->view1D()(seq{first, last, Base::cols()});
   }

   STRICT_CONSTEXPR auto diag() & {
      return this->view1D()(seqN{0, mins(Base::rows(), Base::cols()), Base::cols() + 1_sl});
   }

   STRICT_CONSTEXPR auto diag(ImplicitInt i) & {
      return this->view1D()(this->diag_slice_impl(i));
   }

   STRICT_CONSTEXPR auto view1D() & {
      if constexpr(NonConstBaseType<Base>) {
         return StrictArrayMutable1D<SliceArrayBase1D<StrictArrayBase2D, seqN>>{*this,
                                                                          seqN{0, Base::size(), 1}};
      } else {
         return StrictArrayBase1D<ConstSliceArrayBase1D<StrictArrayBase2D, seqN>>{
             *this, seqN{0, Base::size(), 1}};
      }
   }

   // Implemented in attach2D.hpp.
   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) &;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <IndexType Index>
   STRICT_CONSTEXPR auto row(Index i) const& {
      auto ih = index_row_helper(*this, i);
      ASSERT_STRICT_DEBUG(valid_row(*this, ih));
      auto first = ih * Base::cols();
      return this->view1D()(seq{first, first + Base::cols() - 1_sl, 1});
   }

   template <IndexType Index>
   STRICT_CONSTEXPR auto col(Index j) const& {
      auto jh = index_col_helper(*this, j);
      ASSERT_STRICT_DEBUG(valid_col(*this, jh));
      auto first = jh;
      auto last = first + (Base::rows() - 1_sl) * Base::cols();
      return this->view1D()(seq{first, last, Base::cols()});
   }

   STRICT_CONSTEXPR auto diag() const& {
      return this->view1D()(seqN{0, mins(Base::rows(), Base::cols()), Base::cols() + 1_sl});
   }

   STRICT_CONSTEXPR auto diag(ImplicitInt i) const& {
      return this->view1D()(this->diag_slice_impl(i));
   }

   STRICT_CONSTEXPR auto view1D() const& {
      return StrictArrayBase1D<ConstSliceArrayBase1D<StrictArrayBase2D, seqN>>{*this,
                                                                         seqN{0, Base::size(), 1}};
   }

   // Implemented in attach2D.hpp.
   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) const&;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Must define for rvalues otherwise const& overload is selected, which returns
   // constant slice array in situations when non-constant slice array is needed.
   template <IndexType Index>
   STRICT_CONSTEXPR auto row(Index i) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->row(i);
   }

   template <IndexType Index>
   STRICT_CONSTEXPR auto col(Index j) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->col(j);
   }

   STRICT_CONSTEXPR auto diag() &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->diag();
   }

   STRICT_CONSTEXPR auto diag(ImplicitInt i) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->diag(i);
   }

   STRICT_CONSTEXPR auto view1D() &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->view1D();
   }

   // Implemented in attach2D.hpp.
   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <IndexType Index>
   STRICT_CONSTEXPR auto row(Index i) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <IndexType Index>
   STRICT_CONSTEXPR auto col(Index j) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto diag() const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto diag(ImplicitInt i) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto view1D() const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // For slices, initializer_list overloads are needed so that initializer_list
   // can be deduced implicitly.
   // SliceType is used to resolve ambiguity with operator() for indexing.
   template <SliceType Slice1, SliceType Slice2>
   STRICT_CONSTEXPR auto operator()(Slice1 row_slice, Slice2 col_slice) & {
      auto [s1h, s2h] = slice_row_col_helper(*this, std::move(row_slice), std::move(col_slice));
      if constexpr(NonConstBaseType<Base>) {
         return StrictArrayMutable2D<SliceArrayBase2D<StrictArrayBase2D, decltype(s1h), decltype(s2h)>>{
             *this, std::move(s1h), std::move(s2h)};
      } else {
         return StrictArrayBase2D<ConstSliceArrayBase2D<StrictArrayBase2D, decltype(s1h), decltype(s2h)>>{
             *this, std::move(s1h), std::move(s2h)};
      }
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, Slice col_slice) & {
      auto sh = slice_row_helper(*this, row_list);
      return operator()(std::move(sh), std::move(col_slice));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice row_slice, use::IndexList col_list) & {
      auto sh = slice_col_helper(*this, col_list);
      return operator()(std::move(row_slice), std::move(sh));
   }

   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, use::IndexList col_list) & {
      auto s1h = slice_row_helper(*this, row_list);
      auto s2h = slice_col_helper(*this, col_list);
      return operator()(std::move(s1h), std::move(s2h));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto rows(Slice slice) & {
      auto [s1h, s2h] = slice_row_col_helper(*this, std::move(slice), place::all);
      return operator()(std::move(s1h), std::move(s2h));
   }

   STRICT_CONSTEXPR auto rows(use::IndexList list) & {
      auto sh = slice_row_helper(*this, list);
      return this->rows(std::move(sh));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto cols(Slice slice) & {
      auto [s1h, s2h] = slice_row_col_helper(*this, place::all, std::move(slice));
      return operator()(std::move(s1h), std::move(s2h));
   }

   STRICT_CONSTEXPR auto cols(use::IndexList list) & {
      auto sh = slice_col_helper(*this, list);
      return this->cols(std::move(sh));
   }

   template <IndexType Index1, IndexType Index2, IndexType Index3, IndexType Index4>
   STRICT_CONSTEXPR auto block(Index1 first_row, Index2 first_col, Index3 last_row,
                               Index4 last_col) & {
      auto [i1, j1] = index_row_col_helper(*this, first_row, first_col);
      auto [i2, j2] = index_row_col_helper(*this, last_row, last_col);
      return operator()(seq{i1, i2}, seq{j1, j2});
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_CONSTEXPR auto blockN(Index1 first_row, Index2 first_col, ImplicitInt nrows,
                                ImplicitInt ncols) & {
      auto [i1, j1] = index_row_col_helper(*this, first_row, first_col);
      return operator()(seqN{i1, nrows.get()}, seqN{j1, ncols.get()});
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <SliceType Slice1, SliceType Slice2>
   STRICT_CONSTEXPR auto operator()(Slice1 row_slice, Slice2 col_slice) const& {
      auto [s1h, s2h] = slice_row_col_helper(*this, std::move(row_slice), std::move(col_slice));
      return StrictArrayBase2D<ConstSliceArrayBase2D<StrictArrayBase2D, decltype(s1h), decltype(s2h)>>{
          *this, std::move(s1h), std::move(s2h)};
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, Slice col_slice) const& {
      auto sh = slice_row_helper(*this, row_list);
      return operator()(std::move(sh), std::move(col_slice));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice row_slice, use::IndexList col_list) const& {
      auto sh = slice_col_helper(*this, col_list);
      return operator()(std::move(row_slice), std::move(sh));
   }

   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, use::IndexList col_list) const& {
      auto s1h = slice_row_helper(*this, row_list);
      auto s2h = slice_col_helper(*this, col_list);
      return operator()(std::move(s1h), std::move(s2h));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto rows(Slice slice) const& {
      auto [s1h, s2h] = slice_row_col_helper(*this, std::move(slice), place::all);
      return operator()(std::move(s1h), std::move(s2h));
   }

   STRICT_CONSTEXPR auto rows(use::IndexList list) const& {
      auto sh = slice_row_helper(*this, list);
      return this->rows(std::move(sh));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto cols(Slice slice) const& {
      auto [s1h, s2h] = slice_row_col_helper(*this, place::all, std::move(slice));
      return operator()(std::move(s1h), std::move(s2h));
   }

   STRICT_CONSTEXPR auto cols(use::IndexList list) const& {
      auto sh = slice_col_helper(*this, list);
      return this->cols(std::move(sh));
   }

   template <IndexType Index1, IndexType Index2, IndexType Index3, IndexType Index4>
   STRICT_CONSTEXPR auto block(Index1 first_row, Index2 first_col, Index3 last_row,
                               Index4 last_col) const& {
      auto [i1, j1] = index_row_col_helper(*this, first_row, first_col);
      auto [i2, j2] = index_row_col_helper(*this, last_row, last_col);
      return operator()(seq{i1, i2}, seq{j1, j2});
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_CONSTEXPR auto blockN(Index1 first_row, Index2 first_col, ImplicitInt nrows,
                                ImplicitInt ncols) const& {
      auto [i1, j1] = index_row_col_helper(*this, first_row, first_col);
      return operator()(seqN{i1, nrows.get()}, seqN{j1, ncols.get()});
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <SliceType Slice1, SliceType Slice2>
   STRICT_CONSTEXPR auto operator()(Slice1 row_slice, Slice2 col_slice) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return operator()(std::move(row_slice), std::move(col_slice));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice row_slice, use::IndexList col_list) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return operator()(std::move(row_slice), std::move(col_list));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, Slice col_slice) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return operator()(std::move(row_list), std::move(col_slice));
   }

   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, use::IndexList col_list) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return operator()(std::move(row_list), std::move(col_list));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto rows(Slice slice) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->rows(std::move(slice));
   }

   STRICT_CONSTEXPR auto rows(use::IndexList list) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->rows(std::move(list));
   }

   template <SliceType Slice>
   STRICT_CONSTEXPR auto cols(Slice slice) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->cols(std::move(slice));
   }

   STRICT_CONSTEXPR auto cols(use::IndexList list) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->cols(std::move(list));
   }

   template <IndexType Index1, IndexType Index2, IndexType Index3, IndexType Index4>
   STRICT_CONSTEXPR auto block(Index1 first_row, Index2 first_col, Index3 last_row,
                               Index4 last_col) &&
      requires(!ArrayTwoDimType<StrictArrayBase2D>)
   {
      return this->block(first_row, first_col, last_row, last_col);
   }

   template <IndexType Index1, IndexType Index2>
   STRICT_CONSTEXPR auto blockN(Index1 first_row, Index2 first_col, ImplicitInt nrows,
                                ImplicitInt ncols) &&
      requires(!ArrayTwoDimType< StrictArrayBase2D>)
   {
      return this->blockN(first_row, first_col, nrows, ncols);
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <SliceType Slice1, SliceType Slice2>
   STRICT_CONSTEXPR auto operator()(Slice1 row_slice, Slice2 col_slice) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice row_slice, use::IndexList col_list) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, Slice col_slice) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto operator()(use::IndexList row_list, use::IndexList col_list) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <SliceType Slice>
   STRICT_CONSTEXPR auto rows(Slice slice) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto rows(use::IndexList) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <SliceType Slice>
   STRICT_CONSTEXPR auto cols(Slice slice) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   STRICT_CONSTEXPR auto cols(use::IndexList) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <IndexType Index1, IndexType Index2, IndexType Index3, IndexType Index4>
   STRICT_CONSTEXPR auto block(Index1 first_row, Index2 first_col, Index3 last_row,
                               Index4 last_col) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   template <IndexType Index1, IndexType Index2>
   STRICT_CONSTEXPR auto blockN(Index1 first_row, Index2 first_col, ImplicitInt nrows,
                                ImplicitInt ncols) const&&
      requires ArrayTwoDimType<StrictArrayBase2D>
   = delete;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR StrictArrayBase2D& lval() & = delete;

   STRICT_CONSTEXPR const StrictArrayBase2D& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR StrictArrayBase2D& lval() && {
      return this->lval_impl();
   }

   STRICT_NODISCARD_CONSTEXPR const StrictArrayBase2D& lval() const&& {
      return this->lval_impl();
   }

   //  Return unaligned array so that it can be constexpr.
   STRICT_NODISCARD_CONSTEXPR Array2D<builtin_type, Unaligned> eval() const& {
      return Array2D<builtin_type, Unaligned>(*this);
   }

   STRICT_CONSTEXPR static index_t dimension() {
      return 2_sl;
   }

protected:
   STRICT_CONSTEXPR StrictArrayBase2D& lval_impl() {
      return *this;
   }

   STRICT_CONSTEXPR const StrictArrayBase2D& lval_impl() const {
      return *this;
   }

private:
   STRICT_CONSTEXPR auto diag_slice_impl(ImplicitInt i) const {
      if(i.get() >= 0_sl) {
         ASSERT_STRICT_DEBUG(i.get() < Base::cols());
         return seqN{i, mins(Base::rows(), Base::cols() - i.get()), Base::cols() + 1_sl};
      } else {
         ASSERT_STRICT_DEBUG(abss(i.get()) < Base::rows());
         return seqN{abss(i.get()) * Base::cols(),
                     mins(Base::rows() - abss(i.get()), Base::cols()),
                     Base::cols() + 1_sl};
      }
   }
};


template <TwoDimNonConstBaseType Base>
class STRICT_NODISCARD StrictArrayMutable2D : public StrictArrayBase2D<Base> {
   using CommonBase2D = StrictArrayBase2D<Base>;

public:
   using typename CommonBase2D::size_type;
   using typename CommonBase2D::builtin_type;
   using typename CommonBase2D::value_type;

   using StrictArrayBase2D<Base>::StrictArrayBase2D;
   STRICT_NODISCARD_CONSTEXPR StrictArrayMutable2D(const StrictArrayMutable2D&) = default;
   STRICT_NODISCARD_CONSTEXPR StrictArrayMutable2D(StrictArrayMutable2D&&) = default;

   // Assignments are not inherited because it returns reference to base class.
   STRICT_CONSTEXPR StrictArrayMutable2D& operator=(const StrictArrayMutable2D& A) {
      return static_cast<StrictArrayMutable2D&>(Base::operator=(A));
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator=(StrictArrayMutable2D&& A) {
      return static_cast<StrictArrayMutable2D&>(Base::operator=(static_cast<Base&&>(std::move(A))));
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator=(value_type x) {
      return static_cast<StrictArrayMutable2D&>(Base::operator=(x));
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator=(use::List2D<builtin_type> list) {
      return static_cast<StrictArrayMutable2D&>(Base::operator=(list));
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator=(TwoDimBaseType auto const& A) {
      return static_cast<StrictArrayMutable2D&>(Base::operator=(A));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR StrictArrayMutable2D& operator+=(value_type x) {
      CommonBase2D::view1D() += x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator-=(value_type x) {
      CommonBase2D::view1D() -= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator*=(value_type x) {
      CommonBase2D::view1D() *= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator/=(value_type x) {
      CommonBase2D::view1D() /= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator%=(value_type x)
      requires Integer<builtin_type>
   {
      CommonBase2D::view1D() %= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator<<=(value_type x)
      requires Integer<builtin_type>
   {
      CommonBase2D::view1D() <<= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator>>=(value_type x)
      requires Integer<builtin_type>
   {
      CommonBase2D::view1D() >>= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator&=(value_type x)
      requires Integer<builtin_type>
   {
      CommonBase2D::view1D() &= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator|=(value_type x)
      requires Integer<builtin_type>
   {
      CommonBase2D::view1D() |= x;
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator^=(value_type x)
      requires Integer<builtin_type>
   {
      CommonBase2D::view1D() ^= x;
      return *this;
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR StrictArrayMutable2D& operator+=(TwoDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() += A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator-=(TwoDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() -= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator*=(TwoDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() *= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator/=(TwoDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() /= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator%=(TwoDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() %= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator<<=(TwoDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() <<= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator>>=(TwoDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() >>= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator&=(TwoDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() &= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator|=(TwoDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() |= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& operator^=(TwoDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      CommonBase2D::view1D() ^= A.view1D();
      return *this;
   }

   STRICT_CONSTEXPR StrictArrayMutable2D& lval() & = delete;

   STRICT_CONSTEXPR const StrictArrayMutable2D& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR StrictArrayMutable2D& lval() && {
      return static_cast<StrictArrayMutable2D&>(CommonBase2D::lval_impl());
   }

   STRICT_NODISCARD_CONSTEXPR const StrictArrayMutable2D& lval() const&& {
      return static_cast<StrictArrayMutable2D&>(CommonBase2D::lval_impl());
   }
};


template <typename Base>
class STRICT_NODISCARD StrictArray2D final : public StrictArrayMutable2D<Base> {
   using CommonBase2D = StrictArrayBase2D<Base>;
   using MutableBase2D = StrictArrayMutable2D<Base>;

public:
   // static_assert is used instead of concept to avoid complications
   // with forward declarations in files that implement base classes.
   static_assert(ArrayTwoDimType<Base>);
   using size_type = MutableBase2D::size_type;
   using typename MutableBase2D::builtin_type;
   using typename MutableBase2D::value_type;

   using StrictArrayMutable2D<Base>::StrictArrayMutable2D;
   STRICT_NODISCARD_CONSTEXPR StrictArray2D() = default;
   STRICT_NODISCARD_CONSTEXPR StrictArray2D(const StrictArray2D&) = default;
   STRICT_NODISCARD_CONSTEXPR StrictArray2D(StrictArray2D&&) = default;

   // Assignments are not inherited because it returns reference to base class.
   // Further, for Arrays that own data lvalue qualifier for assignment is preferred.
   STRICT_CONSTEXPR StrictArray2D& operator=(const StrictArray2D& A) & {
      return static_cast<StrictArray2D&>(Base::operator=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator=(StrictArray2D&& A) & {
      return static_cast<StrictArray2D&>(Base::operator=(static_cast<Base&&>(std::move(A))));
   }

   STRICT_CONSTEXPR StrictArray2D& operator=(value_type x) & {
      return static_cast<StrictArray2D&>(Base::operator=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator=(use::List2D<builtin_type> list) & {
      return static_cast<StrictArray2D&>(Base::operator=(list));
   }

   STRICT_CONSTEXPR StrictArray2D& operator=(TwoDimBaseType auto const& A) & {
      return static_cast<StrictArray2D&>(Base::operator=(A));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR StrictArray2D& operator+=(value_type x) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator+=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator-=(value_type x) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator-=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator*=(value_type x) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator*=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator/=(value_type x) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator/=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator%=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<StrictArray2D&>(MutableBase2D::operator%=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator<<=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<StrictArray2D&>(MutableBase2D::operator<<=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator>>=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<StrictArray2D&>(MutableBase2D::operator>>=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator&=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<StrictArray2D&>(MutableBase2D::operator&=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator|=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<StrictArray2D&>(MutableBase2D::operator|=(x));
   }

   STRICT_CONSTEXPR StrictArray2D& operator^=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<StrictArray2D&>(MutableBase2D::operator^=(x));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR StrictArray2D& operator+=(TwoDimRealBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator+=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator-=(TwoDimRealBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator-=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator*=(TwoDimRealBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator*=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator/=(TwoDimRealBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator/=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator%=(TwoDimIntegerBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator%=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator<<=(TwoDimIntegerBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator<<=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator>>=(TwoDimIntegerBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator>>=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator&=(TwoDimIntegerBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator&=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator|=(TwoDimIntegerBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator|=(A));
   }

   STRICT_CONSTEXPR StrictArray2D& operator^=(TwoDimIntegerBaseType auto const& A) {
      return static_cast<StrictArray2D&>(MutableBase2D::operator^=(A));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR StrictLong bytes() const {
      return Base::size() * to_index_t(sizeof(value_type));
   }

   STRICT_CONSTEXPR Strict64 kbytes() const {
      return this->bytes().sd() / 1024._sd;
   }

   STRICT_CONSTEXPR Strict64 mbytes() const {
      return this->bytes().sd() / squares(1024_sl).sd();
   }

   STRICT_CONSTEXPR Strict64 gbytes() const {
      return this->bytes().sd() / cubes(1024_sl).sd();
   }

   STRICT_CONSTEXPR StrictArray2D& lval() & = delete;

   STRICT_CONSTEXPR const StrictArray2D& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR StrictArray2D& lval() && {
      return static_cast<StrictArray2D&>(CommonBase2D::lval_impl());
   }

   STRICT_NODISCARD_CONSTEXPR const StrictArray2D& lval() const&& {
      return static_cast<StrictArray2D&>(CommonBase2D::lval_impl());
   }
};


}  // namespace detail
}  // namespace spp

