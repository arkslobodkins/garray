// Arkadijs Slobodkins, 2023


#pragma once


#include "common_traits.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace spp {


#define STRICT_COMPATIBILITY_ERROR_MSG                                        \
   "THE C++ IMPLEMENTATION YOU ARE USING DOES NOT MEET THE SIZE REQUIREMENTS" \
   " OF STRICT-LIB. CONSIDER USING ANOTHER MACHINE/IMPLEMENTATION TO MEET THESE REQUIREMENTS."


static_assert(sizeof(float) == 4, STRICT_COMPATIBILITY_ERROR_MSG);
static_assert(sizeof(double) == 8, STRICT_COMPATIBILITY_ERROR_MSG);
using float32 = float;
using float64 = double;


static_assert(sizeof(int) >= 4, STRICT_COMPATIBILITY_ERROR_MSG);
static_assert(sizeof(long int) >= 8, STRICT_COMPATIBILITY_ERROR_MSG);  // Ensure 64-bit indexing.


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> concept SignedInteger = SameAs<T, int> || SameAs<T, long int>;

template <typename T> concept UnsignedInteger
    = SameAs<T, unsigned int> || SameAs<T, unsigned long int>;

template <typename T> concept Integer = SignedInteger<T> || UnsignedInteger<T>;

template <typename T> concept StandardFloating
    = SameAs<T, float> || SameAs<T, double> || SameAs<T, long double>;


#ifdef STRICT_QUAD_PRECISION
static_assert(sizeof(__float128) == 16, STRICT_COMPATIBILITY_ERROR_MSG);
using float128 = __float128;
template <typename T> concept Quadruple = SameAs<T, float128>;
template <typename T> concept Floating = StandardFloating<T> || Quadruple<T>;
#else
template <typename T> concept Floating = StandardFloating<T>;
#endif


template <typename T> concept Real = Floating<T> || Integer<T>;
template <typename T> concept Boolean = SameAs<T, bool>;
template <typename T> concept Builtin = Boolean<T> || Real<T>;


#ifdef STRICT_QUAD_PRECISION
template <typename T> concept NotQuadruple = Builtin<T> && !Quadruple<T>;
#else
template <typename T> concept NotQuadruple = Builtin<T>;
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
struct Strict;


// Aliases for unsigned integers are not introduced.
using index_t = Strict<long int>;
using StrictBool = Strict<bool>;
using StrictInt = Strict<int>;
using StrictLong = Strict<long int>;
using Strict32 = Strict<float32>;
using Strict64 = Strict<float64>;
#ifdef STRICT_QUAD_PRECISION
using Strict128 = Strict<float128>;
#endif


// This concept guarantees T to be Strict value since one can only
// construct Strict using a built-in type or copy constructor. But
// value_type is invalid for built-in types.
template <typename T> concept StrictType = requires(T x) {
   typename T::value_type;
   Strict{x};
};


template <typename... Args> concept AllStrict = (... && StrictType<Args>);


}  // namespace spp
