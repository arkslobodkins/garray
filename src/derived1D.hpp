// Arkadijs Slobodkins, 2023


#pragma once


#include <iterator>
#include <type_traits>
#include <utility>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "array_base1D.hpp"
#include "fixed_array_base1D.hpp"
#include "iterator.hpp"
#include "slice.hpp"
#include "slicearray_base1D.hpp"


namespace spp {


namespace detail {


template <OneDimNonConstBaseType Base>
class GenArrayMutable1D;


template <typename Base>
class GenArray1D;


}  // namespace detail


template <Builtin T, AlignmentFlag AF = Aligned>
using Array1D = detail::GenArray1D<detail::ArrayBase1D<T, AF>>;


template <Builtin T, ImplicitIntStatic sz>
using FixedArray1D = detail::GenArray1D<detail::FixedArrayBase1D<T, sz>>;


namespace detail {


template <OneDimBaseType Base>
class STRICT_NODISCARD GenArrayBase1D : public Base {
public:
   using size_type = index_t;
   using typename Base::builtin_type;
   using typename Base::value_type;

   using Base::Base;
   STRICT_NODISCARD_CONSTEXPR GenArrayBase1D(const GenArrayBase1D&) = default;
   STRICT_NODISCARD_CONSTEXPR GenArrayBase1D(GenArrayBase1D&&) = default;
   STRICT_CONSTEXPR GenArrayBase1D& operator=(const GenArrayBase1D&) = delete;
   STRICT_CONSTEXPR GenArrayBase1D& operator=(const auto&) = delete;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <IndexType Index>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator[](Index i) {
      ASSERT_STRICT_RANGE_DEBUG(valid_index(*this, index_helper(*this, i)));
      return Base::un(index_helper(*this, i));
   }

   template <IndexType Index>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) operator[](Index i) const {
      ASSERT_STRICT_RANGE_DEBUG(valid_index(*this, index_helper(*this, i)));
      return Base::un(index_helper(*this, i));
   }

   template <IndexType Index>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(Index i) {
      return Base::un(index_helper(*this, i));
   }

   template <IndexType Index>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(Index i) const {
      return Base::un(index_helper(*this, i));
   }

   template <IndexType Index>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) at(Index i) {
      ASSERT_STRICT_RANGE_ALWAYS(valid_index(*this, index_helper(*this, i)));
      return Base::un(index_helper(*this, i));
   }

   template <IndexType Index>
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) at(Index i) const {
      ASSERT_STRICT_RANGE_ALWAYS(valid_index(*this, index_helper(*this, i)));
      return Base::un(index_helper(*this, i));
   }

   STRICT_CONSTEXPR StrictBool empty() const {
      return Base::size() == 0_sl;
   }

   STRICT_CONSTEXPR index_t size_m1() const {
      return Base::size() - 1_sl;
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
   // For slices, initializer_list overloads are needed so that initializer_list
   // can be deduced implicitly.
   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice slice) & {
      auto sh = slice_helper(*this, std::move(slice));
      if constexpr(NonConstBaseType<Base>) {
         return GenArrayMutable1D<SliceArrayBase1D<GenArrayBase1D, decltype(sh)>>{*this,
                                                                                  std::move(sh)};
      } else {
         return GenArrayBase1D<ConstSliceArrayBase1D<GenArrayBase1D, decltype(sh)>>{*this,
                                                                                    std::move(sh)};
      }
   }

   STRICT_CONSTEXPR auto operator()(use::IndexList list) & {
      auto sh = slice_helper(*this, list);
      return operator()(std::move(sh));
   }

   STRICT_CONSTEXPR auto view1D() & {
      return operator()(place::all);
   }

   // Implemented in attach2D.hpp.
   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) &;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice slice) const& {
      auto sh = slice_helper(*this, std::move(slice));
      return GenArrayBase1D<ConstSliceArrayBase1D<GenArrayBase1D, decltype(sh)>>{*this,
                                                                                 std::move(sh)};
   }

   STRICT_CONSTEXPR auto operator()(use::IndexList list) const& {
      auto sh = slice_helper(*this, list);
      return operator()(std::move(sh));
   }

   STRICT_CONSTEXPR auto view1D() const& {
      return operator()(place::all);
   }

   // Implemented in attach2D.hpp.
   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) const&;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Must define for rvalues otherwise const& overload is selected, which returns
   // constant slice array in situations when non-constant slice array is needed.
   template <SliceType Slice>
   STRICT_CONSTEXPR auto operator()(Slice slice) &&
      requires(!ArrayOneDimType<GenArrayBase1D>)
   {
      return operator()(std::move(slice));
   }

   STRICT_CONSTEXPR auto operator()(use::IndexList list) && {
      return operator()(std::move(list));
   }

   STRICT_CONSTEXPR auto view1D() &&
      requires(!ArrayOneDimType<GenArrayBase1D>)
   {
      return this->view1D();
   }

   // Implemented in attach2D.hpp.
   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) &&
      requires(!ArrayOneDimType<GenArrayBase1D>);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   // Disallow slicing of temporaries that own data to reduce the risk of dangling references.
   template <typename T>
   STRICT_CONSTEXPR auto operator()(T slice) const&&
      requires ArrayOneDimType<GenArrayBase1D>
   = delete;

   STRICT_CONSTEXPR auto view1D() const&&
      requires ArrayOneDimType<GenArrayBase1D>
   = delete;

   STRICT_CONSTEXPR auto view2D(ImplicitInt nrows, ImplicitInt ncols) const&&
      requires ArrayOneDimType<GenArrayBase1D>
   = delete;

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR GenArrayBase1D& lval() & = delete;

   STRICT_CONSTEXPR const GenArrayBase1D& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR GenArrayBase1D& lval() && {
      return this->lval_impl();
   }

   STRICT_NODISCARD_CONSTEXPR const GenArrayBase1D& lval() const&& {
      return this->lval_impl();
   }

   //  Return unaligned array so that it can be constexpr.
   STRICT_NODISCARD_CONSTEXPR Array1D<builtin_type, Unaligned> eval() const& {
      return Array1D<builtin_type, Unaligned>(*this);
   }

   STRICT_CONSTEXPR static index_t dimension() {
      return 1_sl;
   }

protected:
   STRICT_CONSTEXPR GenArrayBase1D& lval_impl() {
      return *this;
   }

   STRICT_CONSTEXPR const GenArrayBase1D& lval_impl() const {
      return *this;
   }
};


template <OneDimNonConstBaseType Base>
class STRICT_NODISCARD GenArrayMutable1D : public GenArrayBase1D<Base> {
   using CommonBase1D = GenArrayBase1D<Base>;

public:
   using typename CommonBase1D::size_type;
   using typename CommonBase1D::builtin_type;
   using typename CommonBase1D::value_type;

   using GenArrayBase1D<Base>::GenArrayBase1D;
   STRICT_NODISCARD_CONSTEXPR GenArrayMutable1D(const GenArrayMutable1D&) = default;
   STRICT_NODISCARD_CONSTEXPR GenArrayMutable1D(GenArrayMutable1D&&) = default;

   // Assignments are not inherited because it returns reference to base class.
   STRICT_CONSTEXPR GenArrayMutable1D& operator=(const GenArrayMutable1D& A) {
      return static_cast<GenArrayMutable1D&>(Base::operator=(A));
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator=(GenArrayMutable1D&& A) {
      return static_cast<GenArrayMutable1D&>(Base::operator=(static_cast<Base&&>(std::move(A))));
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator=(value_type x) {
      return static_cast<GenArrayMutable1D&>(Base::operator=(x));
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator=(use::List1D<builtin_type> list) {
      return static_cast<GenArrayMutable1D&>(Base::operator=(list));
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator=(OneDimBaseType auto const& A) {
      return static_cast<GenArrayMutable1D&>(Base::operator=(A));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR GenArrayMutable1D& operator+=(value_type x) {
      apply0(*this, [x, this](index_t i) { Base::un(i) += x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator-=(value_type x) {
      apply0(*this, [x, this](index_t i) { Base::un(i) -= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator*=(value_type x) {
      apply0(*this, [x, this](index_t i) { Base::un(i) *= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator/=(value_type x) {
      apply0(*this, [x, this](index_t i) { Base::un(i) /= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator%=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(*this, [x, this](index_t i) { Base::un(i) %= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator<<=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(*this, [x, this](index_t i) { Base::un(i) <<= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator>>=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(*this, [x, this](index_t i) { Base::un(i) >>= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator&=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(*this, [x, this](index_t i) { Base::un(i) &= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator|=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(*this, [x, this](index_t i) { Base::un(i) |= x; });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator^=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(*this, [x, this](index_t i) { Base::un(i) ^= x; });
      return *this;
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR GenArrayMutable1D& operator+=(OneDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) += A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator-=(OneDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) -= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator*=(OneDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) *= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator/=(OneDimRealBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) /= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator%=(OneDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) %= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator<<=(OneDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) <<= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator>>=(OneDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) >>= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator&=(OneDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) &= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator|=(OneDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) |= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& operator^=(OneDimIntegerBaseType auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      apply1(*this, A, [&](index_t i) { Base::un(i) ^= A.un(i); });
      return *this;
   }

   STRICT_CONSTEXPR GenArrayMutable1D& lval() & = delete;

   STRICT_CONSTEXPR const GenArrayMutable1D& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR GenArrayMutable1D& lval() && {
      return static_cast<GenArrayMutable1D&>(CommonBase1D::lval_impl());
   }

   STRICT_NODISCARD_CONSTEXPR const GenArrayMutable1D& lval() const&& {
      return static_cast<GenArrayMutable1D&>(CommonBase1D::lval_impl());
   }
};


template <typename Base>
class STRICT_NODISCARD GenArray1D final : public GenArrayMutable1D<Base> {
   using CommonBase1D = GenArrayBase1D<Base>;
   using MutableBase1D = GenArrayMutable1D<Base>;

public:
   // static_assert is used instead of concept to avoid complications
   // with forward declarations in files that implement base classes.
   static_assert(ArrayOneDimType<Base>);
   using size_type = MutableBase1D::size_type;
   using typename MutableBase1D::builtin_type;
   using typename MutableBase1D::value_type;

   using GenArrayMutable1D<Base>::GenArrayMutable1D;
   // Compiler would generate non-explicit default.
   STRICT_NODISCARD_CONSTEXPR explicit GenArray1D() = default;
   STRICT_NODISCARD_CONSTEXPR GenArray1D(const GenArray1D&) = default;
   STRICT_NODISCARD_CONSTEXPR GenArray1D(GenArray1D&&) = default;

   // Assignments are not inherited because it returns reference to base class.
   // Further, for Arrays that own data lvalue qualifier for assignment is preferred.
   STRICT_CONSTEXPR GenArray1D& operator=(const GenArray1D& A) & {
      return static_cast<GenArray1D&>(Base::operator=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator=(GenArray1D&& A) & {
      return static_cast<GenArray1D&>(Base::operator=(static_cast<Base&&>(std::move(A))));
   }

   STRICT_CONSTEXPR GenArray1D& operator=(value_type x) & {
      return static_cast<GenArray1D&>(Base::operator=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator=(use::List1D<builtin_type> list) & {
      return static_cast<GenArray1D&>(Base::operator=(list));
   }

   STRICT_CONSTEXPR GenArray1D& operator=(OneDimBaseType auto const& A) & {
      return static_cast<GenArray1D&>(Base::operator=(A));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR GenArray1D& operator+=(value_type x) {
      return static_cast<GenArray1D&>(MutableBase1D::operator+=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator-=(value_type x) {
      return static_cast<GenArray1D&>(MutableBase1D::operator-=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator*=(value_type x) {
      return static_cast<GenArray1D&>(MutableBase1D::operator*=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator/=(value_type x) {
      return static_cast<GenArray1D&>(MutableBase1D::operator/=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator%=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<GenArray1D&>(MutableBase1D::operator%=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator<<=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<GenArray1D&>(MutableBase1D::operator<<=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator>>=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<GenArray1D&>(MutableBase1D::operator>>=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator&=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<GenArray1D&>(MutableBase1D::operator&=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator|=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<GenArray1D&>(MutableBase1D::operator|=(x));
   }

   STRICT_CONSTEXPR GenArray1D& operator^=(value_type x)
      requires Integer<builtin_type>
   {
      return static_cast<GenArray1D&>(MutableBase1D::operator^=(x));
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR GenArray1D& operator+=(OneDimRealBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator+=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator-=(OneDimRealBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator-=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator*=(OneDimRealBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator*=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator/=(OneDimRealBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator/=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator%=(OneDimIntegerBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator%=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator<<=(OneDimIntegerBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator<<=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator>>=(OneDimIntegerBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator>>=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator&=(OneDimIntegerBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator&=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator|=(OneDimIntegerBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator|=(A));
   }

   STRICT_CONSTEXPR GenArray1D& operator^=(OneDimIntegerBaseType auto const& A) {
      return static_cast<GenArray1D&>(MutableBase1D::operator^=(A));
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

   STRICT_CONSTEXPR GenArray1D& lval() & = delete;

   STRICT_CONSTEXPR const GenArray1D& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR GenArray1D& lval() && {
      return static_cast<GenArray1D&>(CommonBase1D::lval_impl());
   }

   STRICT_NODISCARD_CONSTEXPR const GenArray1D& lval() const&& {
      return static_cast<GenArray1D&>(CommonBase1D::lval_impl());
   }
};


}  // namespace detail


}  // namespace spp

