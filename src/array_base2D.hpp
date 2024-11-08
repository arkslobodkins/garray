// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>
#include <vector>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"
#include "array_base1D.hpp"
#include "fixed_array_base1D.hpp"


namespace spp { namespace detail {


template <typename Base>
class GenArray2D;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
class STRICT_NODISCARD ArrayBase2D : private ReferenceBase2D, private TwoDimArrayBase {
public:
   using value_type = Strict<T>;
   using builtin_type = T;

   // Constructors.
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase2D() = default;
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase2D(ImplicitInt m, ImplicitInt n);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase2D(Rows m, Cols n);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase2D(ImplicitInt m, ImplicitInt n, value_type x);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase2D(Rows m, Cols n, Value<T> x);
   STRICT_NODISCARD_CONSTEXPR explicit ArrayBase2D(use::List2D<T> list);

   STRICT_NODISCARD_CONSTEXPR ArrayBase2D(const ArrayBase2D& A) = default;
   STRICT_NODISCARD_CONSTEXPR ArrayBase2D(ArrayBase2D&& A) noexcept = default;
   STRICT_NODISCARD_CONSTEXPR ArrayBase2D(TwoDimBaseType auto const& A);

   // Assignments.
   STRICT_CONSTEXPR ArrayBase2D& operator=(value_type x);
   STRICT_CONSTEXPR ArrayBase2D& operator=(use::List2D<T> list);
   STRICT_CONSTEXPR ArrayBase2D& operator=(const ArrayBase2D& A);
   STRICT_CONSTEXPR ArrayBase2D& operator=(ArrayBase2D&& A) noexcept;
   STRICT_CONSTEXPR ArrayBase2D& operator=(TwoDimBaseType auto const& A);

   STRICT_CONSTEXPR ~ArrayBase2D() = default;

   STRICT_CONSTEXPR void swap(ArrayBase2D& A) noexcept;
   STRICT_CONSTEXPR void swap(ArrayBase2D&& A) noexcept;

   STRICT_CONSTEXPR auto& resize(ImplicitInt m, ImplicitInt n);
   STRICT_CONSTEXPR auto& resize_forget(ImplicitInt m, ImplicitInt n);

   STRICT_CONSTEXPR auto& resize_and_assign(TwoDimBaseType auto const& A);

   // Optimized implementation.
   STRICT_CONSTEXPR auto& resize_and_assign(GenArray2D<ArrayBase2D>&& A);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR auto& remove_rows(ImplicitInt row_pos, ImplicitInt count);
   STRICT_CONSTEXPR auto& remove_rows(Pos row_pos, Count count);

   STRICT_CONSTEXPR auto& remove_cols(ImplicitInt col_pos, ImplicitInt count);
   STRICT_CONSTEXPR auto& remove_cols(Pos col_pos, Count count);

   STRICT_CONSTEXPR auto& remove_row(ImplicitInt row_pos);
   STRICT_CONSTEXPR auto& remove_row(Pos row_pos);
   STRICT_CONSTEXPR auto& remove_row(Last lst);

   STRICT_CONSTEXPR auto& remove_col(ImplicitInt col_pos);
   STRICT_CONSTEXPR auto& remove_col(Pos col_pos);
   STRICT_CONSTEXPR auto& remove_col(Last lst);

   STRICT_CONSTEXPR auto& remove_rows_front(ImplicitInt count);
   STRICT_CONSTEXPR auto& remove_rows_front(Count count);
   STRICT_CONSTEXPR auto& remove_rows_back(ImplicitInt count);
   STRICT_CONSTEXPR auto& remove_rows_back(Count count);

   STRICT_CONSTEXPR auto& remove_cols_front(ImplicitInt count);
   STRICT_CONSTEXPR auto& remove_cols_front(Count count);
   STRICT_CONSTEXPR auto& remove_cols_back(ImplicitInt count);
   STRICT_CONSTEXPR auto& remove_cols_back(Count count);

   STRICT_CONSTEXPR auto& remove_rows(const std::vector<ImplicitInt>& indexes);
   STRICT_CONSTEXPR auto& remove_cols(const std::vector<ImplicitInt>& indexes);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR auto& insert_rows(ImplicitInt row_pos, TwoDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_rows(Pos row_pos, TwoDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_rows_front(TwoDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_rows_back(TwoDimBaseType auto const& A);

   STRICT_CONSTEXPR auto& insert_cols(ImplicitInt col_pos, TwoDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_cols(Pos col_pos, TwoDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_cols_front(TwoDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_cols_back(TwoDimBaseType auto const& A);

   STRICT_CONSTEXPR auto& insert_row(ImplicitInt row_pos, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_row(Pos row_pos, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_row_front(OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_row_back(OneDimBaseType auto const& A);

   STRICT_CONSTEXPR auto& insert_col(ImplicitInt col_pos, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_col(Pos col_pos, OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_col_front(OneDimBaseType auto const& A);
   STRICT_CONSTEXPR auto& insert_col_back(OneDimBaseType auto const& A);

   ////////////////////////////////////////////////////////////////////////////////////////////////////
   STRICT_CONSTEXPR_INLINE index_t rows() const;
   STRICT_CONSTEXPR_INLINE index_t cols() const;
   STRICT_CONSTEXPR_INLINE index_t size() const;

   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i) const;
   STRICT_NODISCARD_CONSTEXPR_INLINE value_type& un(ImplicitInt i, ImplicitInt j);
   STRICT_NODISCARD_CONSTEXPR_INLINE const value_type& un(ImplicitInt i, ImplicitInt j) const;

   STRICT_NODISCARD_CONSTEXPR value_type* data();
   STRICT_NODISCARD_CONSTEXPR const value_type* data() const;

   // Converting to built-in types requires reinterpret_cast,
   // which cannot be done at compile time(not constexpr).
   STRICT_NODISCARD builtin_type* blas_data()
      requires CompatibleBuiltin<T>;
   STRICT_NODISCARD const builtin_type* blas_data() const
      requires CompatibleBuiltin<T>;

private:
   FixedArrayBase1D<long int, 2> dims_;
   ArrayBase1D<T, AF> data1D_;
};


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase2D<T, AF>::ArrayBase2D(ImplicitInt m, ImplicitInt n)
    : dims_{m.get(), n.get()},
      data1D_(m.get() * n.get()) {
   ASSERT_STRICT_DEBUG(m.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(n.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(semi_valid_row_col_sizes(m.get(), n.get()));
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase2D<T, AF>::ArrayBase2D(Rows m, Cols n)
    : ArrayBase2D(m.get(), n.get()) {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase2D<T, AF>::ArrayBase2D(ImplicitInt m, ImplicitInt n,
                                                           value_type x)
    : ArrayBase2D(m, n) {
   data1D_ = x;
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase2D<T, AF>::ArrayBase2D(Rows m, Cols n, Value<T> x)
    : ArrayBase2D(m.get(), n.get(), x.get()) {
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase2D<T, AF>::ArrayBase2D(use::List2D<T> list) : ArrayBase2D{} {
   ASSERT_STRICT_DEBUG(valid_list2D(list));
   auto [nrows, ncols] = list2D_row_col_sizes(list);
   this->resize_forget(nrows, ncols);
   copy(list, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR ArrayBase2D<T, AF>::ArrayBase2D(TwoDimBaseType auto const& A)
    : ArrayBase2D(A.rows(), A.cols()) {
   copy(A, *this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase2D<T, AF>& ArrayBase2D<T, AF>::operator=(value_type x) {
   data1D_ = x;
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase2D<T, AF>& ArrayBase2D<T, AF>::operator=(use::List2D<T> list) {
   ArrayBase2D tmp{list};
   ASSERT_STRICT_DEBUG(same_size(*this, tmp));
   return *this = std::move(tmp);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase2D<T, AF>& ArrayBase2D<T, AF>::operator=(const ArrayBase2D& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   data1D_ = A.data1D_;
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase2D<T, AF>& ArrayBase2D<T, AF>::operator=(ArrayBase2D&& A) noexcept {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   A.dims_ = 0_sl;
   data1D_ = std::move(A.data1D_);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR ArrayBase2D<T, AF>& ArrayBase2D<T, AF>::operator=(TwoDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(same_size(*this, A));
   copy(A, *this);
   return *this;
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR void ArrayBase2D<T, AF>::swap(ArrayBase2D& A) noexcept {
   dims_ = std::exchange(A.dims_, dims_);
   data1D_.swap(A.data1D_);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR void ArrayBase2D<T, AF>::swap(ArrayBase2D&& A) noexcept {
   this->swap(A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::resize(ImplicitInt m, ImplicitInt n) {
   ASSERT_STRICT_DEBUG(m.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(n.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(semi_valid_row_col_sizes(m.get(), n.get()));

   const auto d0_new = m.get();
   const auto d1_new = n.get();
   if(not(d0_new == dims_.un(0) && d1_new == dims_.un(1))) {
      ArrayBase2D tmp(d0_new, d1_new);
      for(index_t i = 0_sl; i < mins(dims_.un(0), d0_new); ++i) {
         for(index_t j = 0_sl; j < mins(dims_.un(1), d1_new); ++j) {
            tmp.un(i, j) = this->un(i, j);
         }
      }
      this->swap(tmp);
   }

   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::resize_forget(ImplicitInt m, ImplicitInt n) {
   ASSERT_STRICT_DEBUG(m.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(n.get() >= 0_sl);
   ASSERT_STRICT_DEBUG(semi_valid_row_col_sizes(m.get(), n.get()));

   const auto d0_new = m.get();
   const auto d1_new = n.get();
   if(not(d0_new == dims_.un(0) && d1_new == dims_.un(1))) {
      if(d0_new * d1_new != dims_.un(0) * dims_.un(1)) {
         ArrayBase2D tmp(d0_new, d1_new);
         this->swap(tmp);
      } else {
         dims_.un(0) = d0_new;
         dims_.un(1) = d1_new;
      }
   }

   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::resize_and_assign(TwoDimBaseType auto const& A) {
   ArrayBase2D tmp(A);
   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::resize_and_assign(GenArray2D<ArrayBase2D>&& A) {
   this->swap(A);
   A.swap(ArrayBase2D{});
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows(ImplicitInt row_pos, ImplicitInt count) {
   ASSERT_STRICT_DEBUG(count.get() > 0_sl);
   ASSERT_STRICT_DEBUG(valid_row(*this, row_pos.get()));
   ASSERT_STRICT_DEBUG(valid_row(*this, row_pos.get() + count.get() - 1_sl));

   index_t new_rows = this->rows() - count.get();
   ArrayBase2D tmp(new_rows, new_rows == 0_sl ? 0_sl : this->cols());
   copy_rows(*this, tmp, row_pos.get());
   copy_rows(*this,
             tmp,
             row_pos.get() + count.get(),
             row_pos.get(),
             this->rows() - row_pos.get() - count.get());
   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows(Pos row_pos, Count count) {
   return this->remove_rows(row_pos.get(), count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols(ImplicitInt col_pos, ImplicitInt count) {
   ASSERT_STRICT_DEBUG(count.get() > 0_sl);
   ASSERT_STRICT_DEBUG(valid_col(*this, col_pos.get()));
   ASSERT_STRICT_DEBUG(valid_col(*this, col_pos.get() + count.get() - 1_sl));

   index_t new_cols = this->cols() - count.get();
   ArrayBase2D tmp(new_cols == 0_sl ? 0_sl : this->rows(), new_cols);
   copy_cols(*this, tmp, col_pos.get());
   copy_cols(*this,
             tmp,
             col_pos.get() + count.get(),
             col_pos.get(),
             this->cols() - col_pos.get() - count.get());
   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols(Pos col_pos, Count count) {
   return this->remove_cols(col_pos.get(), count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_row(ImplicitInt row_pos) {
   return this->remove_rows(row_pos, 1);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_row(Pos row_pos) {
   return this->remove_row(row_pos.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_row([[maybe_unused]] Last lst) {
   return this->remove_row(this->rows() - 1_sl);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_col(ImplicitInt col_pos) {
   return this->remove_cols(col_pos, 1);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_col(Pos col_pos) {
   return this->remove_col(col_pos.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_col([[maybe_unused]] Last lst) {
   return this->remove_col(this->cols() - 1_sl);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows_front(ImplicitInt count) {
   return this->remove_rows(0, count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows_front(Count count) {
   return this->remove_rows_front(count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows_back(ImplicitInt count) {
   return this->remove_rows(this->rows() - count.get(), count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows_back(Count count) {
   return this->remove_rows_back(count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols_front(ImplicitInt count) {
   return this->remove_cols(0, count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols_front(Count count) {
   return this->remove_cols_front(count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols_back(ImplicitInt count) {
   return this->remove_cols(this->cols() - count.get(), count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols_back(Count count) {
   return this->remove_cols_back(count.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_rows(const std::vector<ImplicitInt>& indexes) {
   if(!indexes.empty()) {
      auto ci = complement_index_vector(
          valid_row<RemoveCVRef<decltype(*this)>>, this->rows(), *this, indexes);

      index_t new_rows = to_index_t(ci.size());
      ArrayBase2D tmp(new_rows, new_rows == 0_sl ? 0_sl : this->cols());
      for(index_t i = 0_sl; i < tmp.rows(); ++i) {
         for(index_t j = 0_sl; j < tmp.cols(); ++j) {
            tmp.un(i, j) = (*this).un(ci[to_size_t(i)], j);
         }
      }
      this->swap(tmp);
   }

   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::remove_cols(const std::vector<ImplicitInt>& indexes) {
   if(!indexes.empty()) {
      auto ci = complement_index_vector(
          valid_col<RemoveCVRef<decltype(*this)>>, this->cols(), *this, indexes);

      index_t new_cols = to_index_t(ci.size());
      ArrayBase2D tmp(new_cols == 0_sl ? 0_sl : this->rows(), new_cols);
      for(index_t i = 0_sl; i < tmp.rows(); ++i) {
         for(index_t j = 0_sl; j < tmp.cols(); ++j) {
            tmp.un(i, j) = (*this).un(i, ci[to_size_t(j)]);
         }
      }
      this->swap(tmp);
   }

   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_rows(ImplicitInt row_pos,
                                                       TwoDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(row_pos.get() >= 0_sl && row_pos.get() <= this->rows());
   if(this->rows() == 0_sl && this->cols() == 0_sl) {
      return this->resize_and_assign(A);
   }

   ASSERT_STRICT_DEBUG(A.cols() == this->cols());
   ArrayBase2D tmp(this->rows() + A.rows(), this->cols());

   copy_rows(*this, tmp, row_pos.get());
   copy_rows(A, tmp, 0_sl, row_pos.get(), A.rows());
   copy_rows(*this, tmp, row_pos.get(), row_pos.get() + A.rows(), this->rows() - row_pos.get());
   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_rows(Pos row_pos, TwoDimBaseType auto const& A) {
   return this->insert_rows(row_pos.get(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_rows_front(TwoDimBaseType auto const& A) {
   return this->insert_rows(0, A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_rows_back(TwoDimBaseType auto const& A) {
   return this->insert_rows(this->rows(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_cols(ImplicitInt col_pos,
                                                       TwoDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(col_pos.get() >= 0_sl && col_pos.get() <= this->cols());
   if(this->rows() == 0_sl && this->cols() == 0_sl) {
      return this->resize_and_assign(A);
   }

   ASSERT_STRICT_DEBUG(A.rows() == this->rows());
   ArrayBase2D tmp(this->rows(), this->cols() + A.cols());

   copy_cols(*this, tmp, col_pos.get());
   copy_cols(A, tmp, 0_sl, col_pos.get(), A.cols());
   copy_cols(*this, tmp, col_pos.get(), col_pos.get() + A.cols(), this->cols() - col_pos.get());
   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_cols(Pos col_pos, TwoDimBaseType auto const& A) {
   return this->insert_cols(col_pos.get(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_cols_front(TwoDimBaseType auto const& A) {
   return this->insert_cols(0, A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_cols_back(TwoDimBaseType auto const& A) {
   return this->insert_cols(this->cols(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_row(ImplicitInt row_pos,
                                                      OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(row_pos.get() >= 0_sl && row_pos.get() <= this->rows());
   // Ensure A is not empty to satisfy semi_valid_row_col_sizes.
   if(this->rows() == 0_sl && this->cols() == 0_sl && A.size() != 0_sl) {
      this->resize(1, A.size());
      copyn(A, *this, A.size());
      return static_cast<GenArray2D<ArrayBase2D>&>(*this);
   }

   ASSERT_STRICT_DEBUG(A.size() == this->cols());
   ArrayBase2D tmp(this->rows() + 1_sl, this->cols());

   for(index_t i = 0_sl, count = 0_sl; i < tmp.rows(); ++i) {
      if(i != row_pos.get()) {
         for(index_t j = 0_sl; j < A.size(); ++j) {
            tmp.un(i, j) = (*this).un(count, j);
         }
         ++count;
      } else {
         for(index_t j = 0_sl; j < A.size(); ++j) {
            tmp.un(i, j) = A.un(j);
         }
      }
   }

   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_row(Pos row_pos, OneDimBaseType auto const& A) {
   return this->insert_row(row_pos.get(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_row_front(OneDimBaseType auto const& A) {
   return this->insert_row(0, A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_row_back(OneDimBaseType auto const& A) {
   return this->insert_row(this->rows(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_col(ImplicitInt col_pos,
                                                      OneDimBaseType auto const& A) {
   ASSERT_STRICT_DEBUG(col_pos.get() >= 0_sl && col_pos.get() <= this->cols());
   // Ensure A is not empty to satisfy semi_valid_row_col_sizes.
   if(this->rows() == 0_sl && this->cols() == 0_sl && A.size() != 0_sl) {
      this->resize(A.size(), 1);
      copyn(A, *this, A.size());
      return static_cast<GenArray2D<ArrayBase2D>&>(*this);
   }

   ASSERT_STRICT_DEBUG(A.size() == this->rows());
   ArrayBase2D tmp(this->rows(), this->cols() + 1_sl);

   for(index_t i = 0_sl; i < A.size(); ++i) {
      for(index_t j = 0_sl, count = 0_sl; j < tmp.cols(); ++j) {
         if(j != col_pos.get()) {
            tmp.un(i, j) = (*this).un(i, count++);
         } else {
            tmp.un(i, j) = A.un(i);
         }
      }
   }

   this->swap(tmp);
   return static_cast<GenArray2D<ArrayBase2D>&>(*this);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_col(Pos col_pos, OneDimBaseType auto const& A) {
   return this->insert_col(col_pos.get(), A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_col_front(OneDimBaseType auto const& A) {
   return this->insert_col(0, A);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR auto& ArrayBase2D<T, AF>::insert_col_back(OneDimBaseType auto const& A) {
   return this->insert_col(this->cols(), A);
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t ArrayBase2D<T, AF>::rows() const {
   return dims_.un(0);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t ArrayBase2D<T, AF>::cols() const {
   return dims_.un(1);
}


template <Builtin T, AlignmentFlag AF>
STRICT_CONSTEXPR_INLINE index_t ArrayBase2D<T, AF>::size() const {
   return data1D_.size();
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto ArrayBase2D<T, AF>::un(ImplicitInt i) -> value_type& {
   return data1D_.un(i);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto ArrayBase2D<T, AF>::un(ImplicitInt i) const
    -> const value_type& {
   return data1D_.un(i);
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto ArrayBase2D<T, AF>::un(ImplicitInt i, ImplicitInt j)
    -> value_type& {
   return data1D_.un(i.get() * dims_.un(1) + j.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR_INLINE auto ArrayBase2D<T, AF>::un(ImplicitInt i, ImplicitInt j) const
    -> const value_type& {
   return data1D_.un(i.get() * dims_.un(1) + j.get());
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto ArrayBase2D<T, AF>::data() -> value_type* {
   return data1D_.data();
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD_CONSTEXPR auto ArrayBase2D<T, AF>::data() const -> const value_type* {
   return data1D_.data();
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD auto ArrayBase2D<T, AF>::blas_data() -> builtin_type*
   requires CompatibleBuiltin<T>
{
   return data1D_.blas_data();
}


template <Builtin T, AlignmentFlag AF>
STRICT_NODISCARD auto ArrayBase2D<T, AF>::blas_data() const -> const builtin_type*
   requires CompatibleBuiltin<T>
{
   return data1D_.blas_data();
}


}}  // namespace spp::detail
