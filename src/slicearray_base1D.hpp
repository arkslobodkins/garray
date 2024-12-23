// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "slice.hpp"


namespace spp::detail {


// Move assignments fall back to copy assignment for slice arrays since
// moving members would violate slicing semantics. Further,
// for linear slices none of the members are moveable.


// Note that Base can be either one or two-dimensional.
template <NonConstBaseType Base, typename Sl>
class STRICT_NODISCARD SliceArrayBase1D : private CopyBase1D {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit SliceArrayBase1D(Base& A, Sl slice_obj);

   STRICT_NODISCARD_CONSTEXPR SliceArrayBase1D(const SliceArrayBase1D& A) = default;
   STRICT_NODISCARD_CONSTEXPR SliceArrayBase1D(SliceArrayBase1D&& A) = default;
   STRICT_CONSTEXPR SliceArrayBase1D& operator=(const SliceArrayBase1D& A);
   STRICT_CONSTEXPR ~SliceArrayBase1D() = default;

   STRICT_CONSTEXPR SliceArrayBase1D& operator=(value_type x);
   STRICT_CONSTEXPR SliceArrayBase1D& operator=(use::List1D<builtin_type> list);
   STRICT_CONSTEXPR SliceArrayBase1D& operator=(OneDimBaseType auto const& A);

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR const auto& get_slice() const&;
   STRICT_NODISCARD_CONSTEXPR auto get_slice() &&;
   STRICT_NODISCARD_CONSTEXPR auto get_slice() const&&;

   STRICT_CONSTEXPR_INLINE index_t size() const;

private:
   typename CopyOrReferenceExpr<Base>::type A_;
   SliceArrayWrapper<Sl> slw_;
};


template <NonConstBaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR SliceArrayBase1D<Base, Sl>::SliceArrayBase1D(Base& A, Sl slice_obj)
    : A_{A},
      slw_{std::move(slice_obj)} {
   ASSERT_STRICT_DEBUG(slw_.valid(A_));
}


template <NonConstBaseType Base, typename Sl>
STRICT_CONSTEXPR SliceArrayBase1D<Base, Sl>& SliceArrayBase1D<Base, Sl>::operator=(value_type x) {
   fill(x, *this);
   return *this;
}


template <NonConstBaseType Base, typename Sl>
STRICT_CONSTEXPR SliceArrayBase1D<Base, Sl>& SliceArrayBase1D<Base, Sl>::operator=(
    use::List1D<builtin_type> list) {
   ASSERT_STRICT_DEBUG(size() == to_index_t(list.size()));
   copy(list, *this);
   return *this;
}


template <NonConstBaseType Base, typename Sl>
STRICT_CONSTEXPR SliceArrayBase1D<Base, Sl>& SliceArrayBase1D<Base, Sl>::operator=(
    const SliceArrayBase1D& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <NonConstBaseType Base, typename Sl>
STRICT_CONSTEXPR SliceArrayBase1D<Base, Sl>& SliceArrayBase1D<Base, Sl>::operator=(
    OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <NonConstBaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase1D<Base, Sl>::un(ImplicitInt i)
    -> value_type& {
   return A_.un(slw_.map(i));
}


template <NonConstBaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase1D<Base, Sl>::un(ImplicitInt i) const
    -> const value_type& {
   return A_.un(slw_.map(i));
}


template <NonConstBaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR const auto& SliceArrayBase1D<Base, Sl>::get_slice() const& {
   return slw_.get();
}


template <NonConstBaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR auto SliceArrayBase1D<Base, Sl>::get_slice() && {
   return std::move(slw_).get();
}


template <NonConstBaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR auto SliceArrayBase1D<Base, Sl>::get_slice() const&& {
   return slw_.get();
}


template <NonConstBaseType Base, typename Sl>
STRICT_CONSTEXPR_INLINE index_t SliceArrayBase1D<Base, Sl>::size() const {
   return slw_.size();
}


// Note that Base can be either one or two-dimensional.
template <BaseType Base, typename Sl>
class STRICT_NODISCARD ConstSliceArrayBase1D : private CopyBase1D, private ConstSliceBase {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit ConstSliceArrayBase1D(const Base& A, Sl slice_obj);

   STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase1D(const ConstSliceArrayBase1D& A) = default;
   STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase1D(ConstSliceArrayBase1D&& A) = default;
   STRICT_CONSTEXPR ConstSliceArrayBase1D& operator=(const ConstSliceArrayBase1D& A) = delete;
   STRICT_CONSTEXPR ~ConstSliceArrayBase1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR const auto& get_slice() const&;
   STRICT_NODISCARD_CONSTEXPR auto get_slice() &&;
   STRICT_NODISCARD_CONSTEXPR auto get_slice() const&&;

   STRICT_CONSTEXPR_INLINE index_t size() const;

private:
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   SliceArrayWrapper<Sl> slw_;
};


template <BaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase1D<Base, Sl>::ConstSliceArrayBase1D(const Base& A,
                                                                                  Sl slice_obj)
    : A_{A},
      slw_{std::move(slice_obj)} {
   ASSERT_STRICT_DEBUG(slw_.valid(A_));
}


template <BaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) ConstSliceArrayBase1D<Base, Sl>::un(
    ImplicitInt i) const {
   return A_.un(slw_.map(i));
}


template <BaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR const auto& ConstSliceArrayBase1D<Base, Sl>::get_slice() const& {
   return slw_.get();
}


template <BaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR auto ConstSliceArrayBase1D<Base, Sl>::get_slice() && {
   return std::move(slw_).get();
}


template <BaseType Base, typename Sl>
STRICT_NODISCARD_CONSTEXPR auto ConstSliceArrayBase1D<Base, Sl>::get_slice() const&& {
   return slw_.get();
}


template <BaseType Base, typename Sl>
STRICT_CONSTEXPR_INLINE index_t ConstSliceArrayBase1D<Base, Sl>::size() const {
   return slw_.size();
}


}  // namespace spp::detail
