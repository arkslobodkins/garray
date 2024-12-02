// Arkadijs Slobodkins, 2023


#pragma once


#include "../StrictCommon/strict_traits.hpp"


namespace spp::detail {


template <typename T, typename... Args>
consteval bool all_strict_except_last() {
   if constexpr(sizeof...(Args) == 0) {
      return true;
   } else if constexpr(sizeof...(Args) == 1) {
      return StrictType<T>;
   } else {
      return StrictType<T> && all_strict_except_last<Args...>();
   }
}


template <typename... Args> concept AllStrictExceptLast = all_strict_except_last<Args...>();


}  // namespace spp::detail

