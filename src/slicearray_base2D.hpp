// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "slice.hpp"


namespace spp::detail {


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
class STRICT_NODISCARD SliceArrayBase2D : private CopyBase2D {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit SliceArrayBase2D(Base& A, Sl1 sl1, Sl2 sl2);

   STRICT_NODISCARD_CONSTEXPR SliceArrayBase2D(const SliceArrayBase2D& A) = default;
   STRICT_CONSTEXPR SliceArrayBase2D& operator=(const SliceArrayBase2D& A);
   STRICT_CONSTEXPR ~SliceArrayBase2D() = default;

   STRICT_CONSTEXPR SliceArrayBase2D& operator=(value_type x);
   STRICT_CONSTEXPR SliceArrayBase2D& operator=(use::List2D<builtin_type> list);
   STRICT_CONSTEXPR SliceArrayBase2D& operator=(TwoDimBaseType auto const& A);

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i, ImplicitInt j);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const;

   STRICT_CONSTEXPR_INLINE index_t rows() const;
   STRICT_CONSTEXPR_INLINE index_t cols() const;
   STRICT_CONSTEXPR_INLINE index_t size() const;

private:
   typename CopyOrReferenceExpr<Base>::type A_;
   SliceArrayWrapper<Sl1> slw1_;
   SliceArrayWrapper<Sl2> slw2_;
};


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR SliceArrayBase2D<Base, Sl1, Sl2>::SliceArrayBase2D(Base& A, Sl1 sl1,
                                                                              Sl2 sl2)
    : A_{A},
      slw1_{std::move(sl1)},
      slw2_{std::move(sl2)} {
   ASSERT_STRICT_DEBUG(slw1_.valid_first(A_));
   ASSERT_STRICT_DEBUG(slw2_.valid_second(A_));
   // Must be either both empty or non-empty.
   ASSERT_STRICT_DEBUG(!((slw1_.size() == 0_sl) ^ (slw2_.size() == 0_sl)));
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR SliceArrayBase2D<Base, Sl1, Sl2>& SliceArrayBase2D<Base, Sl1, Sl2>::operator=(
    value_type x) {
   fill(x, *this);
   return *this;
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR SliceArrayBase2D<Base, Sl1, Sl2>& SliceArrayBase2D<Base, Sl1, Sl2>::operator=(
    use::List2D<builtin_type> list) {
   ASSERT_STRICT_DEBUG(valid_list2D(list));
   auto [nrows, ncols] = list2D_row_col_sizes(list);
   ASSERT_STRICT_DEBUG(this->rows() == nrows);
   ASSERT_STRICT_DEBUG(this->cols() == ncols);
   copy(list, *this);

   return *this;
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR SliceArrayBase2D<Base, Sl1, Sl2>& SliceArrayBase2D<Base, Sl1, Sl2>::operator=(
    const SliceArrayBase2D& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR SliceArrayBase2D<Base, Sl1, Sl2>& SliceArrayBase2D<Base, Sl1, Sl2>::operator=(
    TwoDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase2D<Base, Sl1, Sl2>::un(ImplicitInt i)
    -> value_type& {
   auto [r, c] = index_map_one_to_two_dim(*this, i);
   return this->un(r, c);
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase2D<Base, Sl1, Sl2>::un(ImplicitInt i) const
    -> const value_type& {
   auto [r, c] = index_map_one_to_two_dim(*this, i);
   return this->un(r, c);
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase2D<Base, Sl1, Sl2>::un(ImplicitInt i,
                                                                            ImplicitInt j)
    -> value_type& {
   return A_.un(slw1_.map(i), slw2_.map(j));
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR_INLINE auto SliceArrayBase2D<Base, Sl1, Sl2>::un(ImplicitInt i,
                                                                            ImplicitInt j) const
    -> const value_type& {
   return A_.un(slw1_.map(i), slw2_.map(j));
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR_INLINE index_t SliceArrayBase2D<Base, Sl1, Sl2>::rows() const {
   return slw1_.size();
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR_INLINE index_t SliceArrayBase2D<Base, Sl1, Sl2>::cols() const {
   return slw2_.size();
}


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR_INLINE index_t SliceArrayBase2D<Base, Sl1, Sl2>::size() const {
   return slw1_.size() * slw2_.size();
}


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
class STRICT_NODISCARD ConstSliceArrayBase2D : private CopyBase2D, private ConstSliceBase {
public:
   using value_type = ValueTypeOf<Base>;
   using builtin_type = BuiltinTypeOf<Base>;

   STRICT_NODISCARD_CONSTEXPR explicit ConstSliceArrayBase2D(const Base& A, Sl1 sl1, Sl2 sl2);

   STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase2D(const ConstSliceArrayBase2D& A) = default;
   STRICT_CONSTEXPR ConstSliceArrayBase2D& operator=(const ConstSliceArrayBase2D& A) = delete;
   STRICT_CONSTEXPR ~ConstSliceArrayBase2D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i, ImplicitInt j) const;
   STRICT_CONSTEXPR_INLINE index_t rows() const;
   STRICT_CONSTEXPR_INLINE index_t cols() const;
   STRICT_CONSTEXPR_INLINE index_t size() const;

private:
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   SliceArrayWrapper<Sl1> slw1_;
   SliceArrayWrapper<Sl2> slw2_;
};


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR ConstSliceArrayBase2D<Base, Sl1, Sl2>::ConstSliceArrayBase2D(
    const Base& A, Sl1 sl1, Sl2 sl2)
    : A_{A},
      slw1_{std::move(sl1)},
      slw2_{std::move(sl2)} {
   ASSERT_STRICT_DEBUG(slw1_.valid_first(A_));
   ASSERT_STRICT_DEBUG(slw2_.valid_second(A_));
   // Must be either both empty or non-empty.
   ASSERT_STRICT_DEBUG(!((slw1_.size() == 0_sl) ^ (slw2_.size() == 0_sl)));
}


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) ConstSliceArrayBase2D<Base, Sl1, Sl2>::un(
    ImplicitInt i) const {
   auto [r, c] = index_map_one_to_two_dim(*this, i);
   return this->un(r, c);
}


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) ConstSliceArrayBase2D<Base, Sl1, Sl2>::un(
    ImplicitInt i, ImplicitInt j) const {
   return A_.un(slw1_.map(i), slw2_.map(j));
}


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR_INLINE index_t ConstSliceArrayBase2D<Base, Sl1, Sl2>::rows() const {
   return slw1_.size();
}


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR_INLINE index_t ConstSliceArrayBase2D<Base, Sl1, Sl2>::cols() const {
   return slw2_.size();
}


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
STRICT_CONSTEXPR_INLINE index_t ConstSliceArrayBase2D<Base, Sl1, Sl2>::size() const {
   return slw1_.size() * slw2_.size();
}


}  // namespace spp::detail

