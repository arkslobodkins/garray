// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/strict_traits.hpp"


namespace spp::detail {


// Tests whether all elements except the last one are StrictType.
// Last element may or may not be StrictType.
template <typename T, typename... Args>
consteval bool all_strict_except_last() {
   if constexpr(sizeof...(Args) == 0) {
      return true;
   } else {
      return StrictType<T> && all_strict_except_last<Args...>();
   }
}


template <typename... Args> concept AllStrictExceptLast = all_strict_except_last<Args...>();


}  // namespace spp::detail

