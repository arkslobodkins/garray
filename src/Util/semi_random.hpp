// Arkadijs Slobodkins, 2023


#pragma once


#include "../ArrayCommon/array_traits.hpp"
#include "../Expr/expr.hpp"
#include "../StrictCommon/strict_common.hpp"


namespace spp {


namespace detail {


template <Builtin T>
class SemiGenerator {
public:
   constexpr SemiGenerator(Strict<unsigned> seed = Strict<unsigned>{1U}, Strict<T> low = Zero<T>,
                           Strict<T> high = One<T>)
       : previous_{Strict{unsigned(__TIME__[7])} * Strict{100'000U} + seed * Strict{100'000U}},
         low_{low},
         high_{high} {
   }

   constexpr Strict<T> random() const {
      auto rand_num = this->generate();
      if constexpr(Floating<T>) {
         return low_ + (strict_cast<T>(rand_num) / strict_cast<T>(modulus_)) * (high_ - low_);
      } else if constexpr(Integer<T>) {
         if(low_ == Zero<T> && high_ == One<T>) {
            return rand_num > this->generate() ? Zero<T> : One<T>;
         } else {
            return low_ + strict_cast<T>(rand_num) % (high_ - low_ + One<T>);
         }
      } else {
         return rand_num > this->generate() ? false_sb : true_sb;
      }
   }

private:
   static constexpr Strict<unsigned> modulus_{10'000'000U};
   static constexpr Strict<unsigned> multiplier_{8U};
   static constexpr Strict<unsigned> increment_{13U};
   mutable Strict<unsigned> previous_;
   Strict<T> low_;
   Strict<T> high_;

   constexpr Strict<unsigned> generate() const {
      previous_ = ((multiplier_ * previous_ + increment_) % modulus_);
      return previous_;
   }
};


template <Builtin T>
constexpr Strict<T> semi_rands(Strict<T> low, Strict<T> high, Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   SemiGenerator<T> g{seed, low, high};
   return g.random();
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>>
constexpr void semi_random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                           Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   SemiGenerator<BuiltinTypeOf<Base>> g{seed + A.size().sui(), low, high};
   for(auto& x : A) {
      x = g.random();
   }
}


template <Builtin T>
constexpr auto semi_random(ImplicitInt n, Strict<T> low, Strict<T> high, Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   SemiGenerator<T> g{seed + n.get().sui(), low, high};
   auto G = [g]([[maybe_unused]] auto x) { return g.random(); };
   return generate<decltype(const1D<T>(n, Zero<T>)), decltype(G), true>(const1D<T>(n, Zero<T>), G);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands_not0(Strict<T> low, Strict<T> high, Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   SemiGenerator<T> g{seed, low, high};
   while(true) {
      if(auto r = g.random(); r != Zero<T>) {
         return r;
      }
   }
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                                Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<RealTypeOf<Base>> && high == Zero<RealTypeOf<Base>>));

   SemiGenerator<RealTypeOf<Base>> g{seed + A.size().sui(), low, high};
   for(auto& x : A) {
      while(true) {
         if(auto r = g.random(); r != Zero<RealTypeOf<Base>>) {
            x = r;
            break;
         }
      }
   }
}


template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Strict<T> low, Strict<T> high,
                                Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   SemiGenerator<T> g{seed + n.get().sui(), low, high};
   auto G = [g]([[maybe_unused]] auto x) {
      while(true) {
         if(auto r = g.random(); r != Zero<T>) {
            return r;
         }
      }
   };
   return generate<decltype(const1D<T>(n, Zero<T>)), decltype(G), true>(const1D<T>(n, Zero<T>), G);
}


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands(Strict<T> low, Strict<T> high,
                               Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_rands(low, high, seed);
}


template <Real T>
constexpr Strict<T> semi_rands(Low<T> low, High<T> high, Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_rands(low.get(), high.get(), seed);
}


template <Builtin T>
constexpr Strict<T> semi_rands(Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_rands(Zero<T>, One<T>, seed);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands_not0(Strict<T> low, Strict<T> high,
                                    Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_rands_not0(low, high, seed);
}


template <Real T>
constexpr Strict<T> semi_rands_not0(Low<T> low, High<T> high,
                                    Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_rands_not0(low.get(), high.get(), seed);
}


template <Real T>
constexpr Strict<T> semi_rands_not0(Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_rands_not0(Zero<T>, One<T>, seed);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                           Strict<unsigned> seed = One<unsigned>) {
   detail::semi_random(A, low, high, seed);
}


template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random(Base&& A, Low<BuiltinTypeOf<Base>> low, High<BuiltinTypeOf<Base>> high,
                           Strict<unsigned> seed = One<unsigned>) {
   detail::semi_random(A, low.get(), high.get(), seed);
}


template <typename... Base>
   requires detail::AllNonConstBases<RemoveRef<Base>...>
constexpr void semi_random(Base&&... A) {
   (...,
    detail::semi_random(A, Zero<BuiltinTypeOf<Base>>, One<BuiltinTypeOf<Base>>, One<unsigned>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                                Strict<unsigned> seed = One<unsigned>) {
   detail::semi_random_not0(A, low, high, seed);
}


template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, Low<BuiltinTypeOf<Base>> low,
                                High<BuiltinTypeOf<Base>> high,
                                Strict<unsigned> seed = One<unsigned>) {
   detail::semi_random_not0(A, low.get(), high.get(), seed);
}


template <typename... Base>
   requires detail::AllNonConstBases<RemoveRef<Base>...>
         && Real<BuiltinTypeOf<detail::LastPack_t<Base...>>>
constexpr void semi_random_not0(Base&&... A) {
   (...,
    detail::semi_random_not0(
        A, Zero<BuiltinTypeOf<Base>>, One<BuiltinTypeOf<Base>>, One<unsigned>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr auto semi_random(ImplicitInt n, Strict<T> low, Strict<T> high,
                           Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_random<T>(n, low, high, seed);
}


template <Real T>
constexpr auto semi_random(Size n, Low<T> low, High<T> high,
                           Strict<unsigned> seed = One<unsigned>) {
   return semi_random<T>(n.get(), low.get(), high.get(), seed);
}


template <Builtin T>
constexpr auto semi_random(ImplicitInt n, Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_random<T>(n, Zero<T>, One<T>, seed);
}


template <Builtin T>
constexpr auto semi_random(Size n, Strict<unsigned> seed = One<unsigned>) {
   return semi_random<T>(n.get(), seed);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Strict<T> low, Strict<T> high,
                                Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_random_not0<T>(n, low, high, seed);
}


template <Real T>
constexpr auto semi_random_not0(Size n, Low<T> low, High<T> high,
                                Strict<unsigned> seed = One<unsigned>) {
   return semi_random_not0<T>(n.get(), low.get(), high.get(), seed);
}


template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Strict<unsigned> seed = One<unsigned>) {
   return detail::semi_random_not0<T>(n, Zero<T>, One<T>, seed);
}


template <Real T>
constexpr auto semi_random_not0(Size n, Strict<unsigned> seed = One<unsigned>) {
   return semi_random_not0<T>(n.get(), seed);
}


}  // namespace spp

