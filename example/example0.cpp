#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>

#include <strict.hpp>


// example0 demonstrates how Strict arithmetic types can help
// to prevent errors that arise when using built-in arithmetic types.
int main() {
   using namespace std;
   using namespace spp;


   // 1.
   long double ld_bad{2.2};    // Loses precision, likely a bug.
   long double ld_good{2.2L};  // Ok, retains all digits.
   cout << scientific << setprecision(18);
   cout << "long double initialized with 2.2:   " << ld_bad << '\n';
   cout << "long double initialized with 2.2L:  " << ld_good << "\n\n";

   Strict<long double> sl{2.2L};
   // Strict<long double> s3{2.2}; // Won't compile!


   // 2.
   double A[]{0.1, 0.2, 0.3, 0.4, 0.5};
   cout << "result of accumulate when passing 0(deduces int):      "
        << accumulate(begin(A), end(A), 0) << "\n";  // Accumulates in int, likely a bug.

   cout << "result of accumulate when passing 0.(deduces double):  "
        << accumulate(begin(A), end(A), 0.) << "\n\n";  // Ok, accumulates in double.

   Strict64 B[]{0.1_sd, 0.2_sd, 0.3_sd, 0.4_sd, 0.5_sd};
   (void)accumulate(begin(B), end(B), 0._sd);  // As expected.
   // accumulate(begin(B), end(B), 0_si);   // Won't compile!!!


   // 3.
   // int sure_int1 = 5'000'000'000;  // Wrongfully expecting an int, hopefully a warning from
   // compiler. Strict<int> sure_int2 = 5'000'000'000; // Compiler error, don't need to rely on
   // warnings.

   return EXIT_SUCCESS;
}
