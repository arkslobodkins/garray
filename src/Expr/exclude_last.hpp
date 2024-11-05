// Arkadijs Slobodkins, 2023


#pragma once


#include <tuple>
#include <type_traits>
#include <utility>

#include "../StrictCommon/strict_traits.hpp"


namespace spp::detail {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Tuple, int... I>
constexpr auto firstN_tuple_value(Tuple&& tuple, std::integer_sequence<int, I...>) {
   return std::forward_as_tuple(std::get<I>(std::forward<Tuple>(tuple))...);
}


template <typename... Args>
constexpr auto exclude_last_tuple_value(Args&&... args) {
   return firstN_tuple_value(std::forward_as_tuple(std::forward<Args>(args)...),
                             std::make_integer_sequence<int, sizeof...(Args) - 1>{});
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename... Args>
struct ExcludeLastTuple {
   using type = std::invoke_result_t<decltype(exclude_last_tuple_value<Args...>), Args...>;
};


template <typename... Args>
using ExcludeLastTuple_t = typename ExcludeLastTuple<Args...>::type;


template <typename... Args>
consteval bool all_tuple_strict(std::tuple<Args...>) {
   return AllStrict<Args...>;
}


template <typename... Args> concept AllStrictExceptLast
    = all_tuple_strict(ExcludeLastTuple_t<Args...>{});


}  // namespace spp::detail

