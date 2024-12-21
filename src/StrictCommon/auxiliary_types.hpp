// Arkadijs Slobodkins, 2023


#pragma once


#include "config.hpp"
#include "error.hpp"
#include "strict_literals.hpp"
#include "strict_traits.hpp"
#include "strict_val.hpp"


#define STRICT_GENERATE_SMALL_TYPES(SmallObjectName, cnd)                             \
   template <Builtin T>                                                               \
   class STRICT_NODISCARD SmallObjectName {                                           \
   public:                                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() = default;           \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(T x) : x_{x} {         \
      }                                                                               \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<T> x) : x_{x} { \
      }                                                                               \
                                                                                      \
      STRICT_CONSTEXPR SmallObjectName(auto x) = delete;                              \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR Strict<T> get() const {                              \
         return x_;                                                                   \
      }                                                                               \
                                                                                      \
   private:                                                                           \
      Strict<T> x_;                                                                   \
   };


#define STRICT_GENERATE_SMALL_INT_TYPES(SmallObjectName, cnd, non_neg)      \
   class STRICT_NODISCARD SmallObjectName {                                 \
   public:                                                                  \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() = default; \
                                                                            \
      template <SignedInteger T, bool b = non_neg>                          \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(T x)         \
          : x_{static_cast<long int>(x)} {                                  \
         if constexpr(b) {                                                  \
            ASSERT_STRICT_DEBUG(x_ > -1_sl);                                \
         }                                                                  \
      }                                                                     \
                                                                            \
      template <SignedInteger T, bool b = non_neg>                          \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<T> x) \
          : x_{strict_cast<long int>(x)} {                                  \
         if constexpr(b) {                                                  \
            ASSERT_STRICT_DEBUG(x_ > -1_sl);                                \
         }                                                                  \
      }                                                                     \
                                                                            \
      STRICT_NODISCARD_CONSTEXPR index_t get() const {                      \
         return x_;                                                         \
      }                                                                     \
                                                                            \
   private:                                                                 \
      index_t x_;                                                           \
   };


#define STRICT_GENERATE_SMALL_UNSIGNED_TYPES(SmallObjectName, cnd, Type)                         \
   class STRICT_NODISCARD SmallObjectName {                                                      \
   public:                                                                                       \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() = default;                      \
                                                                                                 \
      template <typename U>                                                                      \
         requires SameAs<Type, U>                                                                \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(U x) : x_{static_cast<Type>(x)} { \
      }                                                                                          \
                                                                                                 \
      template <typename U>                                                                      \
         requires SameAs<Type, U>                                                                \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<U> x)                      \
          : x_{strict_cast<Type>(x)} {                                                           \
      }                                                                                          \
                                                                                                 \
      STRICT_NODISCARD_CONSTEXPR Strict<Type> get() const {                                      \
         return x_;                                                                              \
      }                                                                                          \
                                                                                                 \
   private:                                                                                      \
      Strict<Type> x_;                                                                           \
   };


#define STRICT_GENERATE_SMALL_BOOL_TYPES(SmallObjectName, cnd)                        \
   class STRICT_NODISCARD SmallObjectName {                                           \
   public:                                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName() = default;           \
                                                                                      \
      template <Boolean T>                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(T x) : x_{x} {         \
      }                                                                               \
                                                                                      \
      template <Boolean T>                                                            \
      STRICT_NODISCARD_CONSTEXPR explicit(cnd) SmallObjectName(Strict<T> x) : x_{x} { \
      }                                                                               \
                                                                                      \
      STRICT_NODISCARD_CONSTEXPR StrictBool get() const {                             \
         return x_;                                                                   \
      }                                                                               \
                                                                                      \
   private:                                                                           \
      StrictBool x_;                                                                  \
   };


namespace spp {


STRICT_GENERATE_SMALL_TYPES(Low, true)
STRICT_GENERATE_SMALL_TYPES(High, true)
STRICT_GENERATE_SMALL_TYPES(Start, true)
STRICT_GENERATE_SMALL_TYPES(End, true)
STRICT_GENERATE_SMALL_TYPES(Incr, true)
STRICT_GENERATE_SMALL_TYPES(Value, true)


STRICT_GENERATE_SMALL_INT_TYPES(First, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Endmost, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Size, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Count, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Pos, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Stride, true, false)
STRICT_GENERATE_SMALL_INT_TYPES(Index, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Rows, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(Cols, true, true)
STRICT_GENERATE_SMALL_INT_TYPES(ImplicitInt, false, false)
STRICT_GENERATE_SMALL_INT_TYPES(ImplicitNonNegInt, false, true)

STRICT_GENERATE_SMALL_UNSIGNED_TYPES(Seed, true, unsigned)

STRICT_GENERATE_SMALL_BOOL_TYPES(ImplicitBool, false)


class STRICT_NODISCARD ImplicitIntStatic {
public:
   STRICT_NODISCARD_CONSTEXPR ImplicitIntStatic() : x_{} {
   }

   template <SignedInteger T>
   STRICT_NODISCARD_CONSTEXPR ImplicitIntStatic(T x) : x_{static_cast<long int>(x)} {
   }

   template <SignedInteger T>
   STRICT_NODISCARD_CONSTEXPR ImplicitIntStatic(Strict<T> x) : x_{builtin_cast<long int>(x)} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return index_t{x_};
   }

   STRICT_NODISCARD_CONSTEXPR operator ImplicitInt() const {
      return x_;
   }

   // Using index_t type instead of long int would make it
   // unusable in templates as it contains non-public members.
   long int x_;
};


namespace detail {

class STRICT_NODISCARD All{};

class STRICT_NODISCARD Even{};

class STRICT_NODISCARD Odd{};

class STRICT_NODISCARD Reverse{};

}  // namespace detail


// Moved from namespace::detail for ADL.
class STRICT_NODISCARD Last {
public:
   STRICT_NODISCARD_CONSTEXPR explicit Last(ImplicitInt i) : i_{i.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return i_;
   }

private:
   index_t i_;
};


namespace place {
constexpr inline detail::All all;
constexpr inline detail::Even even;
constexpr inline detail::Odd odd;
constexpr inline detail::Reverse reverse;
constexpr inline Last last{0};
}  // namespace place


using place::last;


// Note that plus operator is allowed from both sides bot not minus.
STRICT_NODISCARD_CONSTEXPR_INLINE Last operator+(Last lst, ImplicitInt i) {
   return Last{ImplicitInt{lst.get() - i.get()}};
}


STRICT_NODISCARD_CONSTEXPR_INLINE Last operator+(ImplicitInt i, Last lst) {
   return Last{ImplicitInt{lst.get() - i.get()}};
}


STRICT_NODISCARD_CONSTEXPR_INLINE Last operator-(Last lst, ImplicitInt i) {
   return Last{ImplicitInt{lst.get() + i.get()}};
}


STRICT_NODISCARD_CONSTEXPR_INLINE ImplicitInt operator+(ImplicitInt i, ImplicitInt j) {
   return i.get() + j.get();
}


STRICT_NODISCARD_CONSTEXPR_INLINE ImplicitInt operator-(ImplicitInt i, ImplicitInt j) {
   return i.get() - j.get();
}


STRICT_NODISCARD_CONSTEXPR_INLINE ImplicitIntStatic operator+(ImplicitIntStatic i,
                                                              ImplicitIntStatic j) {
   return i.get() + j.get();
}


STRICT_NODISCARD_CONSTEXPR_INLINE ImplicitIntStatic operator-(ImplicitIntStatic i,
                                                              ImplicitIntStatic j) {
   return i.get() - j.get();
}


}  // namespace spp
