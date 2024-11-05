// Arkadijs Slobodkins, 2023


#pragma once


#include <concepts>
#include <type_traits>
#include <utility>

#include "../ArrayCommon/array_auxiliary.hpp"
#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"


namespace spp {


namespace expr {


template <typename T, typename F> concept UnaryOperation
    = std::invocable<F, ValueTypeOf<T>>
   && StrictType<decltype(std::declval<F>()(ValueTypeOf<T>{}))>;


template <typename T1, typename T2, typename F> concept BinaryOperation
    = std::invocable<F, ValueTypeOf<T1>, ValueTypeOf<T2>>
   && StrictType<decltype(std::declval<F>()(ValueTypeOf<T1>{}, ValueTypeOf<T2>{}))>;


}  // namespace expr


namespace detail {


// Workaround before CWG2518/P2593R1.
template <typename>
constexpr bool static_false = false;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base, typename Op, bool copy_delete = false>
   requires expr::UnaryOperation<Base, Op>
class STRICT_NODISCARD UnaryExpr1D : private CopyBase1D {
public:
   // value_type is not always the same as ValueTypeOf<Base>. For example,
   // when converting array to a different type.
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit UnaryExpr1D(const Base& A, Op op) : A_{A}, op_{op} {
   }

   STRICT_NODISCARD_CONSTEXPR UnaryExpr1D(const UnaryExpr1D& E) : A_{E.A_}, op_{E.op_} {
      if constexpr(copy_delete) {
         static_assert(static_false<decltype(*this)>,
                       "Copying this expression template is not allowed for additional safety");
      }
   }

   STRICT_CONSTEXPR UnaryExpr1D& operator=(const UnaryExpr1D&) = delete;
   STRICT_CONSTEXPR ~UnaryExpr1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return op_(A_.un(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A_.size();
   }

private:
   // Slice arrays are stored by copy, arrays by reference.
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   Op op_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <OneDimBaseType Base1, OneDimBaseType Base2, typename Op, bool copy_delete = false>
   requires expr::BinaryOperation<Base1, Base2, Op>
class STRICT_NODISCARD BinaryExpr1D : private CopyBase1D {
public:
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base1>{}, ValueTypeOf<Base2>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit BinaryExpr1D(const Base1& A1, const Base2& A2, Op op)
       : A1_{A1},
         A2_{A2},
         op_{op} {
      ASSERT_STRICT_DEBUG(same_size(A1_, A2_));
   }

   STRICT_NODISCARD_CONSTEXPR BinaryExpr1D(const BinaryExpr1D& E)
       : A1_{E.A1_},
         A2_{E.A2_},
         op_{E.op_} {
      if constexpr(copy_delete) {
         static_assert(static_false<decltype(*this)>,
                       "Copying this expression template is not allowed for additional safety");
      }
   }

   STRICT_CONSTEXPR BinaryExpr1D& operator=(const BinaryExpr1D&) = delete;
   STRICT_CONSTEXPR ~BinaryExpr1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return op_(A1_.un(i), A2_.un(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A1_.size();
   }

private:
   // Slice arrays are stored by copy, arrays by reference.
   typename CopyOrReferenceExpr<AddConst<Base1>>::type A1_;
   typename CopyOrReferenceExpr<AddConst<Base2>>::type A2_;
   Op op_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
class STRICT_NODISCARD SequenceExpr1D : private CopyBase1D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // incr can be 0 or negative.
   STRICT_NODISCARD_CONSTEXPR explicit SequenceExpr1D(value_type start, index_t size,
                                                      value_type incr)
       : start_{start},
         size_{size},
         incr_{incr} {
      ASSERT_STRICT_DEBUG(size_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR SequenceExpr1D(const SequenceExpr1D&) = default;
   STRICT_CONSTEXPR SequenceExpr1D& operator=(const SequenceExpr1D&) = delete;
   STRICT_CONSTEXPR ~SequenceExpr1D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return start_ + incr_ * strict_cast<builtin_type>(i.get());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return size_;
   }

private:
   value_type start_;
   index_t size_;
   value_type incr_;
};


}  // namespace detail


}  // namespace spp

