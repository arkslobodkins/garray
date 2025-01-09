// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_traits.hpp"


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


}  // namespace spp::detail

