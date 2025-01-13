// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_auxiliary.hpp"
#include "ArrayCommon/array_traits.hpp"
#include "StrictCommon/strict_common.hpp"


namespace spp::detail {


template <typename Base>
class Lval_CRTP {
public:
   STRICT_CONSTEXPR Base& lval() & = delete;

   STRICT_CONSTEXPR const Base& lval() const& = delete;

   STRICT_NODISCARD_CONSTEXPR Base& lval() && {
      return this->lval_impl();
   }

   STRICT_NODISCARD_CONSTEXPR const Base& lval() const&& {
      return this->lval_impl();
   }

private:
   STRICT_CONSTEXPR Base& lval_impl() {
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR const Base& lval_impl() const {
      return static_cast<const Base&>(*this);
   }
};


template <typename T1, typename T2> concept SameDimensionRealBaseType
    = RealBaseType<T1> && RealBaseType<T2> && same_dimension_b<T1, T2>();


template <typename T1, typename T2> concept SameDimensionIntegerBaseType
    = IntegerBaseType<T1> && IntegerBaseType<T2> && same_dimension_b<T1, T2>();


template <typename Base, typename T>
class Operands_CRTP {
public:
   using value_type = T;
   using builtin_type = value_type::value_type;


   STRICT_CONSTEXPR Base& operator+=(value_type x) {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) += x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator-=(value_type x) {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) -= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator*=(value_type x) {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) *= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator/=(value_type x) {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) /= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator%=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) %= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator<<=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) <<= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator>>=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) >>= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator&=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) &= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator|=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) |= x; });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator^=(value_type x)
      requires Integer<builtin_type>
   {
      apply0(static_cast<Base&>(*this),
             [x, this](index_t i) { static_cast<Base&>(*this).un(i) ^= x; });
      return static_cast<Base&>(*this);
   }

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR Base& operator+=(SameDimensionRealBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) += A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator-=(SameDimensionRealBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) -= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator*=(SameDimensionRealBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) *= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator/=(SameDimensionRealBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) /= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator%=(SameDimensionIntegerBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) %= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator<<=(SameDimensionIntegerBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) <<= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator>>=(SameDimensionIntegerBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) >>= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator&=(SameDimensionIntegerBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) &= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator|=(SameDimensionIntegerBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) |= A.un(i);
      });
      return static_cast<Base&>(*this);
   }

   STRICT_CONSTEXPR Base& operator^=(SameDimensionIntegerBaseType<Base> auto const& A) {
      ASSERT_STRICT_DEBUG(same_size(static_cast<Base&>(*this), A));
      apply1(static_cast<Base&>(*this), A, [&](index_t i) {
         static_cast<Base&>(*this).un(i) ^= A.un(i);
      });
      return static_cast<Base&>(*this);
   }
};


}  // namespace spp::detail

