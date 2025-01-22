// Arkadijs Slobodkins, 2023


#pragma once


#ifdef STRICT_QUAD_PRECISION
#include <cstddef>  // ptrdiff_t
#endif
#include <cstring>
#include <iomanip>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

#include "auxiliary_types.hpp"
#include "common_traits.hpp"
#include "error.hpp"
#include "strict_traits.hpp"
#include "strict_val.hpp"


namespace spp {


template <NotQuadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x);


template <Boolean T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);


template <Integer T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);


template <StandardFloating T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUAD_PRECISION
template <Quadruple T>
std::istream& operator>>(std::istream& is, T& x);


template <Quadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x);


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, T x);


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, Strict<T> x);
#endif


template <Builtin T>
void print(Strict<T> x, const std::string& name);


void print(StrictType auto const&... x);


template <Builtin T>
void printn(Strict<T> x, const std::string& name);


void printn(StrictType auto const&... x);


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace detail {


// Scientific, precision, and reset routines are provided so that
// the user can choose the IO format for all floating-point types.
class StrictFormat {
public:
   StrictFormat& reset() {
      scientific_ = true_sb;
      precision_[0] = float_precision;
      precision_[1] = double_precision;
      precision_[2] = long_double_precision;
      precision_[3] = quad_precision;
      return *this;
   }

   StrictBool is_scientific() const {
      return scientific_;
   }

   StrictFormat& scientific(ImplicitBool b) {
      scientific_ = b.get();
      return *this;
   }

   template <Floating PT>
   StrictInt precision() const {
      if constexpr(SameAs<PT, float>) {
         return Strict{precision_[0]};
      } else if constexpr(SameAs<PT, double>) {
         return Strict{precision_[1]};
      } else if constexpr(SameAs<PT, long double>) {
         return Strict{precision_[2]};
      } else {
         return Strict{precision_[3]};
      }
   }

   StrictFormat& precision(ImplicitNonNegInt n) {
      precision_[0] = static_cast<int>(n.get().val());
      precision_[1] = static_cast<int>(n.get().val());
      precision_[2] = static_cast<int>(n.get().val());
      precision_[3] = static_cast<int>(n.get().val());
      return *this;
   }

   StrictFormat& precision_float(ImplicitNonNegInt n) {
      precision_[0] = static_cast<int>(n.get().val());
      return *this;
   }

   StrictFormat& precision_double(ImplicitNonNegInt n) {
      precision_[1] = static_cast<int>(n.get().val());
      return *this;
   }

   StrictFormat& precision_long_double(ImplicitNonNegInt n) {
      precision_[2] = static_cast<int>(n.get().val());
      return *this;
   }

#ifdef STRICT_QUAD_PRECISION
   StrictFormat& precision_quad(ImplicitNonNegInt n) {
      precision_[3] = static_cast<int>(n.get().val());
      return *this;
   }
#endif

   // Must explicitly qualify namespace.
   template <StandardFloating T>
   friend std::ostream& spp::operator<<(std::ostream& os, Strict<T> x);

#ifdef STRICT_QUAD_PRECISION
   template <Quadruple T>
   friend std::ostream& spp::operator<<(std::ostream& os, T x);
#endif

private:
   static constexpr int float_precision = std::numeric_limits<float>::digits10 + 1;
   static constexpr int double_precision = std::numeric_limits<double>::digits10 + 1;
   static constexpr int long_double_precision = std::numeric_limits<long double>::digits10 + 1;
   static constexpr int quad_precision = 33;

   StrictBool scientific_{true};
   int precision_[4]{float_precision, double_precision, long_double_precision, quad_precision};

   // Originally a templated lambda inside << operator, but changed to private member
   // function since CUDA(even CUDA 12.6) does not seem to support templated lambdas.
   template <StandardFloating T>
   auto set_float_precision() const {
      if constexpr(SameAs<T, float>) {
         return std::setprecision(precision_[0]);
      } else if constexpr(SameAs<T, double>) {
         return std::setprecision(precision_[1]);
      } else {
         return std::setprecision(precision_[2]);
      }
   }
};


}  // namespace detail
inline detail::StrictFormat format;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <NotQuadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x) {
   T v;
   is >> v;
   ASSERT_STRICT_ALWAYS(is);
   x = Strict{v};
   return is;
}


template <Boolean T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   x ? os << "true" : os << "false";
   return os;
}


template <Integer T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   // Sign of unsigned integers is not printed so that
   // one can distinguish signed and unsigned integers.
   os << std::showpos << T{x};
   return os;
}


template <StandardFloating T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   os << std::showpos;
   if(format.scientific_) {
      os << std::scientific << std::showpoint << format.set_float_precision<T>() << T{x};
   } else {
      os << std::fixed << std::showpoint << format.set_float_precision<T>() << T{x};
   }
   return os;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifdef STRICT_QUAD_PRECISION
// Implementation from boost/multiprecision/float128.hpp.
template <Quadruple T>
std::istream& operator>>(std::istream& is, T& x) {
   std::string str;
   is >> str;
   const char* s = str.c_str();
   char* p_end;
   x = strtoflt128(s, &p_end);
   ASSERT_STRICT_ALWAYS(p_end - s == (std::ptrdiff_t)std::strlen(s));

   return is;
}


template <Quadruple T>
std::istream& operator>>(std::istream& is, Strict<T>& x) {
   float128 v;
   is >> v;
   x = Strict{v};
   return is;
}


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, T x) {
   char buf[128];  // Not declared static since it is not thread safe.
   if(format.scientific_) {
      std::string frmt = "%+-#*." + std::to_string(format.precision_[3]) + std::string("QE");
      int width = format.precision_[3] + 7;
      quadmath_snprintf(buf, sizeof(buf), frmt.c_str(), width, x);
   } else {
      std::string frmt = "%+-#*." + std::to_string(format.precision_[3]) + std::string("QF");
      int width = format.precision_[3] + 3;
      quadmath_snprintf(buf, sizeof(buf), frmt.c_str(), width, x);
   }
   os << buf;
   return os;
}


template <Quadruple T>
std::ostream& operator<<(std::ostream& os, Strict<T> x) {
   return os << T{x};
}
#endif


template <Builtin T>
void print(Strict<T> x, const std::string& name) {
   std::ostringstream stream;
   if(!name.empty()) {
      stream << name << ":" << '\n';
   }
   stream << x;
   std::cout << stream.str() << '\n';
}


void print(StrictType auto const&... x) {
   (..., print(x, ""));
}


template <Builtin T>
void printn(Strict<T> x, const std::string& name) {
   print(x, name);
   std::cout << '\n';
}


void printn(StrictType auto const&... x) {
   (..., print(x, ""));
   std::cout << '\n';
}


}  // namespace spp

