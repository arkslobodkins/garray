// Arkadijs Slobodkins, 2023


#pragma once


#include <cmath>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "ArrayCommon/array_traits.hpp"
#include "Expr/expr.hpp"
#include "StrictCommon/strict_common.hpp"
#include "derived1D.hpp"
#include "derived2D.hpp"


namespace spp {


template <Builtin T, AlignmentFlag AF>
std::istream& operator>>(std::istream& is, Array1D<T, AF>& A);


template <Builtin T, AlignmentFlag AF>
std::istream& operator>>(std::istream& is, Array2D<T, AF>& A);


template <Builtin T, AlignmentFlag AF>
void read_from_file(const std::string& file_path, Array1D<T, AF>& A);


template <Builtin T, AlignmentFlag AF>
void read_from_file(const std::string& file_path, Array2D<T, AF>& A);


std::ostream& operator<<(std::ostream& os, BaseType auto const& A);


void print(BaseType auto const& A, const std::string& name);


void print(BaseType auto const&... A);


void printn(BaseType auto const& A, const std::string& name);


void printn(BaseType auto const&... A);


void print_to_file(const std::string& file_path, BaseType auto const& A,
                   const std::string& name = "");


STRICT_INLINE std::ostream& operator<<(std::ostream& os, const std::vector<ImplicitInt>& indexes);


namespace detail {

struct ArrayFormat {
public:
   ArrayFormat& detailed(ImplicitBool d) {
      detailed_ = d.get();
      return *this;
   }

   ArrayFormat& row_style() {
      style_ = Row;
      return *this;
   }

   ArrayFormat& col_style() {
      style_ = Column;
      return *this;
   }

   ArrayFormat& reset() {
      detailed_ = false;
      style_ = Column;
      return *this;
   }

   friend std::ostream& ostream_base_print(std::ostream& os, OneDimBaseType auto const& A,
                                           const std::string& name);

   friend std::ostream& ostream_base_print(std::ostream& os, TwoDimBaseType auto const& A,
                                           const std::string& name);

private:
   bool detailed_ = false;
   enum Style : int { Row, Column } style_ = Column;
};

}  // namespace detail
inline detail::ArrayFormat array_format;


namespace detail {


template <Builtin T, AlignmentFlag AF>
std::istream& istream_base_read(std::istream& is, Array1D<T, AF>& A) {
   T x{};
   index_t count{};

   Array1D<T, AF> tmp(8);
   while(is >> x) {
      tmp.un(count++) = Strict{x};
      if(tmp.size() == count) {
         tmp.resize(2_sl * count);
      }
   }

   ASSERT_STRICT_ALWAYS_MSG(is.eof(), "Invalid input.\n");
   is.clear();

   tmp.resize(count);
   A.swap(tmp);
   return is;
}


template <Builtin T, AlignmentFlag AF>
void get_first_row(const std::string& line, Array2D<T, AF>& A) {
   A.resize(1, 4);

   T x{};
   std::istringstream iss{line};
   index_t ncols{};
   while(iss >> x) {
      A(0, ncols++) = Strict{x};
      if(A.cols() == ncols) {
         A.resize(1, 2_sl * ncols);
      }
   }
   if(ncols != 0_sl) {
      A.resize(1, ncols);
   } else {
      A.resize(0, 0);
   }

   ASSERT_STRICT_ALWAYS_MSG(iss.eof(), "Invalid input.\n");
   iss.clear();
}


template <Builtin T, AlignmentFlag AF>
void get_row(const std::string& line, index_t row, Array2D<T, AF>& A) {
   T x{};
   std::istringstream iss{line};
   index_t ncols{};
   while(iss >> x) {
      A(row, ncols++) = Strict{x};
   }

   ASSERT_STRICT_ALWAYS(ncols == A.cols());
   ASSERT_STRICT_ALWAYS_MSG(iss.eof(), "Invalid input.\n");
   iss.clear();
}


template <Builtin T, AlignmentFlag AF>
std::istream& istream_base_read(std::istream& is, Array2D<T, AF>& A) {
   std::string line{};
   Array2D<T, AF> tmp;
   // Important to test tmp.empty() first, otherwise get off-by-1 error.
   while(tmp.empty() && std::getline(is, line)) {
      get_first_row<T>(line, tmp);
   }

   if(!tmp.empty()) {
      const index_t ncols = tmp.cols();
      index_t row_count = ncols == 0_sl ? 0_sl : 1_sl;

      tmp.resize(2, ncols);
      while(std::getline(is, line)) {
         if(line.empty()) {
            continue;
         }
         get_row<T>(line, row_count, tmp);
         ++row_count;
         if(tmp.rows() == row_count) {
            tmp.resize(2_sl * row_count, ncols);
         }
      }
      tmp.resize(row_count, ncols);
   }

   A.swap(tmp);
   return is;
}


auto count_digit(Real auto number) -> int {
   if(number == 0) {
      return 1;
   }
   return static_cast<int>(std::log10(number)) + 1;
};


const auto integer_spacing = []<Integer T>(Strict<T> x) -> int {
   return UnsignedInteger<T> ? count_digit(x.val()) + 1 : count_digit(x.val()) + 2;
};


const auto boolean_spacing = []() -> int { return 6; };


const auto floating_spacing = []<Floating T>(Strict<T> x) -> int {
   return count_digit(x.val()) + format.precision<T>() + 2;
};


STRICT_INLINE std::string smart_spaces(index_t maxi, index_t i) {
   int max_digits = count_digit(maxi.val());
   int ind_digits = count_digit(i.val());
   return std::string(static_cast<std::string::size_type>(1 + max_digits - ind_digits), 32);
}


std::ostream& ostream_base_print(std::ostream& os, OneDimBaseType auto const& A,
                                 const std::string& name) {
   if(!name.empty()) {
      os << name << ':' << '\n';
   }

   if(array_format.detailed_ && A.empty()) {
      os << "[]\n";
   }

   if(array_format.style_ == ArrayFormat::Style::Column) {
      if(!array_format.detailed_) {
         for(auto i : irange(A)) {
            os << A.un(i) << '\n';
         }
      } else {
         for(auto i : irange(A)) {
            os << "[" << i.sul() << "] =" << smart_spaces(A.size(), i) << A.un(i) << '\n';
         }
      }
   } else {
      if(!array_format.detailed_) {
         for(auto i : irange(A)) {
            auto spaces = i == A.size_m1() ? "" : "  ";
            os << A.un(i) << spaces;
         }
      } else {
         for(auto i : irange(A)) {
            auto spaces = i == A.size_m1() ? "" : "  ";
            os << "[" << i.sul() << "] =" << " " << A.un(i) << spaces;
         }
      }
      os << '\n';
   }

   return os << std::flush;
}


auto max_if_needed(TwoDimBaseType auto const& A) {
   using builtin_type = BuiltinTypeOf<decltype(A)>;
   auto max_abs = Zero<builtin_type>;
   if constexpr(Integer<builtin_type> || Floating<builtin_type>) {
      if constexpr(std::is_copy_constructible_v<RemoveRef<decltype(A)>>) {
         if(!A.empty()) {
            max_abs = max(abs(A.view1D()));
         }
      } else {
         // Handle random expression templates differently since max(abs()) operation
         // is not allowed due to deleted copy constructor.
      }
   }
   return max_abs;
}


std::ostream& set_width_if_needed(std::ostream& os, TwoDimBaseType auto const& A, auto max_abs) {
   using builtin_type = BuiltinTypeOf<decltype(A)>;
   if constexpr(Boolean<builtin_type>) {
      return os << std::setw(boolean_spacing());

   } else if constexpr(Integer<builtin_type>) {
      return os << std::setw(integer_spacing(max_abs));

   } else {
      if(format.is_scientific()) {
         return os;
      } else {
         return os << std::setw(floating_spacing(max_abs));
      }
   }
}


std::ostream& ostream_base_print(std::ostream& os, TwoDimBaseType auto const& A,
                                 const std::string& name) {
   if(!name.empty()) {
      os << name << ":" << '\n';
   }

   if(array_format.detailed_ && A.empty()) {
      os << "[]\n";
   }

   auto max_abs = max_if_needed(A);

   for(auto i : irange(A.rows())) {
      for(auto j : irange(A.cols())) {
         auto post_space = j == A.cols_m1() ? "" : "  ";
         if(array_format.detailed_) {
            os << '[' << i.sul() << ", " << j.sul() << "] =" << smart_spaces(A.rows(), i);
         }
         set_width_if_needed(os, A, max_abs) << A.un(i, j) << post_space;
      }
      os << '\n';
   }

   return os << std::flush;
}


template <BaseType Base1, BaseType... BArgs>
void print_helper(const Base1& A1, const BArgs&... AArgs) {
   std::ostringstream stream;
   if constexpr(sizeof...(AArgs) == 0) {
      ostream_base_print(stream, A1, "");
      std::cout << stream.str();
      return;
   } else {
      ostream_base_print(stream, A1, "") << '\n' << std::flush;
      std::cout << stream.str();
      print_helper(AArgs...);
   }
}


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
std::istream& operator>>(std::istream& is, Array1D<T, AF>& A) {
   return detail::istream_base_read(is, A);
}


template <Builtin T, AlignmentFlag AF>
std::istream& operator>>(std::istream& is, Array2D<T, AF>& A) {
   return detail::istream_base_read(is, A);
}


template <Builtin T, AlignmentFlag AF>
void read_from_file(const std::string& file_path, Array1D<T, AF>& A) {
   std::ifstream ifs{file_path};
   ASSERT_STRICT_ALWAYS_MSG(ifs, "Invalid file path.\n");
   detail::istream_base_read(ifs, A);
   ifs.close();
}


template <Builtin T, AlignmentFlag AF>
void read_from_file(const std::string& file_path, Array2D<T, AF>& A) {
   std::ifstream ifs{file_path};
   ASSERT_STRICT_ALWAYS_MSG(ifs, "Invalid file path.\n");
   detail::istream_base_read(ifs, A);
   ifs.close();
}


std::ostream& operator<<(std::ostream& os, BaseType auto const& A) {
   return detail::ostream_base_print(os, A, "");
}


void print(BaseType auto const& A, const std::string& name) {
   std::ostringstream stream;
   detail::ostream_base_print(stream, A, name);
   std::cout << stream.str();
}


void print(BaseType auto const&... A) {
   static_assert(sizeof...(A) > 0);
   detail::print_helper(A...);
}


void printn(BaseType auto const& A, const std::string& name) {
   print(A, name);
   std::cout << '\n' << std::flush;
}


void printn(BaseType auto const&... A) {
   static_assert(sizeof...(A) > 0);
   (..., printn(A, ""));
}


void print_to_file(const std::string& file_path, BaseType auto const& A, const std::string& name) {
   std::ofstream ofs{file_path};
   ASSERT_STRICT_ALWAYS_MSG(ofs, "Invalid file path.\n");
   detail::ostream_base_print(ofs, A, name);
   ofs.close();
}


STRICT_INLINE std::ostream& operator<<(std::ostream& os, const std::vector<ImplicitInt>& indexes) {
   for(auto x : indexes) {
      os << x.get() << '\n';
   }
   return os << std::flush;
}


}  // namespace spp
