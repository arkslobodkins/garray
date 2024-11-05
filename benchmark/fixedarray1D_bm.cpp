#include <benchmark/benchmark.h>

#include <Eigen/Dense>
#include <strict.hpp>
#include <tuple>


using namespace spp;


template <int N>
static auto eig_array_init() {
   return Eigen::Array<double, N, 1>(Eigen::ArrayXd::Random(N));
}


template <int N>
static auto strict_array_init() {
   return FixedArray1D<double, N>(random(N, -1._sd, 1._sd));
}


template <int N>
static auto eig_array_init_all() {
   return std::tuple{eig_array_init<N>(), eig_array_init<N>(), eig_array_init<N>()};
}


template <int N>
static auto strict_array_init_all() {
   return std::tuple{strict_array_init<N>(), strict_array_init<N>(), strict_array_init<N>()};
}


template <int N>
static void bm_eig_array(benchmark::State& state) {
   auto [A, B, C] = eig_array_init_all<N>();
   for(auto _ : state) {
      benchmark::DoNotOptimize(C = A + B + 2.5);
   }
}


template <int N>
static void bm_strict_array(benchmark::State& state) {
   auto [A, B, C] = strict_array_init_all<N>();
   for(auto _ : state) {
      benchmark::DoNotOptimize(C = A + B + 2.5_sd);
   }
}


template <int N>
static void bm_eig_slice(benchmark::State& state) {
   auto [A, B, C] = eig_array_init_all<N>();
   using Eigen::seqN;
   for(auto _ : state) {
      benchmark::DoNotOptimize(C(seqN(0, N / 2, 2))
                               = A(seqN(0, N / 2, 2)) + B(seqN(0, N / 2, 2)) + 2.5);
   }
}


template <int N>
static void bm_strict_slice(benchmark::State& state) {
   auto [A, B, C] = strict_array_init_all<N>();
   using namespace place;
   for(auto _ : state) {
      benchmark::DoNotOptimize(C(even) = A(even) + B(even) + 2.5_sd);
   }
}


BENCHMARK(bm_eig_array<1 << 10>);
BENCHMARK(bm_strict_array<1 << 10>);
BENCHMARK(bm_eig_slice<1 << 10>);
BENCHMARK(bm_strict_slice<1 << 10>);


BENCHMARK_MAIN();
