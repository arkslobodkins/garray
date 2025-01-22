// Arkadijs Slobodkins, 2023


#pragma once


#include "../ArrayCommon/array_traits.hpp"
#include "../Expr/expr.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "random_traits.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands(Strict<T> low, Strict<T> high, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr Strict<T> semi_rands(Low<T> low, High<T> high, Seed seed = Seed{One<unsigned>});


template <Builtin T>
constexpr Strict<T> semi_rands(Seed seed = Seed{One<unsigned>});


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands_not0(Strict<T> low, Strict<T> high, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr Strict<T> semi_rands_not0(Low<T> low, High<T> high, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr Strict<T> semi_rands_not0(Seed seed = Seed{One<unsigned>});


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                           Seed seed = Seed{One<unsigned>});


template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random(Base&& A, Low<BuiltinTypeOf<Base>> low, High<BuiltinTypeOf<Base>> high,
                           Seed seed = Seed{One<unsigned>});


template <typename... Base>
   requires detail::RandomBases<Base...>
constexpr void semi_random(Base&&... A);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                                Seed seed = Seed{One<unsigned>});


template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, Low<BuiltinTypeOf<Base>> low,
                                High<BuiltinTypeOf<Base>> high, Seed seed = Seed{One<unsigned>});


template <typename... Base>
   requires detail::RandomRealBases<Base...>
constexpr void semi_random_not0(Base&&... A);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T, typename U>
constexpr auto semi_random(ImplicitInt n, Strict<T> low, Strict<T> high,
                           Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random(Size n, Low<T> low, High<T> high, Seed seed = Seed{One<unsigned>});


template <Builtin T>
constexpr auto semi_random(ImplicitInt n, Seed seed = Seed{One<unsigned>});


template <Builtin T>
constexpr auto semi_random(Size n, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high,
                           Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random(Rows m, Cols n, Low<T> low, High<T> high,
                           Seed seed = Seed{One<unsigned>});


template <Builtin T>
constexpr auto semi_random(ImplicitInt m, ImplicitInt n, Seed seed = Seed{One<unsigned>});


template <Builtin T>
constexpr auto semi_random(Rows m, Cols n, Seed seed = Seed{One<unsigned>});


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Strict<T> low, Strict<T> high,
                                Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(Size n, Low<T> low, High<T> high, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(Size n, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high,
                                Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(Rows m, Cols n, Low<T> low, High<T> high,
                                Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(ImplicitInt m, ImplicitInt n, Seed seed = Seed{One<unsigned>});


template <Real T>
constexpr auto semi_random_not0(Rows m, Cols n, Seed seed = Seed{One<unsigned>});


namespace detail {


template <Builtin T>
class SemiGenerator {
public:
   constexpr SemiGenerator(Strict<unsigned> seed = Strict<unsigned>{1U}, Strict<T> low = Zero<T>,
                           Strict<T> high = One<T>)
       : previous_{Strict{static_cast<unsigned>(__TIME__[7])} * Strict{100'000U}
                   + seed * Strict{100'000U}},
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
   return detail::generate_random(const1D<T>(n, Zero<T>), G, low, high);
}


template <Builtin T>
constexpr auto semi_random(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high,
                           Strict<unsigned> seed) {
   ASSERT_STRICT_DEBUG(low <= high);
   SemiGenerator<T> g{seed + n.get().sui(), low, high};
   auto G = [g]([[maybe_unused]] auto x) { return g.random(); };
   return detail::generate_random(const2D<T>(m, n, Zero<T>), G, low, high);
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
   return detail::generate_random(const1D<T>(n, Zero<T>), G, low, high);
}


template <Real T>
constexpr auto semi_random_not0(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high,
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
   return detail::generate_random(const2D<T>(m, n, Zero<T>), G, low, high);
}


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands(Strict<T> low, Strict<T> high, Seed seed) {
   return detail::semi_rands(low, high, seed.get());
}


template <Real T>
constexpr Strict<T> semi_rands(Low<T> low, High<T> high, Seed seed) {
   return detail::semi_rands(low.get(), high.get(), seed.get());
}


template <Builtin T>
constexpr Strict<T> semi_rands(Seed seed) {
   return detail::semi_rands(Zero<T>, One<T>, seed.get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr Strict<T> semi_rands_not0(Strict<T> low, Strict<T> high, Seed seed) {
   return detail::semi_rands_not0(low, high, seed.get());
}


template <Real T>
constexpr Strict<T> semi_rands_not0(Low<T> low, High<T> high, Seed seed) {
   return detail::semi_rands_not0(low.get(), high.get(), seed.get());
}


template <Real T>
constexpr Strict<T> semi_rands_not0(Seed seed) {
   return detail::semi_rands_not0(Zero<T>, One<T>, seed.get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high, Seed seed) {
   detail::semi_random(A, low, high, seed.get());
}


template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random(Base&& A, Low<BuiltinTypeOf<Base>> low, High<BuiltinTypeOf<Base>> high,
                           Seed seed) {
   detail::semi_random(A, low.get(), high.get(), seed.get());
}


template <typename... Base>
   requires detail::RandomBases<Base...>
constexpr void semi_random(Base&&... A) {
   static_assert(sizeof...(Base) > 0);
   (...,
    detail::semi_random(A, Zero<BuiltinTypeOf<Base>>, One<BuiltinTypeOf<Base>>, One<unsigned>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high,
                                Seed seed) {
   detail::semi_random_not0(A, low, high, seed.get());
}


template <typename Base>
   requires detail::NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
constexpr void semi_random_not0(Base&& A, Low<BuiltinTypeOf<Base>> low,
                                High<BuiltinTypeOf<Base>> high, Seed seed) {
   detail::semi_random_not0(A, low.get(), high.get(), seed.get());
}


template <typename... Base>
   requires detail::RandomRealBases<Base...>
constexpr void semi_random_not0(Base&&... A) {
   static_assert(sizeof...(Base) > 0);
   (...,
    detail::semi_random_not0(
        A, Zero<BuiltinTypeOf<Base>>, One<BuiltinTypeOf<Base>>, One<unsigned>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T, typename U>
constexpr auto semi_random(ImplicitInt n, Strict<T> low, Strict<T> high, Seed seed) {
   return detail::semi_random<T>(n, low, high, seed.get());
}


template <Real T>
constexpr auto semi_random(Size n, Low<T> low, High<T> high, Seed seed) {
   return semi_random<T>(n.get(), low.get(), high.get(), seed.get());
}


template <Builtin T>
constexpr auto semi_random(ImplicitInt n, Seed seed) {
   return detail::semi_random<T>(n, Zero<T>, One<T>, seed.get());
}


template <Builtin T>
constexpr auto semi_random(Size n, Seed seed) {
   return semi_random<T>(n.get(), seed.get());
}


template <Real T>
constexpr auto semi_random(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high, Seed seed) {
   return detail::semi_random<T>(m, n, low, high, seed.get());
}


template <Real T>
constexpr auto semi_random(Rows m, Cols n, Low<T> low, High<T> high, Seed seed) {
   return semi_random<T>(m.get(), n.get(), low.get(), high.get(), seed.get());
}


template <Builtin T>
constexpr auto semi_random(ImplicitInt m, ImplicitInt n, Seed seed) {
   return detail::semi_random<T>(m, n, Zero<T>, One<T>, seed.get());
}


template <Builtin T>
constexpr auto semi_random(Rows m, Cols n, Seed seed) {
   return semi_random<T>(m.get(), n.get(), seed.get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Strict<T> low, Strict<T> high, Seed seed) {
   return detail::semi_random_not0<T>(n, low, high, seed.get());
}


template <Real T>
constexpr auto semi_random_not0(Size n, Low<T> low, High<T> high, Seed seed) {
   return semi_random_not0<T>(n.get(), low.get(), high.get(), seed.get());
}


template <Real T>
constexpr auto semi_random_not0(ImplicitInt n, Seed seed) {
   return detail::semi_random_not0<T>(n, Zero<T>, One<T>, seed.get());
}


template <Real T>
constexpr auto semi_random_not0(Size n, Seed seed) {
   return semi_random_not0<T>(n.get(), seed.get());
}


template <Real T>
constexpr auto semi_random_not0(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high,
                                Seed seed) {
   return detail::semi_random_not0<T>(m, n, low, high, seed.get());
}


template <Real T>
constexpr auto semi_random_not0(Rows m, Cols n, Low<T> low, High<T> high, Seed seed) {
   return semi_random_not0<T>(m.get(), n.get(), low.get(), high.get(), seed.get());
}


template <Real T>
constexpr auto semi_random_not0(ImplicitInt m, ImplicitInt n, Seed seed) {
   return detail::semi_random_not0<T>(m, n, Zero<T>, One<T>, seed.get());
}


template <Real T>
constexpr auto semi_random_not0(Rows m, Cols n, Seed seed) {
   return semi_random_not0<T>(m.get(), n.get(), seed.get());
}


}  // namespace spp

