#pragma once


#include <iostream>


#define STRICT_ERROR_EXCEPTIONS
#define STRICT_REMOVE_NODISCARD
#include "../src/strict.hpp"


namespace spp {


static inline void compiler_info() {
#ifdef __VERSION__
   std::cout << "compiler version: " << __VERSION__ << std::endl;
#endif
}


static inline void set_red_output() {
   std::cout << "\033[1;31m";
}


static inline void set_green_output() {
   std::cout << "\033[1;32m";
}


static inline void set_cyan_output() {
   std::cout << "\033[1;36m";
}


static inline void reset_output() {
   std::cout << "\033[1;0m";
}


}  // namespace spp


#define REQUIRE_THROW(condition)                                                              \
   do {                                                                                       \
      try {                                                                                   \
         condition;                                                                           \
         spp::detail::print_stacktrace();                                                     \
         std::cerr << trace_err(__FILE__, __func__, __LINE__) << "Assertion " << (#condition) \
                   << " did not throw exception." << std::endl;                               \
         std::abort();                                                                        \
      } catch(const spp::StrictException&) {                                                  \
      }                                                                                       \
   } while(false)


#define REQUIRE_NOT_THROW(condition)                                                          \
   do {                                                                                       \
      try {                                                                                   \
         condition;                                                                           \
      } catch(const spp::StrictException&) {                                                  \
         spp::detail::print_stacktrace();                                                     \
         std::cerr << trace_err(__FILE__, __func__, __LINE__) << "Assertion " << (#condition) \
                   << " threw exception." << std::endl;                                       \
         std::abort();                                                                        \
      }                                                                                       \
   } while(false)


#define ASSERT(cnd) ASSERT_STRICT_ALWAYS(cnd)


#define PERFORM_FUNCTION_CALLS(function_name, ...)              \
   do {                                                         \
      try {                                                     \
         __VA_ARGS__;                                           \
         spp::set_green_output();                               \
         std::cout << "passed " << #function_name << std::endl; \
         spp::reset_output();                                   \
      } catch(const StrictException& e) {                       \
         spp::set_red_output();                                 \
         std::cout << e.what() << std::endl;                    \
         spp::reset_output();                                   \
      }                                                         \
   } while(false)


#ifdef STRICT_QUAD_PRECISION
#define FLOAT128_TEST(function_name, ...) function_name<float128>(__VA_ARGS__)
#else
#define FLOAT128_TEST(function_name, ...) (void(0));
#endif


#define TEST_ALL_TYPES(function_name, ...)                                 \
   PERFORM_FUNCTION_CALLS(function_name, function_name<bool>(__VA_ARGS__); \
                          function_name<int>(__VA_OPT__(__VA_ARGS__));     \
                          function_name<long int>(__VA_ARGS__);            \
                          function_name<unsigned int>(__VA_ARGS__);        \
                          function_name<unsigned long int>(__VA_ARGS__);   \
                          function_name<float>(__VA_ARGS__);               \
                          function_name<double>(__VA_ARGS__);              \
                          function_name<long double>(__VA_ARGS__);         \
                          FLOAT128_TEST(function_name, __VA_ARGS__))


#define TEST_ALL_REAL_TYPES(function_name, ...)                           \
   PERFORM_FUNCTION_CALLS(function_name, function_name<int>(__VA_ARGS__); \
                          function_name<long int>(__VA_ARGS__);           \
                          function_name<unsigned int>(__VA_ARGS__);       \
                          function_name<unsigned long int>(__VA_ARGS__);  \
                          function_name<float>(__VA_ARGS__);              \
                          function_name<double>(__VA_ARGS__);             \
                          function_name<long double>(__VA_ARGS__);        \
                          FLOAT128_TEST(function_name, __VA_ARGS__))


#define TEST_ALL_FLOAT_TYPES(function_name, ...)                            \
   PERFORM_FUNCTION_CALLS(function_name, function_name<float>(__VA_ARGS__); \
                          function_name<double>(__VA_ARGS__);               \
                          function_name<long double>(__VA_ARGS__);          \
                          FLOAT128_TEST(function_name, __VA_ARGS__))


#define TEST_STANDARD_FLOAT_TYPES(function_name, ...)                       \
   PERFORM_FUNCTION_CALLS(function_name, function_name<float>(__VA_ARGS__); \
                          function_name<double>(__VA_ARGS__);               \
                          function_name<long double>(__VA_ARGS__))


#define TEST_NON_TYPE(function_name, ...) \
   PERFORM_FUNCTION_CALLS(function_name, function_name(__VA_ARGS__))

