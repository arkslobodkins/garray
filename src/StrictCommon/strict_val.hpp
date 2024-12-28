// Arkadijs Slobodkins, 2023


#pragma once


#include <cstdlib>

#include "config.hpp"
#include "error.hpp"
#include "strict_traits.hpp"


namespace spp {


// Forward declarations.
template <Builtin T>
struct Strict;


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(U x);


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(Strict<U> x);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <>
struct STRICT_NODISCARD alignas(bool) Strict<bool> {
public:
   using value_type = bool;

   // Special member functions.
   STRICT_NODISCARD_CONSTEXPR explicit Strict() = default;
   STRICT_NODISCARD_CONSTEXPR Strict(const Strict&) = default;
   STRICT_CONSTEXPR Strict& operator=(const Strict&) & = default;

   // Other constructors.
   STRICT_NODISCARD_CONSTEXPR_INLINE explicit Strict(bool x) : val_{x} {
   }

   STRICT_CONSTEXPR Strict(auto x) = delete;

   // Conversions.
   STRICT_NODISCARD_CONSTEXPR_INLINE bool val() const {
      return val_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE operator bool() const {
      return val_;
   }

   template <typename T>
   STRICT_NODISCARD_CONSTEXPR_INLINE operator T() const = delete;

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator!() const {
      return Strict{!val_};
   }

private:
   bool val_{};
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
struct STRICT_NODISCARD alignas(T) Strict {
public:
   using value_type = T;

   // Special member functions.
   STRICT_NODISCARD_CONSTEXPR Strict() = default;
   STRICT_NODISCARD_CONSTEXPR Strict(const Strict&) = default;
   STRICT_CONSTEXPR Strict& operator=(const Strict&) & = default;

   // Other constructors.
   STRICT_NODISCARD_CONSTEXPR_INLINE explicit Strict(T x) : val_{x} {
   }

   STRICT_CONSTEXPR Strict(auto x) = delete;

   // Conversions.
   STRICT_NODISCARD_CONSTEXPR_INLINE T val() const {
      return val_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE explicit operator T() const {
      return val_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator+() const {
      return *this;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator-() const {
      static_assert(!UnsignedInteger<T>);
      // -val_ should not change the type of val_ but T is used to be safe.
      return Strict{T{-val_}};
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator~() const {
      static_assert(Integer<T>);
      return Strict<T>{~val_};
   }

   STRICT_CONSTEXPR_INLINE Strict& operator++() & {
      ++val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator--() & {
      --val_;
      return *this;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator++(int) & {
      Strict old{val_};
      ++val_;
      return old;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict operator--(int) & {
      Strict old{val_};
      --val_;
      return old;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator+=(Strict x) & {
      val_ += x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator-=(Strict x) & {
      val_ -= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator*=(Strict x) & {
      val_ *= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator/=(Strict x) & {
      if constexpr(Integer<T>) {
         ASSERT_STRICT_DIVISION_DEBUG(x.val_ != 0);
      }

      val_ /= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator%=(Strict x) & {
      static_assert(Integer<T>);
      ASSERT_STRICT_REMAINDER_DEBUG(x.val_ != 0);
      val_ %= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator<<=(Strict x) & {
      static_assert(Integer<T>);
      if constexpr(SignedInteger<T>) {
         ASSERT_STRICT_SHIFT_FIRST_DEBUG(val_ > -1);
         ASSERT_STRICT_SHIFT_SECOND_DEBUG(x.val_ > -1);
      }
      val_ <<= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator>>=(Strict x) & {
      static_assert(Integer<T>);
      if constexpr(SignedInteger<T>) {
         ASSERT_STRICT_SHIFT_FIRST_DEBUG(val_ > -1);
         ASSERT_STRICT_SHIFT_SECOND_DEBUG(x.val_ > -1);
      }
      val_ >>= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator&=(Strict x) & {
      static_assert(Integer<T>);
      val_ &= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator|=(Strict x) & {
      static_assert(Integer<T>);
      val_ |= x.val_;
      return *this;
   }

   STRICT_CONSTEXPR_INLINE Strict& operator^=(Strict x) & {
      static_assert(Integer<T>);
      val_ ^= x.val_;
      return *this;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<bool> sb() const {
      return strict_cast<bool>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<int> si() const {
      return strict_cast<int>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long int> sl() const {
      return strict_cast<long int>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<unsigned int> sui() const {
      return strict_cast<unsigned int>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<unsigned long int> sul() const {
      return strict_cast<unsigned long int>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<float> sf() const {
      return strict_cast<float>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<double> sd() const {
      return strict_cast<double>(val_);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<long double> sld() const {
      return strict_cast<long double>(val_);
   }

#ifdef STRICT_QUAD_PRECISION
   STRICT_NODISCARD_CONSTEXPR_INLINE Strict<float128> sq() const {
      return strict_cast<float128>(val_);
   }
#endif

private:
   T val_{};
};


namespace detail {
template <typename T> concept CompatibleBuiltin = Builtin<T> && sizeof(T) == sizeof(Strict<T>);
}


template <Builtin T>
STRICT_CONSTEXPR_INLINE auto Zero = Strict<T>{T(0)};


template <Builtin T>
STRICT_CONSTEXPR_INLINE auto One = Strict<T>{T(1)};


// Boolean and unsigned integers are not allowed.
template <Real T>
   requires(!UnsignedInteger<T>)
STRICT_CONSTEXPR_INLINE auto NegOne = Strict<T>{T(-1)};


template <Real T>
STRICT_CONSTEXPR_INLINE auto Thousand = Strict<T>{T(1'000)};


template <Real T>
STRICT_CONSTEXPR_INLINE auto Million = Strict<T>{T(1'000'000)};


template <Real T>
STRICT_CONSTEXPR_INLINE auto Billion = Strict<T>{T(1'000'000'000)};


STRICT_CONSTEXPR_INLINE auto true_sb = StrictBool{true};
STRICT_CONSTEXPR_INLINE auto false_sb = StrictBool{false};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE std::size_t to_size_t(T x) {
   return static_cast<std::size_t>(x);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE std::size_t to_size_t(Strict<T> x) {
   return static_cast<std::size_t>(x.val());
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t to_index_t(T x) {
   return strict_cast<index_t::value_type>(x);
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE index_t to_index_t(Strict<T> x) {
   return strict_cast<index_t::value_type>(x);
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE T builtin_cast(U x) {
   return static_cast<T>(x);
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE T builtin_cast(Strict<U> x) {
   return static_cast<T>(U{x});
}


template <Real T, Real U>
STRICT_NODISCARD_CONSTEXPR_INLINE T real_cast(U x) {
   return static_cast<T>(x);
}


template <Real T, Real U>
STRICT_NODISCARD_CONSTEXPR_INLINE T real_cast(Strict<U> x) {
   return static_cast<T>(U{x});
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(U x) {
   return Strict{builtin_cast<T>(x)};
}


template <Builtin T, Builtin U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> strict_cast(Strict<U> x) {
   return Strict{builtin_cast<T>(x)};
}


template <Floating T, Integer U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> whole(U x) {
   return strict_cast<T, U>(x);
}


template <Floating T, Integer U>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> whole(Strict<U> x) {
   return strict_cast<T, U>(x);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator+(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} + T{y}};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator-(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} - T{y}};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator*(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} * T{y}};
}


template <Real T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator/(Strict<T> x, Strict<T> y) {
   if constexpr(Integer<T>) {
      ASSERT_STRICT_DIVISION_DEBUG(T{y} != 0);
   }
   return Strict<T>{T{x} / T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator%(Strict<T> x, Strict<T> y) {
   ASSERT_STRICT_REMAINDER_DEBUG(T{y} != 0);
   return Strict<T>{T{x} % T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator<<(Strict<T> x, Strict<T> y) {
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_SHIFT_FIRST_DEBUG(T{x} > -1);
      ASSERT_STRICT_SHIFT_SECOND_DEBUG(T{y} > -1);
   }
   return Strict<T>{T{x} << T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator>>(Strict<T> x, Strict<T> y) {
   if constexpr(SignedInteger<T>) {
      ASSERT_STRICT_SHIFT_FIRST_DEBUG(T{x} > -1);
      ASSERT_STRICT_SHIFT_SECOND_DEBUG(T{y} > -1);
   }
   return Strict<T>{T{x} >> T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator&(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} & T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator|(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} | T{y}};
}


template <Integer T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator^(Strict<T> x, Strict<T> y) {
   return Strict<T>{T{x} ^ T{y}};
}


template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE Strict<T> operator^(Strict<T> x, Strict<T> y) {
   return strict_cast<bool>(bool{x} ^ bool{y});
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator&&(Strict<T> x, Strict<T> y) {
   return StrictBool{bool{x} && bool{y}};
}


template <Boolean T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator||(Strict<T> x, Strict<T> y) {
   return StrictBool{bool{x} || bool{y}};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator==(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} == T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator!=(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} != T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator<(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} < T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator<=(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} <= T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator>(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} > T{y}};
}


template <Builtin T>
STRICT_NODISCARD_CONSTEXPR_INLINE StrictBool operator>=(Strict<T> x, Strict<T> y) {
   return StrictBool{T{x} >= T{y}};
}


}  // namespace spp
