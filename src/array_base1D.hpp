// Arkadijs Slobodkins, 2023


#pragma once


#include <new>
#include <utility>
#include <vector>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "iterator.hpp"


namespace spp {


enum AlignmentFlag { Aligned, Unaligned };


namespace detail {


template <typename Base>
class GenArray1D;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
class STRICT_NODISCARD ArrayBase1D : private ReferenceBase1D, private OneDimArrayBase {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // Constructors.
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D();
   STRICT_NODISCARD explicit ArrayBase1D(ImplicitInt n)
      requires(AF == Aligned);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(ImplicitInt n)
      requires(AF == Unaligned);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(Size n);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(ImplicitInt n, value_type x);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(Size n, Value<T> x);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(use::List1D<builtin_type> list);

   template <LinearIteratorType L>
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase1D(L b, L e);

   STRICT_NODISCARD_CONSTEXPR ArrayBase1D(const ArrayBase1D& A);
   STRICT_NODISCARD_CONSTEXPR ArrayBase1D(ArrayBase1D&& A) noexcept;
   STRICT_NODISCARD_CONSTEXPR ArrayBase1D(OneDimBaseType auto const& A);

   // Assignments.
   STRICT_CONSTEXPR ArrayBase1D& operator=(value_type x);
   STRICT_CONSTEXPR ArrayBase1D& operator=(use::List1D<builtin_type> list);
   STRICT_CONSTEXPR ArrayBase1D& operator=(const ArrayBase1D& A);
   STRICT_CONSTEXPR ArrayBase1D& operator=(ArrayBase1D&& A) noexcept;
   STRICT_CONSTEXPR ArrayBase1D& operator=(OneDimBaseType auto const& A);

   ~ArrayBase1D()
      requires(AF == Aligned);
   STRICT_CONSTEXPR ~ArrayBase1D()
      requires(AF == Unaligned);

   STRICT_CONSTEXPR void swap(ArrayBase1D& A) noexcept;
   STRICT_CONSTEXPR void swap(ArrayBase1D&& A) noexcept;

   STRICT_CONSTEXPR auto& resize(ImplicitInt n);
   STRICT_CONSTEXPR auto& resize_forget(ImplicitInt n);

   STRICT_CONSTEXPR auto& resize_and_assign(OneDimBaseType auto const& A);

   // Optimized implementation.
   STRICT_CONSTEXPR auto& resize_and_assign(GenArray1D<ArrayBase1D>&& A);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR auto& remove(ImplicitInt pos, ImplicitInt count = 1);
   STRICT_CONSTEXPR auto& remove(Pos pos, Count count = Count{1});
   STRICT_CONSTEXPR auto& remove_front(ImplicitInt count = 1);
   STRICT_CONSTEXPR auto& remove_front(Count count);
   STRICT_CONSTEXPR auto& remove_back(ImplicitInt count = 1);
   STRICT_CONSTEXPR auto& remove_back(Count count);

   STRICT_CONSTEXPR auto& remove(Last lst);
   STRICT_CONSTEXPR auto& remove(const std::vector<ImplicitInt>& indexes);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR auto& insert(ImplicitInt pos, value_type x);
   STRICT_CONSTEXPR auto& insert(Pos pos, Value<builtin_type> x);
   STRICT_CONSTEXPR auto& insert_front(value_type x);
   STRICT_CONSTEXPR auto& insert_front(Value<builtin_type> x);
   STRICT_CONSTEXPR auto& insert_back(value_type x);
   STRICT_CONSTEXPR auto& insert_back(Value<builtin_type> x);

   STRICT_CONSTEXPR auto& insert(ImplicitInt pos, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert(Pos pos, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_front(OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_back(OneDimBaseType auto const& A);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR_INLINE index_t size() const;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const;

   STRICT_NODISCARD_CONSTEXPR value_type* data();
   STRICT_NODISCARD_CONSTEXPR const value_type* data() const;

   // Converting to built-in types requires reinterpret_cast,
   // which cannot be done at compile time(not constexpr).
   STRICT_NODISCARD builtin_type* blas_data()
      requires CompatibleBuiltin<T>;
   STRICT_NODISCARD const builtin_type* blas_data() const
      requires CompatibleBuiltin<T>;

private:
   value_type* data_;
   index_t n_;
};


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D() : data_{nullptr},
                                                               n_{} {
}


// Align to 512 byte boundary for AVX-512.
template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD ArrayBase1D<T, AF>::ArrayBase1D(ImplicitInt n)
   requires(AF == Aligned)
    : data_{nullptr},
      n_{n.get()} {
   ASSERT_STRICT_DEBUG(n_ > -1_sl);
   if(n_ != 0_sl) {
      data_ = new(std::align_val_t{512}) value_type[to_size_t(n_)];
   }
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(ImplicitInt n)
   requires(AF == Unaligned)
    : data_{nullptr},
      n_{n.get()} {
   ASSERT_STRICT_DEBUG(n_ > -1_sl);
   if(n_ != 0_sl) {
      data_ = new value_type[to_size_t(n_)];
   }
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(Size n) : ArrayBase1D(n.get()) {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(ImplicitInt n, Strict<T> x)
    : ArrayBase1D(n) {
   fill(x, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(Size n, Value<T> x)
    : ArrayBase1D(n.get(), x.get()) {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(use::List1D<builtin_type> list)
    : ArrayBase1D(to_index_t(list.size())) {
   copy(list, *this);
}


// Passes absolute value of e - b so that assertion e >= b can provide more detail.
template <Builtin T, AlignmentFlag AF>
template <LinearIteratorType L>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(L b, L e)
    : ArrayBase1D(abss(Strict{e - b})) {
   ASSERT_STRICT_DEBUG(e >= b);
   copy(b, e, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(const ArrayBase1D& A)
    : ArrayBase1D(A.size()) {
   copy(A, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(ArrayBase1D&& A) noexcept
    : data_{std::exchange(A.data_, nullptr)},
      n_{std::exchange(A.n_, 0_sl)} {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase1D<T, AF>::ArrayBase1D(OneDimBaseType auto const& A)
    : ArrayBase1D(A.size()) {
   copy(A, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(Strict<T> x) {
   fill(x, *this);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(use::List1D<builtin_type> list) {
   ASSERT_STRICT_DEBUG(this->size() == to_index_t(list.size()));
   copy(list, *this);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(const ArrayBase1D& A) {
   if(this != &A) {
      ASSERT_STRICT_DEBUG(same_size(*this, A));
      copy(A, *this);
   }
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(ArrayBase1D&& A) noexcept {
   if(this != &A) {
      NORMAL_ASSERT_STRICT_DEBUG(same_size(*this, A));
      this->swap(A);
      A.swap(ArrayBase1D{});
   }
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>& ArrayBase1D<T, AF>::operator=(OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
ArrayBase1D<T, AF>::~ArrayBase1D()
   requires(AF == Aligned)
{
   operator delete[](data_, std::align_val_t{512});
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase1D<T, AF>::~ArrayBase1D()
   requires(AF == Unaligned)
{
   delete[] data_;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR void ArrayBase1D<T, AF>::swap(ArrayBase1D& A) noexcept {
   std::swap(data_, A.data_);
   std::swap(n_, A.n_);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR void ArrayBase1D<T, AF>::swap(ArrayBase1D&& A) noexcept {
   this->swap(A);
}


// Preserves values of the remaining elements.
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::resize(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);

   if(auto n_new = n.get(); n_new != n_) {
      ArrayBase1D tmp(n_new);
      copyn(*this, tmp, mins(n_, n_new));
      this->swap(tmp);
   }
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


// Does not preserve values of the remaining elements.
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::resize_forget(ImplicitInt n) {
   ASSERT_STRICT_DEBUG(n.get() > -1_sl);

   if(auto n_new = n.get(); n_new != n_) {
      ArrayBase1D tmp(n_new);
      this->swap(tmp);
   }
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::resize_and_assign(OneDimBaseType auto const& A) {
   ArrayBase1D tmp(A);
   this->swap(tmp);
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::resize_and_assign(GenArray1D<ArrayBase1D>&& A) {
   this->swap(A);
   A.swap(ArrayBase1D{});
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove(ImplicitInt pos, ImplicitInt count) {
   ASSERT_STRICT_DEBUG(count.get() > 0_sl);
   ASSERT_STRICT_DEBUG(valid_index(*this, pos.get()));
   ASSERT_STRICT_DEBUG(valid_index(*this, pos.get() + count.get() - 1_sl));

   ArrayBase1D tmp(this->size() - count.get());
   copyn(*this, tmp, pos.get());
   copyn(*this, tmp, pos.get() + count.get(), pos.get(), this->size() - pos.get() - count.get());
   this->swap(tmp);
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove(Pos pos, Count count) {
   return this->remove(pos.get(), count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove_front(ImplicitInt count) {
   return this->remove(0, count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove_front(Count count) {
   return this->remove_front(count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove_back(ImplicitInt count) {
   return this->remove(this->size() - count.get(), count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove_back(Count count) {
   return this->remove_back(count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove(Last lst) {
   return this->remove(this->size() - 1_sl - lst.get(), 1);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::remove(const std::vector<ImplicitInt>& indexes) {
   if(!indexes.empty()) {
      auto ci = complement_index_vector(
          valid_index<RemoveCVRef<decltype(*this)>>, this->size(), *this, indexes);
      ArrayBase1D tmp(to_index_t(ci.size()));

      for(index_t i = 0_sl; i < tmp.size(); ++i) {
         tmp.un(i) = (*this).un(ci[to_size_t(i)]);
      }
      this->swap(tmp);
   }

   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert(ImplicitInt pos, value_type x) {
   ASSERT_STRICT_DEBUG(pos.get() >= 0_sl && pos.get() <= this->size());
   ArrayBase1D tmp(this->size() + 1_sl);
   copyn(*this, tmp, pos.get());
   tmp.un(pos.get()) = x;
   copyn(*this, tmp, pos.get(), pos.get() + 1_sl, this->size() - pos.get());

   this->swap(tmp);
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert(Pos pos, Value<builtin_type> x) {
   return this->insert(pos.get(), x.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert_front(value_type x) {
   return this->insert(0, x);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert_front(Value<builtin_type> x) {
   return this->insert_front(x.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert_back(value_type x) {
   return this->insert(this->size(), x);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert_back(Value<builtin_type> x) {
   return this->insert_back(x.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert(ImplicitInt pos, OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(pos.get() >= 0_sl && pos.get() <= this->size());
   ArrayBase1D tmp(this->size() + A.size());
   copyn(*this, tmp, pos.get());
   copyn(A, tmp, 0_sl, pos.get(), A.size());
   copyn(*this, tmp, pos.get(), pos.get() + A.size(), this->size() - pos.get());

   this->swap(tmp);
   return static_cast<GenArray1D<ArrayBase1D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert(Pos pos, OneDimBaseType auto const& A) {
   return this->insert(pos.get(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert_front(OneDimBaseType auto const& A) {
   return this->insert(0, A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase1D<T, AF>::insert_back(OneDimBaseType auto const& A) {
   return this->insert(this->size(), A);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t ArrayBase1D<T, AF>::size() const {
   return n_;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T>& ArrayBase1D<T, AF>::un(ImplicitInt i) {
   return data_[i.get().val()];
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE const Strict<T>& ArrayBase1D<T, AF>::un(ImplicitInt i) const {
   return data_[i.get().val()];
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto ArrayBase1D<T, AF>::data() -> value_type* {
   return this->size() != 0_sl ? data_ : nullptr;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto ArrayBase1D<T, AF>::data() const -> const value_type* {
   return this->size() != 0_sl ? data_ : nullptr;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD auto ArrayBase1D<T, AF>::blas_data() -> builtin_type*
   requires CompatibleBuiltin<T>
{
   return reinterpret_cast<T*>(this->size() != 0_sl ? data_ : nullptr);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD auto ArrayBase1D<T, AF>::blas_data() const -> const builtin_type*
   requires CompatibleBuiltin<T>
{
   return reinterpret_cast<const T*>(this->size() != 0_sl ? data_ : nullptr);
}


}  // namespace detail
}  // namespace spp
