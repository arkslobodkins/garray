// Arkadijs Slobodkins, 2023


#pragma once


#include <cassert>
#include <exception>
#include <string>
#include <utility>

#include "config.hpp"


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace spp {


class StrictException : public std::exception {
public:
   explicit StrictException(std::string msg) : msg_{std::move(msg)} {
   }

   [[nodiscard]] const char* what() const noexcept override {
      return msg_.c_str();
   }

private:
   std::string msg_;
};


inline std::string trace_err(const char* file, const char* func, int line) {
   return std::string{"File: "} + std::string{file} + std::string{", function: "}
        + std::string{func} + std::string{", line: "} + std::to_string(line) + std::string{":"}
        + std::string{"\n"};
}


}  // namespace spp


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define NORMAL_ASSERT_STRICT_DEBUG_MSG(condition, msg) \
   do {                                                \
      if(!(condition)) {                               \
         spp::detail::print_stacktrace();              \
         assert((condition) && msg);                   \
      }                                                \
   } while(false)
#define NORMAL_ASSERT_STRICT_DEBUG(condition) NORMAL_ASSERT_STRICT_DEBUG_MSG(condition, "")


#else
#define NORMAL_ASSERT_STRICT_DEBUG_MSG(condition, msg) ((void)0)
#define NORMAL_ASSERT_STRICT_DEBUG(condition) ((void)0)
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_ERROR_EXCEPTIONS
#define ASSERT_STRICT_ALWAYS_MSG(condition, msg) \
   do {                                          \
      if(!(condition)) {                         \
         spp::detail::print_stacktrace();        \
         assert((condition) && msg);             \
      }                                          \
   } while(false)
#define ASSERT_STRICT_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "")


#else
#define ASSERT_STRICT_ALWAYS_MSG(condition, msg)                                       \
   do {                                                                                \
      if(!(condition)) {                                                               \
         throw spp::StrictException{msg + spp::trace_err(__FILE__, __func__, __LINE__) \
                                    + (#condition) + " failed, exception thrown."};    \
      }                                                                                \
   } while(false)
#define ASSERT_STRICT_ALWAYS(condition) ASSERT_STRICT_ALWAYS_MSG(condition, "")
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#ifndef STRICT_DEBUG_OFF
#define ASSERT_STRICT_DEBUG_MSG(condition, msg) ASSERT_STRICT_ALWAYS_MSG(condition, msg)
#define ASSERT_STRICT_DEBUG(condition) ASSERT_STRICT_DEBUG_MSG(condition, "")


#else
#define ASSERT_STRICT_DEBUG_MSG(condition, msg) ((void)0)
#define ASSERT_STRICT_DEBUG(condition) ((void)0)
#endif


////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_STRICT_RANGE_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "OUT OF RANGE!")

#define ASSERT_STRICT_RANGE_ALWAYS(condition) \
   ASSERT_STRICT_ALWAYS_MSG(condition, "OUT OF RANGE!")


////////////////////////////////////////////////////////////////////////////////////////////////////
#define ASSERT_STRICT_DIVISION_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "INTEGER DIVISION BY 0!")

#define ASSERT_STRICT_REMAINDER_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "MODULO DIVISION BY 0!")

#define ASSERT_STRICT_SHIFT_FIRST_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "FIRST SHIFT OPERAND HAS A NEGATIVE VALUE!")

#define ASSERT_STRICT_SHIFT_SECOND_DEBUG(condition) \
   ASSERT_STRICT_DEBUG_MSG(condition, "SECOND SHIFT OPERAND HAS A NEGATIVE VALUE!")
