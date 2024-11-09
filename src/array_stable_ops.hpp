// Arkadijs Slobodkins, 2023


#pragma once


#include <tuple>

#include "ArrayCommon/array_auxiliary.hpp"
#include "ArrayCommon/array_traits.hpp"
#include "Expr/expr.hpp"
#include "StrictCommon/strict_common.hpp"
#include "array_ops.hpp"
#include "derived1D.hpp"


namespace spp {


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_sum(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> semi_stable_sum(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_prod(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_mean(const Base& A);


template <FloatingBaseType Base1, FloatingBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
ValueTypeOf<Base1> stable_dot_prod(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base1, FloatingBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
ValueTypeOf<Base1> semi_stable_dot_prod(const Base1& A1, const Base2& A2);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm1(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm1_scaled(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm2(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm2_scaled(const Base& A);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm_lp(const Base& A, ImplicitInt lp);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm_lp_scaled(const Base& A, ImplicitInt lp);


template <OneDimBaseType Base1>
ValueTypeOf<Base1> stable_polynomial(const Base1& coeffs, ValueTypeOf<Base1> x);


template <OneDimBaseType Base1, OneDimSignedIntegerBaseType Base2>
ValueTypeOf<Base1> stable_polynomial(const Base1& coeffs, ValueTypeOf<Base1> x,
                                     const Base2& powers);


template <OneDimFloatingBaseType Base1, OneDimFloatingBaseType Base2,
          OneDimSignedIntegerBaseType Base3>
ValueTypeOf<Base1> stable_gpolynomial(const Base1& coeffs, const Base2& X, const Base3& powers);


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_sum(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<Base>;
   using value_type = ValueTypeOf<Base>;

   real_type s{};
   real_type c{};
   for(index_t i = 0_sl; i < A.size(); ++i) {
      auto xi = A.un(i);
      volatile real_type t = s + xi.val();
      if(abss(value_type{s}) >= abss(xi)) {
         volatile real_type z = s - t;
         c += z + xi.val();
      } else {
         volatile real_type z = xi.val() - t;
         c += z + s;
      }
      s = t;
   }
   return value_type{s + c};
}


template <FloatingBaseType Base>
ValueTypeOf<Base> semi_stable_sum(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   index_t block_size = 64_sl;
   index_t nblocks = A.size() / block_size;
   index_t rem = A.size() % block_size;

   Array1D<RealTypeOf<Base>> sb(nblocks);
   for(index_t i = 0_sl; i < nblocks; ++i) {
      for(index_t j = 0_sl; j < block_size; ++j) {
         sb[i] += A.un(i * block_size + j);
      }
   }

   ValueTypeOf<Base> r{};
   for(index_t i = 0_sl; i < rem; ++i) {
      r += A.un(A.size() - 1_sl - i);
   }

   if(sb.empty()) {
      return r;
   }

   return stable_sum(sb) + r;
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_prod(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   using real_type = RealTypeOf<Base>;

   Array1D<real_type> res(A.size() - 1_sl);
   auto p = A.un(0);
   for(index_t i = 1_sl; i < A.size(); ++i) {
      res.un(i - 1_sl) = two_prods(p, A.un(i)).second;
      p *= A.un(i);
   }

   Strict<real_type> prod_temp{real_type(1)};
   auto offset = generate(irange(A.size() - 1_sl), [&A, &res, &prod_temp](auto i) {
      auto t = prod_temp * res.un(res.size() - 1_sl - i);
      prod_temp *= A.un(A.size() - 1_sl - i);
      return t;
   });
   return p + stable_sum(offset);
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_mean(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return stable_sum(A) / value_type_cast<Base>(A.size());
}


template <FloatingBaseType Base1, FloatingBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
ValueTypeOf<Base1> stable_dot_prod(const Base1& A1, const Base2& A2) {
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));

   auto [p, s] = two_prods(A1.un(0), A2.un(0));
   decltype(p) q;
   for(index_t i = 1_sl; i < A1.size(); ++i) {
      auto [h, r] = two_prods(A1.un(i), A2.un(i));
      std::tie(p, q) = two_sums(p, h);
      s = s + (q + r);
   }

   return p + s;
}


template <FloatingBaseType Base1, FloatingBaseType Base2>
   requires(same_dimension_b<Base1, Base2>())
ValueTypeOf<Base1> semi_stable_dot_prod(const Base1& A1, const Base2& A2) {
   ASSERT_STRICT_DEBUG(!A1.empty());
   ASSERT_STRICT_DEBUG(same_size(A1, A2));
   return semi_stable_sum(A1 * A2);
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm1(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return stable_sum(abs(A));
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm1_scaled(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return stable_norm1(A) / value_type_cast<Base>(A.size());
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm2(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return sqrts(stable_dot_prod(A, A));
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm2_scaled(const Base& A) {
   ASSERT_STRICT_DEBUG(!A.empty());
   return stable_norm2(A) / sqrts(value_type_cast<Base>(A.size()));
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm_lp(const Base& A, ImplicitInt lp) {
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(lp.get() > 0_sl);

   auto pw = lp.get();
   auto sz = A.size();

   using real_type = RealTypeOf<Base>;
   auto coeff = const1D(Size{sz}, Value{real_type(1)});
   auto P = const1D(Size{sz}, Value(pw));
   auto s = stable_gpolynomial(coeff, abs(A.view1D()), P);
   return pows(s, invs(value_type_cast<Base>(pw)));
}


template <FloatingBaseType Base>
ValueTypeOf<Base> stable_norm_lp_scaled(const Base& A, ImplicitInt lp) {
   ASSERT_STRICT_DEBUG(!A.empty());
   ASSERT_STRICT_DEBUG(lp.get() > 0_sl);

   return stable_norm_lp(A, lp)
        / pows(value_type_cast<Base>(A.size()), invs(value_type_cast<Base>(lp.get())));
}


template <OneDimBaseType Base1>
ValueTypeOf<Base1> stable_polynomial(const Base1& coeffs, ValueTypeOf<Base1> x) {
   ASSERT_STRICT_DEBUG(!coeffs.empty());

   auto X = const1D(Size{coeffs.size()}, Value{x});
   auto powers = sequence(Size{coeffs.size()}, Start{0}, Incr{1});
   return stable_gpolynomial(coeffs, X, powers);
}


template <OneDimBaseType Base1, OneDimSignedIntegerBaseType Base2>
ValueTypeOf<Base1> stable_polynomial(const Base1& coeffs, ValueTypeOf<Base1> x,
                                     const Base2& powers) {
   ASSERT_STRICT_DEBUG(!coeffs.empty());
   ASSERT_STRICT_DEBUG(same_size(coeffs, powers));
   ASSERT_STRICT_DEBUG(all_non_neg(powers));

   auto X = const1D(Size{coeffs.size()}, Value{x});
   return stable_gpolynomial(coeffs, X, powers);
}


template <OneDimFloatingBaseType Base1, OneDimFloatingBaseType Base2,
          OneDimSignedIntegerBaseType Base3>
ValueTypeOf<Base1> stable_gpolynomial(const Base1& coeffs, const Base2& X, const Base3& powers) {
   ASSERT_STRICT_DEBUG(!X.empty());
   ASSERT_STRICT_DEBUG(same_size(coeffs, X, powers));
   ASSERT_STRICT_DEBUG(all_non_neg(powers));

   auto R = pow_prod(X, powers).first;
   auto S = pow_prod(X, powers).second;
   auto coeff_pair = merge(coeffs, coeffs);
   auto RS = merge(R, S);
   return stable_dot_prod(coeff_pair, RS);
}


}  // namespace spp
