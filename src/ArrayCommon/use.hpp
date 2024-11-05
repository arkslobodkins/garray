// Arkadijs Slobodkins, 2023


#pragma once


#include <initializer_list>

#include "../StrictCommon/auxiliary_types.hpp"
#include "../StrictCommon/strict_traits.hpp"
#include "../StrictCommon/strict_val.hpp"


namespace spp::use {


template <Builtin T>
using List1D = std::initializer_list<Strict<T>>;


template <Builtin T>
using List2D = std::initializer_list<std::initializer_list<Strict<T>>>;


using IndexList = std::initializer_list<ImplicitInt>;


}  // namespace spp::use

