// Arkadijs Slobodkins, 2023


#pragma once


#include <limits>

#include "error.hpp"
#include "strict_val.hpp"


namespace spp::inline literals {


consteval Strict<int> operator""_si(unsigned long long x) {
   ASSERT_STRICT_DEBUG(x <= std::numeric_limits<int>::max());
   return Strict{static_cast<int>(x)};
}


consteval Strict<long int> operator""_sl(unsigned long long x) {
   ASSERT_STRICT_DEBUG(x <= std::numeric_limits<long int>::max());
   return Strict{static_cast<long int>(x)};
}


consteval Strict<unsigned int> operator""_sui(unsigned long long x) {
   return Strict{static_cast<unsigned int>(x)};
}


consteval Strict<unsigned long int> operator""_sul(unsigned long long x) {
   return Strict{static_cast<unsigned long int>(x)};
}


consteval Strict<float> operator""_sf(long double x) {
   return Strict{static_cast<float>(x)};
}


consteval Strict<double> operator""_sd(long double x) {
   return Strict{static_cast<double>(x)};
}


consteval Strict<long double> operator""_sl(long double x) {
   return Strict{x};
}


// Implementation from mp++.
#ifdef STRICT_QUAD_PRECISION
template <char... Chars>
inline Strict<float128> operator""_sq() {
   constexpr char str[] = {Chars..., '\0'};

   // Pre-check for binary/octal literals.
   ASSERT_STRICT_ALWAYS_MSG(
       !(sizeof...(Chars) >= 2u && str[0] == '0'
         && (str[1] == 'b' || str[1] == 'B' || (str[1] >= '0' && str[1] <= '7'))),
       "A real128 cannot be constructed from binary or octal literals\n");

   float128 x = strtoflt128(str, nullptr);
   return Strict{x};
}
#endif


consteval Strict<int> operator""_SI(unsigned long long x) {
   return operator""_si(x);
}


consteval Strict<long int> operator""_SL(unsigned long long x) {
   return operator""_sl(x);
}


consteval Strict<unsigned int> operator""_SUI(unsigned long long x) {
   return operator""_sui(x);
}


consteval Strict<unsigned long int> operator""_SUL(unsigned long long x) {
   return operator""_sul(x);
}


consteval Strict<float> operator""_SF(long double x) {
   return operator""_sf(x);
}


consteval Strict<double> operator""_SD(long double x) {
   return operator""_sd(x);
}


consteval Strict<long double> operator""_SL(long double x) {
   return operator""_sl(x);
}


// Implementation from mp++.
#ifdef STRICT_QUAD_PRECISION
template <char... Chars>
inline Strict<float128> operator""_SQ() {
   return operator""_sq < Chars... > ();
}
#endif


}  // namespace spp::inline literals

