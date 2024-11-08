// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "iterator.hpp"


namespace spp::detail {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, ImplicitIntStatic N>
class STRICT_NODISCARD FixedArrayBase1D : private ReferenceBase1D, private OneDimArrayBase {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   static_assert(N.get() > -1_sl);

   // Constructors.
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase1D();
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase1D(value_type x);
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase1D(Value<T> x);
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase1D(use::List1D<builtin_type> list);

   template <LinearIteratorType L>
   STRICT_NODISCARD_CONSTEXPR explicit FixedArrayBase1D(L b, L e);

   STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D(const FixedArrayBase1D& A);
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D(FixedArrayBase1D&& A) noexcept;
   STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D(OneDimBaseType auto const& A);

   // Assignments.
   STRICT_CONSTEXPR FixedArrayBase1D& operator=(value_type x);
   STRICT_CONSTEXPR FixedArrayBase1D& operator=(use::List1D<builtin_type> list);
   STRICT_CONSTEXPR FixedArrayBase1D& operator=(const FixedArrayBase1D& A);
   STRICT_CONSTEXPR FixedArrayBase1D& operator=(FixedArrayBase1D&& A) noexcept;
   STRICT_CONSTEXPR FixedArrayBase1D& operator=(OneDimBaseType auto const& A);

   STRICT_CONSTEXPR ~FixedArrayBase1D() = default;

   STRICT_CONSTEXPR_INLINE static index_t size();

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
   // Align to 512 byte boundary for AVX-512.
   alignas(512) value_type data_[to_size_t(N.get())];
};


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D() {
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(value_type x) {
   fill(x, *this);
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(Value<T> x)
    : FixedArrayBase1D(x.get()) {
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(
    use::List1D<builtin_type> list) {
   ASSERT_STRICT_DEBUG(N.get() == to_index_t(list.size()));
   copy(list, *this);
}


template <Builtin T, ImplicitIntStatic N>
template <LinearIteratorType L>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(L b, L e) {
   ASSERT_STRICT_DEBUG(index_t{e - b} == N.get());
   ASSERT_STRICT_DEBUG(e >= b);
   copy(b, e, *this);
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(const FixedArrayBase1D& A) {
   copy(A, *this);
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(FixedArrayBase1D&& A) noexcept
    : FixedArrayBase1D(A) {
   A = Zero<T>;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR FixedArrayBase1D<T, N>::FixedArrayBase1D(OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
}


template <Builtin T, ImplicitIntStatic N>
STRICT_CONSTEXPR FixedArrayBase1D<T, N>& FixedArrayBase1D<T, N>::operator=(value_type x) {
   fill(x, *this);
   return *this;
}


// Handles empty initializer list case as well.
template <Builtin T, ImplicitIntStatic N>
STRICT_CONSTEXPR FixedArrayBase1D<T, N>& FixedArrayBase1D<T, N>::operator=(
    use::List1D<builtin_type> list) {
   ASSERT_STRICT_DEBUG(N.get() == to_index_t(list.size()));
   copy(list, *this);
   return *this;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_CONSTEXPR FixedArrayBase1D<T, N>& FixedArrayBase1D<T, N>::operator=(
    const FixedArrayBase1D& A) {
   if(this != &A) {
      copy(A, *this);
   }
   return *this;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_CONSTEXPR FixedArrayBase1D<T, N>& FixedArrayBase1D<T, N>::operator=(
    FixedArrayBase1D&& A) noexcept {
   if(this != &A) {
      *this = A;
      A = Zero<T>;
   }
   return *this;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_CONSTEXPR FixedArrayBase1D<T, N>& FixedArrayBase1D<T, N>::operator=(
    OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_CONSTEXPR_INLINE index_t FixedArrayBase1D<T, N>::size() {
   return N.get();
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T>& FixedArrayBase1D<T, N>::un(ImplicitInt i) {
   return data_[i.get().val()];
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR_INLINE const Strict<T>& FixedArrayBase1D<T, N>::un(ImplicitInt i) const {
   return data_[i.get().val()];
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR auto FixedArrayBase1D<T, N>::data() -> value_type* {
   return this->size() != 0_sl ? data_ : nullptr;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD_CONSTEXPR auto FixedArrayBase1D<T, N>::data() const -> const value_type* {
   return this->size() != 0_sl ? data_ : nullptr;
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD auto FixedArrayBase1D<T, N>::blas_data() -> builtin_type*
   requires CompatibleBuiltin<T>
{
   return reinterpret_cast<T*>(this->size() != 0_sl ? data_ : nullptr);
}


template <Builtin T, ImplicitIntStatic N>
STRICT_NODISCARD auto FixedArrayBase1D<T, N>::blas_data() const -> const builtin_type*
   requires CompatibleBuiltin<T>
{
   return reinterpret_cast<const T*>(this->size() != 0_sl ? data_ : nullptr);
}


}  // namespace spp::detail
