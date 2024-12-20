// Arkadijs Slobodkins, 2023


#pragma once


#include <iostream>
#if __has_include(<version>)
#include <version>
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_REMOVE_NODISCARD
#define STRICT_NODISCARD [[nodiscard]]
#else
#define STRICT_NODISCARD
#endif


#define STRICT_NODISCARD_CONSTEXPR_INLINE STRICT_NODISCARD constexpr inline
#define STRICT_NODISCARD_INLINE STRICT_NODISCARD inline
#define STRICT_NODISCARD_CONSTEXPR STRICT_NODISCARD constexpr
#define STRICT_CONSTEXPR_INLINE constexpr inline
#define STRICT_CONSTEXPR constexpr
#define STRICT_INLINE inline


#if __cplusplus >= 202302L
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2023 STRICT_NODISCARD_CONSTEXPR_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2023 STRICT_NODISCARD_CONSTEXPR
#define STRICT_CONSTEXPR_INLINE_2023 STRICT_CONSTEXPR_INLINE
#define STRICT_CONSTEXPR_2023 STRICT_CONSTEXPR

#else
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2023 STRICT_NODISCARD_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2023 STRICT_NODISCARD
#define STRICT_CONSTEXPR_INLINE_2023 STRICT_INLINE
#define STRICT_CONSTEXPR_2023
#endif


#if __cplusplus > 202302L
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2026 STRICT_NODISCARD_CONSTEXPR_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2026 STRICT_NODISCARD_CONSTEXPR
#define STRICT_CONSTEXPR_INLINE_2026 STRICT_CONSTEXPR_INLINE
#define STRICT_CONSTEXPR_2026 STRICT_CONSTEXPR

#else
#define STRICT_NODISCARD_CONSTEXPR_INLINE_2026 STRICT_NODISCARD_INLINE
#define STRICT_NODISCARD_CONSTEXPR_2026 STRICT_NODISCARD
#define STRICT_CONSTEXPR_INLINE_2026 STRICT_INLINE
#define STRICT_CONSTEXPR_2026
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#if defined __GNUG__ || defined __INTEL_COMPILER || defined __INTEL_LLVM_COMPILER \
    || defined __clang__
#define STRICT_RESTRICT __restrict
#else
#define STRICT_RESTRICT
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUAD_PRECISION

#ifdef __GNUG__

#if __has_include(<quadmath.h>)
#include <quadmath.h>
#else
#error QUADRUPLE PRECISION IS NOT PROVIDED ON THIS COMPILER. TO ENABLE IT, COMPILE WITH GCC COMPILER THAT SUPPORTS QUAD PRECISION.
#endif

#else
#error QUADRUPLE PRECISION IS NOT PROVIDED ON THIS COMPILER. TO ENABLE IT, COMPILE WITH GCC COMPILER THAT SUPPORTS QUAD PRECISION.
#endif

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_STACKTRACE

#ifdef __cpp_lib_stacktrace

#include <stacktrace>

namespace spp::detail {
inline void print_stacktrace() {
   auto s = std::stacktrace::current();
   for(std::stacktrace::size_type i = 0; i < s.size(); ++i) {
      std::cout << s[i] << '\n';
   }
   std::cout << "..." << std::endl;
}

}  // namespace spp::detail

#else

#error STACKTRACE IS NOT AVAILABLE. Compile without STRICT_STACKTRACE or compile with C++23 staacktrace support.

namespace spp::detail {
constexpr inline void print_stacktrace() {
}
}  // namespace spp::detail

#endif

// if STRICT_STACKTRACE not defined.
#else

namespace spp::detail {
constexpr inline void print_stacktrace() {
}
}  // namespace spp::detail

#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace spp {


inline void config_info() {
#ifndef STRICT_DEBUG_OFF
   std::cout << "compute mode: DEBUG" << '\n';
#else
   std::cout << "compute mode: RELEASE" << '\n';
#endif

#ifndef STRICT_ERROR_EXCEPTIONS
   std::cout << "error handling: ASSERTIONS" << '\n';
#else
   std::cout << "error handling: EXCEPTIONS" << '\n';
#endif

#ifndef STRICT_QUAD_PRECISION
   std::cout << "quadruple precision: OFF" << '\n';
#else
   std::cout << "quadruple precision: ON" << '\n';
#endif

#ifndef STRICT_STACKTRACE
   std::cout << "C++23 stacktrace: OFF" << '\n';
#else
   std::cout << "C++23 stacktrace: ON" << '\n';
#endif
}


}  // namespace spp

