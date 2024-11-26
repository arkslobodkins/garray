// Arkadijs Slobodkins, 2023


#pragma once


#include <concepts>
#include <type_traits>
#include <utility>

#include "../ArrayCommon/array_auxiliary.hpp"
#include "../ArrayCommon/array_traits.hpp"
#include "../ArrayCommon/valid.hpp"
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
template <BaseType Base, typename Op, bool copy_delete = false>
   requires expr::UnaryOperation<Base, Op>
class STRICT_NODISCARD UnaryExpr
    : private std::conditional_t<OneDimBaseType<Base>, CopyBase1D, CopyBase2D> {
public:
   // value_type is not always the same as ValueTypeOf<Base>. For example,
   // when converting array to a different type.
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit UnaryExpr(const Base& A, Op op) : A_{A}, op_{op} {
   }

   STRICT_NODISCARD_CONSTEXPR UnaryExpr(const UnaryExpr& E) : A_{E.A_}, op_{E.op_} {
      if constexpr(copy_delete) {
         static_assert(static_false<decltype(*this)>,
                       "Copying this expression template is not allowed for additional safety");
      }
   }

   STRICT_CONSTEXPR UnaryExpr& operator=(const UnaryExpr&) = delete;
   STRICT_CONSTEXPR ~UnaryExpr() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return op_(A_.un(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i, ImplicitInt j) const
      requires TwoDimBaseType<Base>
   {
      return op_(A_.un(i, j));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A_.size();
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t rows() const
      requires TwoDimBaseType<Base>
   {
      return A_.rows();
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t cols() const
      requires TwoDimBaseType<Base>
   {
      return A_.cols();
   }

private:
   // Slice arrays are stored by copy, arrays by reference.
   typename CopyOrReferenceExpr<AddConst<Base>>::type A_;
   Op op_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base1, BaseType Base2, typename Op, bool copy_delete = false>
   requires expr::BinaryOperation<Base1, Base2, Op>
class STRICT_NODISCARD BinaryExpr
    : private std::conditional_t<OneDimBaseType<Base1>, CopyBase1D, CopyBase2D> {
public:
   using value_type = decltype(std::declval<Op>()(ValueTypeOf<Base1>{}, ValueTypeOf<Base2>{}));
   using builtin_type = value_type::value_type;

   STRICT_NODISCARD_CONSTEXPR explicit BinaryExpr(const Base1& A1, const Base2& A2, Op op)
       : A1_{A1},
         A2_{A2},
         op_{op} {
      ASSERT_STRICT_DEBUG(same_size(A1_, A2_));
      static_assert(same_dimension<Base1, Base2>());
   }

   STRICT_NODISCARD_CONSTEXPR BinaryExpr(const BinaryExpr& E) : A1_{E.A1_}, A2_{E.A2_}, op_{E.op_} {
      if constexpr(copy_delete) {
         static_assert(static_false<decltype(*this)>,
                       "Copying this expression template is not allowed for additional safety");
      }
   }

   STRICT_CONSTEXPR BinaryExpr& operator=(const BinaryExpr&) = delete;
   STRICT_CONSTEXPR ~BinaryExpr() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return op_(A1_.un(i), A2_.un(i));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i, ImplicitInt j) const
      requires TwoDimBaseType<Base1>
   {
      return op_(A1_.un(i, j), A2_.un(i, j));
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t size() const {
      return A1_.size();
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t rows() const
      requires TwoDimBaseType<Base1>
   {
      return A1_.rows();
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE index_t cols() const
      requires TwoDimBaseType<Base2>
   {
      return A1_.cols();
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


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Real T>
class STRICT_NODISCARD SequenceExpr2D : private CopyBase2D {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // incr can be 0 or negative.
   STRICT_NODISCARD_CONSTEXPR explicit SequenceExpr2D(value_type start, index_t rows, index_t cols,
                                                      value_type incr)
       : start_{start},
         rows_{rows},
         cols_{cols},
         incr_{incr} {
      ASSERT_STRICT_DEBUG(rows_ > -1_sl);
      ASSERT_STRICT_DEBUG(cols_ > -1_sl);
      ASSERT_STRICT_DEBUG(semi_valid_row_col_sizes(rows_, cols_));
   }

   STRICT_NODISCARD_CONSTEXPR SequenceExpr2D(const SequenceExpr2D&) = default;
   STRICT_CONSTEXPR SequenceExpr2D& operator=(const SequenceExpr2D&) = delete;
   STRICT_CONSTEXPR ~SequenceExpr2D() = default;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i) const {
      return start_ + incr_ * strict_cast<builtin_type>(i.get());
   }

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type un(ImplicitInt i, ImplicitInt j) const {
      return this->un(i.get() * cols_ + j.get());
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
   value_type start_;
   index_t rows_;
   index_t cols_;
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
      auto row_pos = i.get() / cols_;
      auto col_pos = i.get() % cols_;
      return this->un(row_pos, col_pos);
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
   Op op_;
   index_t rows_;
   index_t cols_;
};


}  // namespace detail


}  // namespace spp

