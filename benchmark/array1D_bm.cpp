#include <benchmark/benchmark.h>

#include <Eigen/Dense>
#include <strict.hpp>
#include <tuple>


using namespace spp;


static auto eig_array_init(long int n) {
   return Eigen::ArrayXd(Eigen::ArrayXd::Random(n));
}


static auto strict_array_init(long int n) {
   return Array1D<double, Aligned>(random(n, -1._sd, 1._sd));
}


static auto eig_array_init_all(long int n) {
   return std::tuple{eig_array_init(n), eig_array_init(n), eig_array_init(n)};
}


static auto strict_array_init_all(long int n) {
   return std::tuple{strict_array_init(n), strict_array_init(n), strict_array_init(n)};
}


static void bm_eig_array(benchmark::State& state) {
   auto [A, B, C] = eig_array_init_all(state.range(0));
   for(auto _ : state) {
      benchmark::DoNotOptimize(C = A + B + 2.5);
   }
}


static void bm_strict_array(benchmark::State& state) {
   auto [A, B, C] = strict_array_init_all(state.range(0));
   for(auto _ : state) {
      benchmark::DoNotOptimize(C = A + B + 2.5_sd);
   }
}


static void bm_eig_slice(benchmark::State& state) {
   long int n = state.range(0);
   auto [A, B, C] = eig_array_init_all(n);
   using Eigen::seqN;
   for(auto _ : state) {
      benchmark::DoNotOptimize(C(seqN(0, n / 2, 2))
                               = A(seqN(0, n / 2, 2)) + B(seqN(0, n / 2, 2)) + 2.5);
   }
}


static void bm_strict_slice(benchmark::State& state) {
   auto [A, B, C] = strict_array_init_all(state.range(0));
   using namespace place;
   for(auto _ : state) {
      benchmark::DoNotOptimize(C(even) = A(even) + B(even) + 2.5_sd);
   }
}


BENCHMARK(bm_eig_array)->Args({1 << 10});
BENCHMARK(bm_strict_array)->Args({1 << 10});
BENCHMARK(bm_eig_slice)->Args({1 << 10});
BENCHMARK(bm_strict_slice)->Args({1 << 10});


BENCHMARK_MAIN();
