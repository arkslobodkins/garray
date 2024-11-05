// Arkadijs Slobodkins, 2023


#pragma once


#include <concepts>
#include <type_traits>


namespace spp {


template <typename D, typename B> concept DerivedFrom = std::derived_from<D, B>;
template <typename B, typename D> concept BaseOf = std::is_base_of_v<B, D>;
template <typename T, typename U> concept SameAs = std::same_as<T, U>;


template <typename T>
using RemoveRef = std::remove_reference_t<T>;


// Not all compilers support remove_cvref_t.
template <typename T>
using RemoveCVRef = std::remove_cv_t<std::remove_reference_t<T>>;


template <typename T>
using AddConst = std::add_const_t<T>;


namespace detail {
template <typename T> concept IsConst = std::is_const_v<T>;
}  // namespace detail


}  // namespace spp

