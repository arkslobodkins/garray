// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_traits.hpp"
#include "StrictCommon/strict_common.hpp"
#include "derived1D.hpp"
#include "slice.hpp"


namespace spp {


namespace detail {


template <NonConstBaseType Base, typename Sl>
class SliceArrayBase1D;


template <BaseType Base, typename Sl>
class ConstSliceArrayBase1D;


template <Builtin T>
struct STRICT_NODISCARD strict_attach_ptr1D : private CopyBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD strict_attach_ptr1D(T* data, ImplicitInt n)
       : data_{reinterpret_cast<Strict<T>*>(data)},
         n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE value_type& un(ImplicitInt i) {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return n_;
   }

private:
   Strict<T>* data_;
   index_t n_;
};


template <Builtin T>
struct STRICT_NODISCARD const_strict_attach_ptr1D : private CopyBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   STRICT_NODISCARD const_strict_attach_ptr1D(const T* data, ImplicitInt n)
       : data_{reinterpret_cast<const Strict<T>*>(data)},
         n_{n.get()} {
   }

   STRICT_NODISCARD_INLINE const value_type& un(ImplicitInt i) const {
      return data_[i.get().val()];
   }

   STRICT_NODISCARD_INLINE auto size() const {
      return n_;
   }

private:
   const Strict<T>* data_;
   index_t n_;
};


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <detail::PointerConvertibleLvalue T>
auto attach1D(T&& data, ImplicitInt n) {
   using namespace detail;
   auto proxy = strict_attach_ptr1D(data, n);
   return StrictArrayMutable1D<SliceArrayBase1D<decltype(proxy), seqN>>{proxy, seqN{0, n}};
}


template <detail::PointerConvertibleLvalueConst T>
auto attach1D(T&& data, ImplicitInt n) {
   using namespace detail;
   auto proxy = const_strict_attach_ptr1D(data, n);
   return StrictArrayBase1D<ConstSliceArrayBase1D<decltype(proxy), seqN>>{proxy, seqN{0, n}};
}


}  // namespace spp

