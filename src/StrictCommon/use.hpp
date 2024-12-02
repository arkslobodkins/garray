// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "strict_traits.hpp"
#include "strict_val.hpp"


namespace spp::use {


template <Builtin T>
using StrictPair = std::pair<Strict<T>, Strict<T>>;


}

