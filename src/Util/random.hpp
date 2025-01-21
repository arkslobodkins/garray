// Arkadijs Slobodkins, 2023


#pragma once


#include <random>

#include "../ArrayCommon/array_traits.hpp"
#include "../Expr/expr.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "random_traits.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands(Strict<T> low, Strict<T> high);


template <Real T>
Strict<T> rands(Low<T> low, High<T> high);


template <Builtin T>
Strict<T> rands();


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands_not0(Strict<T> low, Strict<T> high);


template <Real T>
Strict<T> rands_not0(Low<T> low, High<T> high);


template <Real T>
Strict<T> rands_not0();


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base, typename... BasesAndStrict>
   requires detail::RandomRealBasesAndStrict<Base, BasesAndStrict...>
void random(Base&& A, BasesAndStrict&&... AArgs_and_xargs);


template <typename Base, typename... BasesAndLowHigh>
   requires detail::RandomRealBasesAndLowHigh<Base, BasesAndLowHigh...>
void random(Base&& A, BasesAndLowHigh&&... AArgs_and_xargs);


template <typename... Base>
   requires detail::RandomBases<Base...>
void random(Base&&... A);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base, typename... BasesAndStrict>
   requires detail::RandomRealBasesAndStrict<Base, BasesAndStrict...>
void random_not0(Base&& A, BasesAndStrict&&... AArgs_and_xargs);


template <typename Base, typename... BasesAndLowHigh>
   requires detail::RandomRealBasesAndLowHigh<Base, BasesAndLowHigh...>
void random_not0(Base&& A, BasesAndLowHigh&&... AArgs_and_xargs);


template <typename... Base>
   requires detail::RandomRealBases<Base...>
void random_not0(Base&&... A);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
auto random(ImplicitInt n, Strict<T> low, Strict<T> high);


template <Real T>
auto random(Size n, Low<T> low, High<T> high);


template <Builtin T>
auto random(ImplicitInt n);


template <Builtin T>
auto random(Size n);


template <Real T>
auto random(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high);


template <Real T>
auto random(Rows m, Cols n, Low<T> low, High<T> high);


template <Builtin T>
auto random(ImplicitInt m, ImplicitInt n);


template <Builtin T>
auto random(Rows m, Cols n);


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
auto random_not0(ImplicitInt n, Strict<T> low, Strict<T> high);


template <Real T>
auto random_not0(Size n, Low<T> low, High<T> high);


template <Real T>
auto random_not0(ImplicitInt n);


template <Real T>
auto random_not0(Size n);


template <Real T>
auto random_not0(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high);


template <Real T>
auto random_not0(Rows m, Cols n, Low<T> low, High<T> high);


template <Real T>
auto random_not0(ImplicitInt m, ImplicitInt n);


template <Real T>
auto random_not0(Rows m, Cols n);


namespace detail {


template <typename T>
struct Generator;


template <Integer T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.val(), h.val()} {
   }

   auto random() const {
      return Strict{dst(gen)};
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_int_distribution<T> dst;
};


template <Boolean T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.val(), h.val()} {
   }

   auto random() const {
      return dst(gen) % 2 == 0 ? false_sb : true_sb;
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_int_distribution<int> dst;
};


template <StandardFloating T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h) : gen{std::random_device{}()}, dst{l.val(), h.val()} {
   }

   auto random() const {
      return Strict{dst(gen)};
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_real_distribution<T> dst;
};


// Generates quadruple precision numbers that are double precision numbers.
#ifdef STRICT_QUAD_PRECISION
template <Quadruple T>
struct Generator<T> {
   Generator(Strict<T> l, Strict<T> h)
       : gen{std::random_device{}()},
         dst{l.sd().val(), h.sd().val()} {
   }

   auto random() const {
      return Strict{dst(gen)}.sq();
   }

private:
   mutable std::mt19937 gen;
   mutable std::uniform_real_distribution<double> dst;
};
#endif


template <Builtin T>
Strict<T> rands(Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<T> g{low, high};
   return g.random();
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>>
void random(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<BuiltinTypeOf<Base>> g{low, high};
   for(auto& x : A) {
      x = g.random();
   }
}


template <Builtin T>
auto random(ImplicitInt n, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<T> g{low, high};
   auto G = [g]([[maybe_unused]] auto x) { return g.random(); };
   return detail::generate_random(const1D<T>(n, Zero<T>), G, low, high);
}


template <Builtin T>
auto random(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   Generator<T> g{low, high};
   auto G = [g]([[maybe_unused]] auto x) { return g.random(); };
   return detail::generate_random(const2D<T>(m, n, Zero<T>), G, low, high);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands_not0(Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   Generator<T> g{low, high};
   while(true) {
      if(auto r = g.random(); r != Zero<T>) {
         return r;
      }
   }
}


template <typename Base>
   requires NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
void random_not0(Base&& A, ValueTypeOf<Base> low, ValueTypeOf<Base> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<RealTypeOf<Base>> && high == Zero<RealTypeOf<Base>>));

   Generator<RealTypeOf<Base>> g{low, high};
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
auto random_not0(ImplicitInt n, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   Generator<T> g{low, high};
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
auto random_not0(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high) {
   ASSERT_STRICT_DEBUG(low <= high);
   ASSERT_STRICT_DEBUG(!(low == Zero<T> && high == Zero<T>));

   Generator<T> g{low, high};
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
Strict<T> rands(Strict<T> low, Strict<T> high) {
   return detail::rands(low, high);
}


template <Real T>
Strict<T> rands(Low<T> low, High<T> high) {
   return detail::rands(low.get(), high.get());
}


template <Builtin T>
Strict<T> rands() {
   return detail::rands(Zero<T>, One<T>);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
Strict<T> rands_not0(Strict<T> low, Strict<T> high) {
   return detail::rands_not0(low, high);
}


template <Real T>
Strict<T> rands_not0(Low<T> low, High<T> high) {
   return detail::rands_not0(low.get(), high.get());
}


template <Real T>
Strict<T> rands_not0() {
   return detail::rands_not0(Zero<T>, One<T>);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base, typename... BasesAndStrict>
   requires detail::RandomRealBasesAndStrict<Base, BasesAndStrict...>
void random(Base&& A, BasesAndStrict&&... AArgs_and_xargs) {
   detail::random(A,
                  detail::second_last_value_of(AArgs_and_xargs...),
                  detail::last_value_of(AArgs_and_xargs...));
   if constexpr(sizeof...(BasesAndStrict) >= 3) {
      random(AArgs_and_xargs...);
   }
}


template <typename Base, typename... BasesAndLowHigh>
   requires detail::RandomRealBasesAndLowHigh<Base, BasesAndLowHigh...>
void random(Base&& A, BasesAndLowHigh&&... AArgs_and_xargs) {
   detail::random(A,
                  detail::second_last_value_of(AArgs_and_xargs...).get(),
                  detail::last_value_of(AArgs_and_xargs...).get());
   if constexpr(sizeof...(BasesAndLowHigh) >= 3) {
      random(AArgs_and_xargs...);
   }
}


template <typename... Base>
   requires detail::RandomBases<Base...>
void random(Base&&... A) {
   static_assert(sizeof...(Base) > 0);
   (..., detail::random(A, Zero<BuiltinTypeOf<Base>>, One<BuiltinTypeOf<Base>>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename Base, typename... BasesAndStrict>
   requires detail::RandomRealBasesAndStrict<Base, BasesAndStrict...>
void random_not0(Base&& A, BasesAndStrict&&... AArgs_and_xargs) {
   detail::random_not0(A,
                       detail::second_last_value_of(AArgs_and_xargs...),
                       detail::last_value_of(AArgs_and_xargs...));
   if constexpr(sizeof...(BasesAndStrict) >= 3) {
      random_not0(AArgs_and_xargs...);
   }
}


template <typename Base, typename... BasesAndLowHigh>
   requires detail::RandomRealBasesAndLowHigh<Base, BasesAndLowHigh...>
void random_not0(Base&& A, BasesAndLowHigh&&... AArgs_and_xargs) {
   detail::random_not0(A,
                       detail::second_last_value_of(AArgs_and_xargs...).get(),
                       detail::last_value_of(AArgs_and_xargs...).get());
   if constexpr(sizeof...(BasesAndLowHigh) >= 3) {
      random_not0(AArgs_and_xargs...);
   }
}


template <typename... Base>
   requires detail::RandomRealBases<Base...>
void random_not0(Base&&... A) {
   static_assert(sizeof...(Base) > 0);
   (..., detail::random_not0(A, Zero<BuiltinTypeOf<Base>>, One<BuiltinTypeOf<Base>>));
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
auto random(ImplicitInt n, Strict<T> low, Strict<T> high) {
   return detail::random<T>(n, low, high);
}


template <Real T>
auto random(Size n, Low<T> low, High<T> high) {
   return random<T>(n.get(), low.get(), high.get());
}


template <Builtin T>
auto random(ImplicitInt n) {
   return detail::random<T>(n, Zero<T>, One<T>);
}


template <Builtin T>
auto random(Size n) {
   return random<T>(n.get());
}


template <Real T>
auto random(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high) {
   return detail::random<T>(m, n, low, high);
}


template <Real T>
auto random(Rows m, Cols n, Low<T> low, High<T> high) {
   return random<T>(m.get(), n.get(), low.get(), high.get());
}


template <Builtin T>
auto random(ImplicitInt m, ImplicitInt n) {
   return detail::random<T>(m, n, Zero<T>, One<T>);
}


template <Builtin T>
auto random(Rows m, Cols n) {
   return random<T>(m.get(), n.get());
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
auto random_not0(ImplicitInt n, Strict<T> low, Strict<T> high) {
   return detail::random_not0<T>(n, low, high);
}


template <Real T>
auto random_not0(Size n, Low<T> low, High<T> high) {
   return random_not0<T>(n.get(), low.get(), high.get());
}


template <Real T>
auto random_not0(ImplicitInt n) {
   return detail::random_not0<T>(n, Zero<T>, One<T>);
}


template <Real T>
auto random_not0(Size n) {
   return random_not0<T>(n.get());
}


template <Real T>
auto random_not0(ImplicitInt m, ImplicitInt n, Strict<T> low, Strict<T> high) {
   return detail::random_not0<T>(m, n, low, high);
}


template <Real T>
auto random_not0(Rows m, Cols n, Low<T> low, High<T> high) {
   return random_not0<T>(m.get(), n.get(), low.get(), high.get());
}


template <Real T>
auto random_not0(ImplicitInt m, ImplicitInt n) {
   return detail::random_not0<T>(m, n, Zero<T>, One<T>);
}


template <Real T>
auto random_not0(Rows m, Cols n) {
   return random_not0<T>(m.get(), n.get());
}


}  // namespace spp
