// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_traits.hpp"
#include "StrictCommon/strict_common.hpp"
#include "derived1D.hpp"
#include "derived2D.hpp"
#include "slice.hpp"


namespace spp {


namespace detail {


template <TwoDimNonConstBaseType Base, typename Sl1, typename Sl2>
class SliceArrayBase2D;


template <TwoDimBaseType Base, typename Sl1, typename Sl2>
class ConstSliceArrayBase2D;


template <Builtin T>
struct STRICT_NODISCARD strict_attach_ptr2D : private CopyBase2D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD strict_attach_ptr2D(T* data, ImplicitInt m, ImplicitInt n)
       : data_{reinterpret_cast<Strict<T>*>(data)},
         m_{m.get()},
         n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE value_type& un(ImplicitInt i) {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE value_type& un(ImplicitInt i, ImplicitInt j) {
      return data_[i.get().val() * n_.val() + j.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const {
      return data_[i.get().val() * n_.val() + j.get().val()];
   }

   STRICT_NODISCARD_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return m_ * n_;
   }

private:
   Strict<T>* data_;
   index_t m_, n_;
};


template <Builtin T>
struct STRICT_NODISCARD const_strict_attach_ptr2D : private CopyBase2D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD const_strict_attach_ptr2D(const T* data, ImplicitInt m, ImplicitInt n)
       : data_{reinterpret_cast<const Strict<T>*>(data)},
         m_{m.get()},
         n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const {
      return data_[i.get().val() * n_.val() + j.get().val()];
   }

   STRICT_NODISCARD_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return m_ * n_;
   }

private:
   const Strict<T>* data_;
   index_t m_, n_;
};


// concept is BaseType since both 1-D and 2-D arrays must be handled.
template <BaseType Base>
struct STRICT_NODISCARD strict_convert_slice2D : private CopyBase2D {
public:
   using value_type = Base::value_type;
   using builtin_type = Base::builtin_type;

   STRICT_NODISCARD_CONSTEXPR strict_convert_slice2D(Base& A, ImplicitInt m, ImplicitInt n)
       : A_{A},
         m_{m.get()},
         n_{n.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR strict_convert_slice2D(const strict_convert_slice2D&) = default;

   // SliceArrayBase2D does not implement assignment based on the assignment of its members,
   // thus the following assignment is not strictly necessary, but might be useful for other
   // purposes.
   STRICT_NODISCARD_CONSTEXPR strict_convert_slice2D& operator=(const strict_convert_slice2D& S) {
      ASSERT_STRICT_DEBUG(same_size(*this, S));
      A_ = S.A_;
      return *this;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i) {
      return A_.un(i.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const {
      return A_.un(i.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i, ImplicitInt j) {
      return A_.un(i.get().val() * n_.val() + j.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const {
      return A_.un(i.get().val() * n_.val() + j.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto size() const {
      return m_ * n_;
   }

private:
   typename CopyOrReferenceExpr<Base>::type A_;
   index_t m_, n_;
};


// concept is BaseType since both 1-D and 2-D arrays must be handled.
template <BaseType Base>
struct STRICT_NODISCARD const_strict_convert_slice2D : private CopyBase2D {
public:
   using value_type = Base::value_type;
   using builtin_type = Base::builtin_type;

   STRICT_NODISCARD_CONSTEXPR const_strict_convert_slice2D(const Base& A, ImplicitInt m,
                                                           ImplicitInt n)
       : A_{A},
         m_{m.get()},
         n_{n.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR const_strict_convert_slice2D(const const_strict_convert_slice2D&)
       = default;
   STRICT_NODISCARD_CONSTEXPR const_strict_convert_slice2D& operator=(
       const const_strict_convert_slice2D&)
       = delete;

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i) const {
      return A_.un(i.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE decltype(auto) un(ImplicitInt i, ImplicitInt j) const {
      return A_.un(i.get().val() * n_.val() + j.get().val());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto rows() const {
      return m_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto cols() const {
      return n_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE auto size() const {
      return m_ * n_;
   }

private:
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   index_t m_, n_;
};


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR auto convert2D(Base& A, ImplicitInt m, ImplicitInt n) {
   auto proxy = strict_convert_slice2D{A, m, n};
   return GenArrayMutable2D<SliceArrayBase2D<decltype(proxy), seqN, seqN>>{
       proxy, seqN{0, m}, seqN{0, n}};
}


template <BaseType Base>
STRICT_NODISCARD_CONSTEXPR auto const_convert2D(const Base& A, ImplicitInt m, ImplicitInt n) {
   auto proxy = const_strict_convert_slice2D{A, m, n};
   return GenArrayBase2D<ConstSliceArrayBase2D<decltype(proxy), seqN, seqN>>{
       proxy, seqN{0, m}, seqN{0, n}};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base>
STRICT_CONSTEXPR auto GenArrayBase1D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) & {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   if constexpr(NonConstBaseType<GenArrayBase1D>) {
      return convert2D(*this, nrows, ncols);
   } else {
      return const_convert2D(*this, nrows, ncols);
   }
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto GenArrayBase1D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) const& {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   return const_convert2D(*this, nrows, ncols);
}


template <OneDimBaseType Base>
STRICT_CONSTEXPR auto GenArrayBase1D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) &&
   requires(!ArrayOneDimType<GenArrayBase1D>)
{
   return this->view2D(nrows, ncols);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto GenArrayBase2D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) & {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   if constexpr(NonConstBaseType<GenArrayBase2D>) {
      return convert2D(*this, nrows, ncols);
   } else {
      return const_convert2D(*this, nrows, ncols);
   }
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto GenArrayBase2D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) const& {
   ASSERT_STRICT_ALWAYS(nrows.get() * ncols.get() == Base::size());
   return const_convert2D(*this, nrows, ncols);
}


template <TwoDimBaseType Base>
STRICT_CONSTEXPR auto GenArrayBase2D<Base>::view2D(ImplicitInt nrows, ImplicitInt ncols) &&
   requires(!ArrayTwoDimType<GenArrayBase2D>)
{
   return this->view2D(nrows, ncols);
}


}  // namespace detail


///////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <CompatibleBuiltin T>
auto attach2D(T* data, ImplicitInt m, ImplicitInt n) {
   using namespace detail;
   auto proxy = strict_attach_ptr2D(data, m, n);
   return GenArrayMutable2D<SliceArrayBase2D<decltype(proxy), seqN, seqN>>{
       proxy, seqN{0, m}, seqN{0, n}};
}


template <CompatibleBuiltin T>
auto attach2D(const T* data, ImplicitInt m, ImplicitInt n) {
   using namespace detail;
   auto proxy = const_strict_attach_ptr2D(data, m, n);
   return GenArrayBase2D<ConstSliceArrayBase2D<decltype(proxy), seqN, seqN>>{
       proxy, seqN{0, m}, seqN{0, n}};
}


}  // namespace spp
