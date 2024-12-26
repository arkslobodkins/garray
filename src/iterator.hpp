// Arkadijs Slobodkins, 2023


#pragma once


#include <iterator>
#include <type_traits>
#include <utility>

#include "ArrayCommon/array_traits.hpp"
#include "ArrayCommon/valid.hpp"
#include "StrictCommon/strict_common.hpp"


namespace spp {


namespace detail {


template <NonConstBaseType Base>
class IndexFlag {
public:
   STRICT_CONSTEXPR explicit IndexFlag(Base* A) : A_{A} {
   }

   // Non-const iterator only needs non-const indexing.
   // Returns a reference because non-const iterators only
   // operate on NonConstBaseType bases, which return lvalues.
   STRICT_CONSTEXPR ValueTypeOf<Base>& operator()(ImplicitInt i) const {
      return (*A_).un(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).size();
   }

private:
   Base* A_;
};


template <TwoDimNonConstBaseType Base>
class RowFlag {
public:
   STRICT_CONSTEXPR explicit RowFlag(Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).row(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).rows();
   }

private:
   Base* A_;
};


template <TwoDimNonConstBaseType Base>
class ColFlag {
public:
   STRICT_CONSTEXPR explicit ColFlag(Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).col(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).cols();
   }

private:
   Base* A_;
};


template <BaseType Base>
class ConstIndexFlag {
public:
   STRICT_CONSTEXPR explicit ConstIndexFlag(const Base* A) : A_{A} {
   }

   // Const iterator only needs const indexing(A_ is a pointer to const).
   // May return either by const reference or by value, as it the case
   // with expression templates.
   STRICT_CONSTEXPR decltype(auto) operator()(ImplicitInt i) const {
      return (*A_).un(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).size();
   }

private:
   const Base* A_;
};


template <TwoDimBaseType Base>
class ConstRowFlag {
public:
   STRICT_CONSTEXPR explicit ConstRowFlag(const Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).row(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).rows();
   }

private:
   const Base* A_;
};


template <TwoDimBaseType Base>
class ConstColFlag {
public:
   STRICT_CONSTEXPR explicit ConstColFlag(const Base* A) : A_{A} {
   }

   STRICT_CONSTEXPR auto operator()(ImplicitInt i) const {
      return (*A_).col(i);
   }

   STRICT_CONSTEXPR index_t size() const {
      return (*A_).cols();
   }

private:
   const Base* A_;
};


class LinearIterator {};


// Second condition is added so that it works for reverse iterators as well.
template <typename T> concept LinearIteratorType
    = BaseOf<LinearIterator, T> || BaseOf<LinearIterator, typename T::iterator_type>;


template <typename T, typename Flag>
class IteratorTag {};


// IteratorTag that uses .un() for indexing is a linear iterator.
// They can be passed to constructors of one-dimensional Arrays.
template <typename T>
class IteratorTag<T, IndexFlag<T>> : private LinearIterator {};


template <typename T>
class IteratorTag<T, ConstIndexFlag<T>> : private LinearIterator {};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base, typename F = IndexFlag<Base>>
class STRICT_NODISCARD Iterator : private IteratorTag<Base, F> {
public:
   // Ensure that constant types are passed to ConstIterator
   // even though Iterator would function properly for iterating over rows and columns,
   // it would not work for one-dimensional indexing and potentially cause other issues.
   static_assert(!IsConst<Base>);

   // difference_type is long int instead of index_t so that std::random_access_iterator
   // concept is true and for better compatibility with algorithms from the standard library.
   // Similarly, operators + += - -=, and [] have difference_type as a parameter instead of
   // ImplicitInt for similar reasons.
   using difference_type = long int;
   using value_type = RemoveRef<decltype(std::declval<F>()(0))>;
   using pointer = value_type*;
   using reference = value_type&;
   // Contiguous if Base owns the data and iterates over indexes(not rows and not columns).
   using iterator_category
       = std::conditional_t<ReferenceStorable<Base> && LinearIteratorType<IteratorTag<Base, F>>,
                            std::contiguous_iterator_tag, std::random_access_iterator_tag>;

public:
   STRICT_NODISCARD_CONSTEXPR explicit Iterator();
   STRICT_NODISCARD_CONSTEXPR explicit Iterator(Base& A, index_t pos);

   STRICT_CONSTEXPR Iterator& operator++();
   STRICT_CONSTEXPR Iterator& operator--();
   STRICT_NODISCARD_CONSTEXPR Iterator operator++(int);
   STRICT_NODISCARD_CONSTEXPR Iterator operator--(int);

   STRICT_CONSTEXPR Iterator& operator+=(difference_type incr);
   STRICT_CONSTEXPR Iterator& operator-=(difference_type incr);

   STRICT_NODISCARD_CONSTEXPR Iterator operator+(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR Iterator operator-(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR difference_type operator-(const Iterator& it) const;

   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator[](difference_type n) const;
   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator*() const;
   // Only works for iterators over indexes, since rows and
   // columns are returned by value and appear as rvalues.
   STRICT_NODISCARD_CONSTEXPR pointer operator->() const;

   // bool is used rather than StrictBool so that std::random_access_iterator concept is true
   // and for better compatibility with algorithms from the standard library.
   STRICT_NODISCARD_CONSTEXPR bool operator==(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator!=(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<=(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>(const Iterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>=(const Iterator& it) const;

   STRICT_NODISCARD_CONSTEXPR StrictBool points_somewhere() const;

private:
   Base* A_ptr_;
   index_t pos_;
   F f_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> operator+(long int incr, const Iterator<Base, F>& it) {
   ASSERT_STRICT_DEBUG(it.points_somewhere());
   return it + incr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F>::Iterator()
    : A_ptr_{nullptr},
      pos_{-1_sl},
      f_{nullptr} {
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F>::Iterator(Base& A, index_t pos)
    : A_ptr_{&A},
      pos_{pos},
      f_{&A} {
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator++() {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ++pos_;
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator--() {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   --pos_;
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator++(int) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto old = *this;
   ++*this;
   return old;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator--(int) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto old = *this;
   --*this;
   return old;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator+=(difference_type incr) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   pos_ += Strict{incr};
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_CONSTEXPR Iterator<Base, F>& Iterator<Base, F>::operator-=(difference_type incr) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   pos_ -= Strict{incr};
   return *this;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator+(
    difference_type incr) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto new_it = *this;
   new_it += incr;
   return new_it;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR Iterator<Base, F> Iterator<Base, F>::operator-(
    difference_type incr) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto Iterator<Base, F>::operator-(const Iterator& it) const
    -> difference_type {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return (pos_ - it.pos_).val();
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) Iterator<Base, F>::operator[](difference_type n) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ + Strict{n} > -1_sl && pos_ + Strict{n} < f_.size());
   return f_(pos_ + Strict{n});
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) Iterator<Base, F>::operator*() const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return f_(pos_);
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto Iterator<Base, F>::operator->() const -> pointer {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return &f_(pos_);
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator==(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ == it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator!=(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ != it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator<(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ < it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator<=(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ <= it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator>(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ > it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool Iterator<Base, F>::operator>=(const Iterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ >= it.pos_};
}


template <NonConstBaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR StrictBool Iterator<Base, F>::points_somewhere() const {
   return StrictBool{A_ptr_ != nullptr};
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F = ConstIndexFlag<Base>>
class STRICT_NODISCARD ConstIterator : private IteratorTag<Base, F> {
public:
   // difference_type is long int instead of index_t so that std::random_access_iterator
   // concept is true and for better compatibility with algorithms from the standard library.
   // Similarly, operators + += - -=, and [] have difference_type as a parameter instead of
   // ImplicitInt for similar reasons.
   using difference_type = long int;
   using value_type = RemoveRef<decltype(std::declval<F>()(0))>;
   using pointer = value_type*;
   using const_pointer = const value_type*;
   using reference = value_type&;
   using const_reference = const value_type&;
   // Contiguous if Base owns the data and iterates over indexes(not rows and not columns).
   using iterator_category
       = std::conditional_t<ReferenceStorable<Base> && LinearIteratorType<IteratorTag<Base, F>>,
                            std::contiguous_iterator_tag, std::random_access_iterator_tag>;

   STRICT_NODISCARD_CONSTEXPR explicit ConstIterator();
   STRICT_NODISCARD_CONSTEXPR explicit ConstIterator(const Base& A, index_t pos);

   STRICT_CONSTEXPR ConstIterator& operator++();
   STRICT_CONSTEXPR ConstIterator& operator--();
   STRICT_NODISCARD_CONSTEXPR ConstIterator operator++(int);
   STRICT_NODISCARD_CONSTEXPR ConstIterator operator--(int);

   STRICT_CONSTEXPR ConstIterator& operator+=(difference_type incr);
   STRICT_CONSTEXPR ConstIterator& operator-=(difference_type incr);

   STRICT_NODISCARD_CONSTEXPR ConstIterator operator+(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR ConstIterator operator-(difference_type incr) const;
   STRICT_NODISCARD_CONSTEXPR difference_type operator-(const ConstIterator& it) const;

   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator[](difference_type n) const;
   STRICT_NODISCARD_CONSTEXPR decltype(auto) operator*() const;
   // Only works for iterators over indexes, since rows and
   // columns are returned by value and appear as rvalues.
   STRICT_NODISCARD_CONSTEXPR const_pointer operator->() const;

   // bool is used rather than StrictBool so that std::random_access_iterator concept is true
   // and for better compatibility with algorithms from the standard library.
   STRICT_NODISCARD_CONSTEXPR bool operator==(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator!=(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator<=(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>(const ConstIterator& it) const;
   STRICT_NODISCARD_CONSTEXPR bool operator>=(const ConstIterator& it) const;

   STRICT_NODISCARD_CONSTEXPR StrictBool points_somewhere() const;

private:
   const Base* A_ptr_;
   index_t pos_;
   F f_;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> operator+(long int incr,
                                                            const ConstIterator<Base, F>& it) {
   ASSERT_STRICT_DEBUG(it.points_somewhere());
   return it + incr;
}


////////////////////////////////////////////////////////////////////////////////////////////////////
template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F>::ConstIterator()
    : A_ptr_{nullptr},
      pos_{-1_sl},
      f_{nullptr} {
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F>::ConstIterator(const Base& A, index_t pos)
    : A_ptr_{&A},
      pos_{pos},
      f_{&A} {
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator++() {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ++pos_;
   return *this;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator--() {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   --pos_;
   return *this;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator++(int) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto old = *this;
   ++*this;
   return old;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator--(int) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto old = *this;
   --*this;
   return old;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator+=(difference_type incr) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   pos_ += Strict{incr};
   return *this;
}


template <BaseType Base, typename F>
STRICT_CONSTEXPR ConstIterator<Base, F>& ConstIterator<Base, F>::operator-=(difference_type incr) {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   pos_ -= Strict{incr};
   return *this;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator+(
    difference_type incr) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto new_it = *this;
   new_it += incr;
   return new_it;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR ConstIterator<Base, F> ConstIterator<Base, F>::operator-(
    difference_type incr) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   auto new_it = *this;
   new_it -= incr;
   return new_it;
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto ConstIterator<Base, F>::operator-(const ConstIterator& it) const
    -> difference_type {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return (pos_ - it.pos_).val();
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) ConstIterator<Base, F>::operator[](
    difference_type n) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ + Strict{n} > -1_sl && pos_ + Strict{n} < f_.size());
   return f_(pos_ + Strict{n});
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR decltype(auto) ConstIterator<Base, F>::operator*() const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return f_(pos_);
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR auto ConstIterator<Base, F>::operator->() const -> const_pointer {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_RANGE_DEBUG(pos_ > -1_sl && pos_ < f_.size());
   return &(f_(pos_));
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator==(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ == it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator!=(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ != it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator<(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ < it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator<=(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ <= it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator>(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ > it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR bool ConstIterator<Base, F>::operator>=(const ConstIterator& it) const {
   ASSERT_STRICT_DEBUG(this->points_somewhere());
   ASSERT_STRICT_DEBUG(A_ptr_ == it.A_ptr_);
   return bool{pos_ >= it.pos_};
}


template <BaseType Base, typename F>
STRICT_NODISCARD_CONSTEXPR StrictBool ConstIterator<Base, F>::points_somewhere() const {
   return StrictBool{A_ptr_ != nullptr};
}


class STRICT_NODISCARD PlaneIterator {
public:
   STRICT_NODISCARD_CONSTEXPR explicit PlaneIterator() = default;
   STRICT_NODISCARD_CONSTEXPR PlaneIterator(const PlaneIterator&) = default;

   STRICT_NODISCARD_CONSTEXPR explicit PlaneIterator(index_t i, index_t j, index_t rows,
                                                     index_t cols)
       : i_{i},
         j_{j},
         rows_{rows},
         cols_{cols} {
      ASSERT_STRICT_DEBUG(i_ >= 0_sl);
      ASSERT_STRICT_DEBUG(j_ >= 0_sl);
      ASSERT_STRICT_DEBUG(rows_ >= i_);
      ASSERT_STRICT_DEBUG(cols_ >= j_);
   }

   STRICT_CONSTEXPR PlaneIterator& operator=(const PlaneIterator& it) {
      ASSERT_STRICT_DEBUG(rows_ == it.rows_);
      ASSERT_STRICT_DEBUG(cols_ == it.cols_);
      i_ = it.i_;
      j_ = it.j_;
      return *this;
   }

   STRICT_CONSTEXPR PlaneIterator& operator++() {
      // Iterating past the end iterator for PlaneIterator is not allowed.
      ASSERT_STRICT_DEBUG(i_ < rows_);
      ASSERT_STRICT_DEBUG(j_ < cols_);

      if((i_ == rows_ - 1_sl) && (j_ == cols_ - 1_sl)) {
         ++i_;
         ++j_;
         return *this;
      }

      // Branch removing transformations.
      i_ = i_ + strict_cast<long>(j_ + 1_sl == cols_);
      // Assume j_ starts at 0, otherwise another variable would be needed to store j_start.
      j_ = (j_ + 1_sl) % cols_;

      return *this;
   }

   STRICT_NODISCARD_CONSTEXPR PlaneIterator operator++(int) {
      auto old = *this;
      ++*this;
      return old;
   }

   STRICT_NODISCARD_CONSTEXPR StrictBool operator==(const PlaneIterator& it) const {
      ASSERT_STRICT_DEBUG(rows_ == it.rows_);
      ASSERT_STRICT_DEBUG(cols_ == it.cols_);
      return (i_ == it.i_) && (j_ == it.j_);
   }

   STRICT_NODISCARD_CONSTEXPR StrictBool operator!=(const PlaneIterator& it) const {
      ASSERT_STRICT_DEBUG(rows_ == it.rows_);
      ASSERT_STRICT_DEBUG(cols_ == it.cols_);
      return !(*this == it);
   }

   STRICT_NODISCARD_CONSTEXPR std::pair<index_t, index_t> operator*() const {
      ASSERT_STRICT_DEBUG(i_ < rows_);
      ASSERT_STRICT_DEBUG(j_ < cols_);
      return {i_, j_};
   }

private:
   index_t i_{}, j_{};
   const index_t rows_{}, cols_{};
};


}  // namespace detail


// Must specify std::is_lvalue_reference_v so that BaseRef template argument cannot
// be instantiated by value. It would lead to incorrect semantics of the class and
// arrays that own data would be stored by copy.
template <typename BaseRef>
   requires TwoDimBaseType<RemoveRef<BaseRef>> && std::is_lvalue_reference_v<BaseRef>
class STRICT_NODISCARD RowIt {
   using base_type = RemoveCVRef<BaseRef>;

public:
   STRICT_NODISCARD_CONSTEXPR explicit RowIt(auto&& A)
      requires(!std::is_same_v<RemoveCVRef<decltype(A)>, RowIt>)
       : A_{std::forward<decltype(A)>(A)} {
      static_assert(std::is_lvalue_reference_v<decltype(A)>);
      // avoid implicit conversions from slice and expression templates to constant arrays.
      static_assert(std::is_same_v<BaseRef, decltype(A)>);
   }

   STRICT_NODISCARD_CONSTEXPR RowIt(const RowIt&) = default;
   STRICT_CONSTEXPR RowIt& operator=(const RowIt&) = delete;

   STRICT_NODISCARD_CONSTEXPR auto begin() {
      using namespace detail;
      if constexpr(NonConstBaseType<RemoveRef<BaseRef>>) {
         return Iterator<base_type, RowFlag<base_type>>{A_, 0_sl};
      } else {
         return ConstIterator<base_type, ConstRowFlag<base_type>>{A_, 0_sl};
      }
   }

   // Iterator member functions are not ref-qualified since constructor ensures that A is an lvalue.
   // Calling them with rvalue RowIt objects is allowed and even encouraged.
   STRICT_NODISCARD_CONSTEXPR auto begin() const {
      using namespace detail;
      return ConstIterator<base_type, ConstRowFlag<base_type>>{A_, 0_sl};
   }

   STRICT_NODISCARD_CONSTEXPR auto end() {
      using namespace detail;
      if constexpr(NonConstBaseType<RemoveRef<BaseRef>>) {
         return Iterator<base_type, RowFlag<base_type>>{A_, A_.rows()};
      } else {
         return ConstIterator<base_type, ConstRowFlag<base_type>>{A_, A_.rows()};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto end() const {
      using namespace detail;
      return ConstIterator<base_type, ConstRowFlag<base_type>>{A_, A_.rows()};
   }

   STRICT_NODISCARD_CONSTEXPR auto cbegin() const {
      return this->begin();
   }

   STRICT_NODISCARD_CONSTEXPR auto cend() const {
      return this->end();
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() const {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() const {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crbegin() const {
      return std::reverse_iterator{this->rbegin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crend() const {
      return std::reverse_iterator{this->rend()};
   }

private:
   BaseRef A_;
};


template <typename BaseRef>
RowIt(BaseRef&&) -> RowIt<BaseRef>;


// Must specify std::is_lvalue_reference_v so that BaseRef template argument cannot
// be instantiated by value. It would lead to incorrect semantics of the class and
// arrays that own data would be stored by copy.
template <typename BaseRef>
   requires TwoDimBaseType<RemoveRef<BaseRef>> && std::is_lvalue_reference_v<BaseRef>
class STRICT_NODISCARD ColIt {
   using base_type = RemoveCVRef<BaseRef>;

public:
   STRICT_NODISCARD_CONSTEXPR explicit ColIt(auto&& A)
      requires(!std::is_same_v<RemoveCVRef<decltype(A)>, ColIt>)
       : A_{std::forward<decltype(A)>(A)} {
      static_assert(std::is_lvalue_reference_v<decltype(A)>);
      // avoid implicit conversions from slice and expression templates to constant arrays.
      static_assert(std::is_same_v<BaseRef, decltype(A)>);
   }

   STRICT_NODISCARD_CONSTEXPR ColIt(const ColIt&) = default;
   STRICT_CONSTEXPR ColIt& operator=(const ColIt&) = delete;

   // Iterator member functions are not ref-qualified since constructor ensures that A is an lvalue.
   // Calling them with rvalue ColIt objects is allowed and even encouraged.
   STRICT_NODISCARD_CONSTEXPR auto begin() {
      using namespace detail;
      if constexpr(NonConstBaseType<RemoveRef<BaseRef>>) {
         return Iterator<base_type, ColFlag<base_type>>{A_, 0_sl};
      } else {
         return ConstIterator<base_type, ConstColFlag<base_type>>{A_, 0_sl};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto begin() const {
      using namespace detail;
      return ConstIterator<base_type, ConstColFlag<base_type>>{A_, 0_sl};
   }

   STRICT_NODISCARD_CONSTEXPR auto end() {
      using namespace detail;
      if constexpr(NonConstBaseType<RemoveRef<BaseRef>>) {
         return Iterator<base_type, ColFlag<base_type>>{A_, A_.cols()};
      } else {
         return ConstIterator<base_type, ConstColFlag<base_type>>{A_, A_.cols()};
      }
   }

   STRICT_NODISCARD_CONSTEXPR auto end() const {
      using namespace detail;
      return ConstIterator<base_type, ConstColFlag<base_type>>{A_, A_.cols()};
   }

   STRICT_NODISCARD_CONSTEXPR auto cbegin() const {
      return this->begin();
   }

   STRICT_NODISCARD_CONSTEXPR auto cend() const {
      return this->end();
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rbegin() const {
      return std::reverse_iterator{this->end()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto rend() const {
      return std::reverse_iterator{this->begin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crbegin() const {
      return std::reverse_iterator{this->rbegin()};
   }

   STRICT_NODISCARD_CONSTEXPR auto crend() const {
      return std::reverse_iterator{this->rend()};
   }

private:
   BaseRef A_;
};


template <typename BaseRef>
ColIt(BaseRef&&) -> ColIt<BaseRef>;


class STRICT_NODISCARD IndexRange2D {
public:
   STRICT_NODISCARD_CONSTEXPR explicit IndexRange2D() = default;
   STRICT_NODISCARD_CONSTEXPR IndexRange2D(const IndexRange2D&) = default;

   STRICT_NODISCARD_CONSTEXPR explicit IndexRange2D(ImplicitInt rows, ImplicitInt cols)
       : rows_{rows.get()},
         cols_{cols.get()} {
      ASSERT_STRICT_DEBUG(rows_ > -1_sl);
      ASSERT_STRICT_DEBUG(cols_ > -1_sl);
      ASSERT_STRICT_DEBUG(detail::semi_valid_row_col_sizes(rows_, cols_));
   }

   STRICT_NODISCARD_CONSTEXPR explicit IndexRange2D(TwoDimBaseType auto const& A)
       : IndexRange2D{A.rows(), A.cols()} {
   }

   STRICT_CONSTEXPR IndexRange2D& operator=(const IndexRange2D&) = delete;

   STRICT_CONSTEXPR auto begin() const {
      return detail::PlaneIterator{0_sl, 0_sl, rows_, cols_};
   }

   STRICT_CONSTEXPR auto end() const {
      return detail::PlaneIterator{rows_, cols_, rows_, cols_};
   }

private:
   index_t rows_{}, cols_{};
};


}  // namespace spp
