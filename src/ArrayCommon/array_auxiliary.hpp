// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/config.hpp"
#include "../StrictCommon/strict_literals.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"
#include "array_traits.hpp"


namespace spp {


template <BaseType Base, Builtin U>
STRICT_CONSTEXPR_INLINE auto value_type_cast(U x) {
   return strict_cast<BuiltinTypeOf<Base>>(x);
}


template <BaseType Base, Builtin U>
STRICT_CONSTEXPR_INLINE auto value_type_cast(Strict<U> x) {
   return strict_cast<BuiltinTypeOf<Base>>(x);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base1, OneDimBaseType Base2, OneDimBaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR StrictBool same_size(const Base1& A1, const Base2& A2,
                                                const BArgs&... AArgs) {
   if constexpr(sizeof...(BArgs) > 0) {
      return A1.size() == A2.size() && same_size(A2, AArgs...);
   }
   return A1.size() == A2.size();
}


template <TwoDimBaseType Base1, TwoDimBaseType Base2, TwoDimBaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR StrictBool same_size(const Base1& A1, const Base2& A2,
                                                const BArgs&... AArgs) {
   StrictBool b = A1.rows() == A2.rows() && A1.cols() == A2.cols();
   if constexpr(sizeof...(BArgs) > 0) {
      return b && same_size(A2, AArgs...);
   }
   return b;
}


template <BaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR StrictBool same_dimension() {
   auto last_dim = detail::LastPack_t<BArgs...>::dimension();
   return (... && (last_dim == BArgs::dimension()));
}


template <BaseType... BArgs>
STRICT_NODISCARD_CONSTEXPR bool same_dimension_b() {
   return bool{same_dimension<BArgs...>()};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base1, BaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool operator==(const Base1& A1, const Base2& A2) {
   if(!same_size(A1, A2)) {
      return false_sb;
   }
   for(index_t i = 0_sl; i < A1.size(); ++i) {
      if(A1.un(i) != A2.un(i)) {
         return false_sb;
      }
   }
   return true_sb;
}


template <BaseType Base1, BaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
STRICT_CONSTEXPR StrictBool operator!=(const Base1& A1, const Base2& A2) {
   return !(A1 == A2);
}


}  // namespace spp

