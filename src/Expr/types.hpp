// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>

#include "../ArrayCommon/array_auxiliary.hpp"
#include "../ArrayCommon/array_traits.hpp"
#include "../ArrayCommon/valid.hpp"
#include "../StrictCommon/strict_common.hpp"
#include "expr_traits.hpp"


namespace spp::detail {


// Workaround before CWG2518/P2593R1.
template <typename>
constexpr bool static_false = false;


// One-dimensional and two-dimensional cases of unary and binary expression templates
// could be rewritten as the same class, where two-dimensional functionality would be
// implemented by adding requires clause to member functions. However, two-dimensional
// functionality would still be shown by autocomplete for one-dimensional types.
// Therefore, specialization is used where each specialization derives from the base class.


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename Op, bool copy_delete>
   requires expr::UnaryOperation<Base, Op>
class STRICT_NODISCARD UnaryExprBase
    : private std::conditional_t<OneDimBaseType<Base>, CopyBase1D, CopyBase2D> {
public:
   // value_type is not always the same as ValueTypeOf<Base>. For example,
   // when converting array to a different type.
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit UnaryExprBase(const Base& A, Op op) : A_{A}, op_{op} {
   }

   STRICT_NODISCARD_CONSTEXPR UnaryExprBase(const UnaryExprBase& E)
      requires(!copy_delete)
       : A_{E.A_},
         op_{E.op_} {
   }

   STRICT_NODISCARD_CONSTEXPR UnaryExprBase(const UnaryExprBase& E)
      requires copy_delete
   = delete;

   STRICT_CONSTEXPR UnaryExprBase& operator=(const UnaryExprBase&) = delete;
   STRICT_CONSTEXPR ~UnaryExprBase() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return op_(A_.un(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A_.size();
   }

protected:
   // Slice arrays are stored by copy, arrays by reference.
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   Op op_;
};


template <BaseType Base, typename Op, bool copy_delete = false>
class STRICT_NODISCARD UnaryExpr;


template <OneDimBaseType Base, typename Op, bool copy_delete>
   requires expr::UnaryOperation<Base, Op>
class STRICT_NODISCARD UnaryExpr<Base, Op, copy_delete>
    : public UnaryExprBase<Base, Op, copy_delete> {
public:
   using UnaryExprBase<Base, Op, copy_delete>::UnaryExprBase;
};


template <TwoDimBaseType Base, typename Op, bool copy_delete>
   requires expr::UnaryOperation<Base, Op>
class STRICT_NODISCARD UnaryExpr<Base, Op, copy_delete>
    : public UnaryExprBase<Base, Op, copy_delete> {
private:
   using ExprBase = UnaryExprBase<Base, Op, copy_delete>;

public:
   using ExprBase::un;  // Unhide.
   using ExprBase::UnaryExprBase;

   STRICT_NODISCARD_CONSTEXPR_INLINE ExprBase::value_type un(ImplicitInt i, ImplicitInt j) const {
      return ExprBase::op_(ExprBase::A_.un(i, j));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t rows() const {
      return ExprBase::A_.rows();
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t cols() const {
      return ExprBase::A_.cols();
   }
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base1, BaseType Base2, typename Op, bool copy_delete>
   requires expr::BinaryOperation<Base1, Base2, Op>
class STRICT_NODISCARD BinaryExprBase
    : private std::conditional_t<OneDimBaseType<Base1>, CopyBase1D, CopyBase2D> {
public:
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base1>{}, ValueTypeOf<Base2>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit BinaryExprBase(const Base1& A1, const Base2& A2, Op op)
       : A1_{A1},
         A2_{A2},
         op_{op} {
      ASSERT_STRICT_DEBUG(same_size(A1_, A2_));
      static_assert(same_dimension<Base1, Base2>());
   }

   STRICT_NODISCARD_CONSTEXPR BinaryExprBase(const BinaryExprBase& E)
      requires(!copy_delete)
       : A1_{E.A1_},
         A2_{E.A2_},
         op_{E.op_} {
   }

   STRICT_NODISCARD_CONSTEXPR BinaryExprBase(const BinaryExprBase& E)
      requires(copy_delete)
   = delete;

   STRICT_CONSTEXPR BinaryExprBase& operator=(const BinaryExprBase&) = delete;
   STRICT_CONSTEXPR ~BinaryExprBase() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return op_(A1_.un(i), A2_.un(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A1_.size();
   }

protected:
   // Slice arrays are stored by copy, arrays by reference.
   typename CopyOrReferenceExpr<AddConst<Base1>>::type A1_;
   typename CopyOrReferenceExpr<AddConst<Base2>>::type A2_;
   Op op_;
};


template <BaseType Base1, BaseType Base2, typename Op, bool copy_delete = false>
class STRICT_NODISCARD BinaryExpr;


template <OneDimBaseType Base1, OneDimBaseType Base2, typename Op, bool copy_delete>
   requires expr::BinaryOperation<Base1, Base2, Op>
class STRICT_NODISCARD BinaryExpr<Base1, Base2, Op, copy_delete>
    : public BinaryExprBase<Base1, Base2, Op, copy_delete> {
public:
   using BinaryExprBase<Base1, Base2, Op, copy_delete>::BinaryExprBase;
};


template <TwoDimBaseType Base1, TwoDimBaseType Base2, typename Op, bool copy_delete>
   requires expr::BinaryOperation<Base1, Base2, Op>
class STRICT_NODISCARD BinaryExpr<Base1, Base2, Op, copy_delete>
    : public BinaryExprBase<Base1, Base2, Op, copy_delete> {
private:
   using ExprBase = BinaryExprBase<Base1, Base2, Op, copy_delete>;

public:
   using ExprBase::BinaryExprBase;
   using ExprBase::un;  // Unhide.

   STRICT_NODISCARD_CONSTEXPR_INLINE ExprBase::value_type un(ImplicitInt i, ImplicitInt j) const {
      return ExprBase::op_(ExprBase::A1_.un(i, j), ExprBase::A2_.un(i, j));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t rows() const {
      return ExprBase::A1_.rows();
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t cols() const {
      return ExprBase::A1_.cols();
   }
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


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, typename Op>
class STRICT_NODISCARD IndexExpr2D : private CopyBase2D {
public:
   using value_type = Strict<T>;
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit IndexExpr2D(index_t rows, index_t cols, Op op)
       : rows_{rows},
         cols_{cols},
         op_{op} {
      ASSERT_STRICT_DEBUG(rows_ > -1_sl);
      ASSERT_STRICT_DEBUG(cols_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR IndexExpr2D(const IndexExpr2D&) = default;
   STRICT_CONSTEXPR IndexExpr2D& operator=(const IndexExpr2D&) = delete;
   STRICT_CONSTEXPR ~IndexExpr2D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      auto [r, c] = index_map_one_to_two_dim(*this, i);
      return this->un(r, c);
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i, ImplicitInt j) const {
      return op_(i.get(), j.get());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return rows_ * cols_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t rows() const {
      return rows_;
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t cols() const {
      return cols_;
   }

private:
   index_t rows_;
   index_t cols_;
   Op op_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <TwoDimBaseType Base, typename Op, bool rowwise>
class STRICT_NODISCARD ReduceExpr : private CopyBase1D {
   using row_type = decltype(std::declval<Base>().lval().row(0));
   using col_type = decltype(std::declval<Base>().lval().col(0));

public:
   using value_type = decltype(std::declval<Op>()(
       std::declval<std::conditional_t<rowwise, row_type, col_type>>()));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit ReduceExpr(const Base& A, Op op) : A_{A}, op_{op} {
   }

   STRICT_NODISCARD_CONSTEXPR ReduceExpr(const ReduceExpr& E) = default;
   STRICT_CONSTEXPR ReduceExpr& operator=(const ReduceExpr&) = delete;
   STRICT_CONSTEXPR ~ReduceExpr() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      if constexpr(rowwise) {
         return op_(A_.row(i));
      } else {
         return op_(A_.col(i));
      }
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      if constexpr(rowwise) {
         return A_.rows();
      } else {
         return A_.cols();
      }
   }

private:
   // Slice arrays are stored by copy, arrays by reference.
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   Op op_;
};


template <BaseType Base, typename Op>
   requires expr::UnaryOperation<Base, Op>
class STRICT_NODISCARD RandUnaryExpr : public UnaryExpr<Base, Op, true> {
public:
   using typename UnaryExpr<Base, Op, true>::value_type;
   using typename UnaryExpr<Base, Op, true>::builtin_type;

   STRICT_NODISCARD_CONSTEXPR explicit RandUnaryExpr(const Base& A, Op op, value_type low,
                                                     value_type high)
       : UnaryExpr<Base, Op, true>{A, op},
         low_{low},
         high_{high} {
   }

   STRICT_NODISCARD_CONSTEXPR RandUnaryExpr(const RandUnaryExpr& E) = default;
   STRICT_CONSTEXPR RandUnaryExpr& operator=(const RandUnaryExpr&) = delete;
   STRICT_CONSTEXPR ~RandUnaryExpr() = default;

   STRICT_CONSTEXPR value_type low_rand() const {
      return low_;
   }

   STRICT_CONSTEXPR value_type high_rand() const {
      return high_;
   }

private:
   value_type low_, high_;
};


}  // namespace spp::detail

