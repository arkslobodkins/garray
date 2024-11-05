#include <cstdlib>
#include <iostream>
#include <string>

#include <strict.hpp>


using namespace spp;


void output_results(Strict64 x, Strict64 y, Strict128 z, std::string op);


// example3 performs product, sum, and dot product on ill-conditioned problems
// using standard and stable variants. Stable norm operation is presented as well.
// Results are compared against results obtained in quadruple precision.
int main() {
   using namespace spp::place;


   // 1.
   index_t n = 100'000_sl;
   Array1D<float64> A = random(n, -2.71_sd, 2.71_sd);
   auto AQ = array_cast<float128>(A);
   output_results(prod(A), stable_prod(A), prod(AQ), "product");


   // 2.
   n = Million<long>;
   Array1D<double> B(n);
   random(B(even), 0._sd, 10._sd);
   B(odd) = -B(even) + 1.E-16_sd;
   auto BQ = array_cast<float128>(B);
   output_results(sum(B), stable_sum(B), sum(BQ), "sum");


   // 3.
   Array1D<double> C1(n), C2(n);
   random(C1(even), 0._sd, 10._sd);
   random(C2(even), 0._sd, 10._sd);
   C1(odd) = C1(even) + 1.E-8_sd;
   C2(odd) = -C2(even) + 1.E-8_sd;
   auto CQ1 = array_cast<float128>(C1);
   auto CQ2 = array_cast<float128>(C2);
   output_results(dot_prod(C1, C2), stable_dot_prod(C1, C2), dot_prod(CQ1, CQ2), "dot product");


   // 4.
   auto D = const1D(Size{n}, Value{0.1});
   auto DQ = array_cast<float128>(D);
   output_results(norm_lp(D, 3), stable_norm_lp(D, 3), norm_lp(DQ, 3), "3-norm");
}


void output_results(Strict64 x, Strict64 y, Strict128 z, std::string op) {
   spp::format.precision(16);
   using std::cout, std::endl;
   std::string st_op = "stable " + op;
   cout << op + " using double precision:        " << x << endl;
   cout << st_op + " using double precision: " << y << endl;
   cout << op + " using quadruple precision:     " << z << endl << endl;
}
