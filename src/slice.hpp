// Arkadijs Slobodkins, 2023


#pragma once


#include <utility>
#include <vector>

#include "ArrayCommon/array_common.hpp"
#include "StrictCommon/strict_common.hpp"


namespace spp {


class STRICT_NODISCARD seqN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit seqN(ImplicitInt start, ImplicitInt size,
                                            ImplicitInt stride = 1)
       : start_{start.get()},
         size_{size.get()},
         stride_{stride.get()} {
      ASSERT_STRICT_DEBUG(start_ > -1_sl);
      ASSERT_STRICT_DEBUG(size_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit seqN(First first, Size size, Stride stride = Stride{1})
       : seqN{first.get(), size.get(), stride.get()} {
   }

   STRICT_CONSTEXPR index_t start() const {
      return start_;
   }

   STRICT_CONSTEXPR index_t size() const {
      return size_;
   }

   STRICT_CONSTEXPR index_t stride() const {
      return stride_;
   }

   // Note: stride is already valid, start and size are "partially" valid.
   template <BaseType BaseT>
   STRICT_CONSTEXPR StrictBool valid(const BaseT& A) const {
      if(size_ != 0_sl) {
         return detail::valid_index(A, start_)
             && detail::valid_index(A, start_ + stride_ * (size_ - 1_sl));
      }
      return true_sb;
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_CONSTEXPR StrictBool valid_first(const TwoDimBaseT& A) const {
      if(size_ != 0_sl) {
         return detail::valid_row(A, start_)
             && detail::valid_row(A, start_ + stride_ * (size_ - 1_sl));
      }
      return true_sb;
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_CONSTEXPR StrictBool valid_second(const TwoDimBaseT& A) const {
      if(size_ != 0_sl) {
         return detail::valid_col(A, start_)
             && detail::valid_col(A, start_ + stride_ * (size_ - 1_sl));
      }
      return true_sb;
   }

private:
   index_t start_;
   index_t size_;
   index_t stride_;
};


// Unlike seqN, seq cannot be empty.
class STRICT_NODISCARD seq {
public:
   STRICT_NODISCARD_CONSTEXPR explicit seq(ImplicitInt first, ImplicitInt last,
                                           ImplicitInt stride = 1)
       : first_{first.get()},
         last_{last.get()},
         stride_{stride.get()} {
      ASSERT_STRICT_DEBUG(first_ > -1_sl);
      ASSERT_STRICT_DEBUG(last_ > -1_sl);
      ASSERT_STRICT_DEBUG(((stride_ > 0_sl) && (first_ <= last_))
                          || ((stride_ < 0_sl) && (first_ >= last_))
                          || ((stride_ == 0_sl) && (first_ == last_)));
   }

   STRICT_NODISCARD_CONSTEXPR explicit seq(First first, Endmost last, Stride stride = Stride{1})
       : seq{first.get(), last.get(), stride.get()} {
   }

   STRICT_CONSTEXPR seqN to_slice() const {
      if(stride_ != 0_sl) {
         return seqN{first_, (last_ - first_) / stride_ + 1_sl, stride_};
      }
      // If stride is 0, avoid division by 0.
      // In that case, first must be equal to last.
      return seqN{first_, 1_sl, 0_sl};
   }

   STRICT_CONSTEXPR index_t first() const {
      return first_;
   }

   STRICT_CONSTEXPR index_t last() const {
      return last_;
   }

   STRICT_CONSTEXPR index_t stride() const {
      return stride_;
   }

   // Note: stride is already valid, first and last are "partially" valid.
   template <BaseType BaseT>
   STRICT_CONSTEXPR StrictBool valid(const BaseT& A) const {
      return detail::valid_index(A, first_) && detail::valid_index(A, last_);
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_CONSTEXPR StrictBool valid_first(const TwoDimBaseT& A) const {
      return detail::valid_row(A, first_) && detail::valid_row(A, last_);
   }

   template <TwoDimBaseType TwoDimBaseT>
   STRICT_CONSTEXPR StrictBool valid_second(const TwoDimBaseT& A) const {
      return detail::valid_col(A, first_) && detail::valid_col(A, last_);
   }

private:
   index_t first_;
   index_t last_;
   index_t stride_;
};


namespace place {


class STRICT_NODISCARD skipN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit skipN(ImplicitInt stride) : stride_{stride.get()} {
      // Stride cannot be 0, otherwise size of slice cannot be deduced.
      ASSERT_STRICT_DEBUG(stride_ > 0_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit skipN(Stride stride) : skipN{stride.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t stride() const {
      return stride_;
   }

private:
   index_t stride_;
};


class STRICT_NODISCARD firstN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit firstN(ImplicitInt n) : n_{n.get()} {
      ASSERT_STRICT_DEBUG(n_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit firstN(Count count) : firstN{count.get()} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return n_;
   }

private:
   index_t n_;
};


class STRICT_NODISCARD lastN {
public:
   STRICT_NODISCARD_CONSTEXPR explicit lastN(ImplicitInt n) : n_{n.get()} {
      ASSERT_STRICT_DEBUG(n_ > -1_sl);
   }

   STRICT_NODISCARD_CONSTEXPR explicit lastN(Count count) : lastN{ImplicitInt{count.get()}} {
   }

   STRICT_NODISCARD_CONSTEXPR index_t get() const {
      return n_;
   }

private:
   index_t n_;
};


class STRICT_NODISCARD complement {
public:
   STRICT_NODISCARD_CONSTEXPR explicit complement(std::vector<ImplicitInt> v) : v_{std::move(v)} {
   }

   STRICT_NODISCARD_CONSTEXPR const std::vector<ImplicitInt>& get() const {
      return v_;
   }

private:
   std::vector<ImplicitInt> v_;
};


}  // namespace place


namespace detail {


template <typename T> concept LinearSliceType = SameAs<T, seqN> || SameAs<T, seq> || SameAs<T, All>
                                             || SameAs<T, Even> || SameAs<T, Odd>
                                             || SameAs<T, Reverse> || SameAs<T, place::skipN>
                                             || SameAs<T, place::firstN> || SameAs<T, place::lastN>;


template <typename T> concept NonlinearSliceType
    = SameAs<T, std::vector<ImplicitInt>> || SameAs<T, place::complement>;


template <typename T> concept SliceType = LinearSliceType<T> || NonlinearSliceType<T>;


template <typename T>
class SliceWrapper {
public:
   SliceWrapper(T i) = delete;
};


template <>
class SliceWrapper<seqN> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(const seqN& s) : s_{s} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(s_.valid(A));
      return s_;
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(s_.valid_first(A));
      return s_;
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(s_.valid_second(A));
      return s_;
   }

private:
   seqN s_;
};


template <>
class SliceWrapper<seq> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(const seq& s) : s_{s} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(s_.valid(A));
      return s_.to_slice();
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(s_.valid_first(A));
      return s_.to_slice();
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(!A.empty());
      ASSERT_STRICT_DEBUG(s_.valid_second(A));
      return s_.to_slice();
   }

private:
   seq s_;
};


template <>
class SliceWrapper<All> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(All) {
   }

   STRICT_CONSTEXPR_INLINE static auto get(OneDimBaseType auto const& A) {
      return seqN{0, A.size(), 1};
   }

   STRICT_CONSTEXPR_INLINE static auto get_row(TwoDimBaseType auto const& A) {
      return seqN{0, A.rows(), 1};
   }

   STRICT_CONSTEXPR_INLINE static auto get_col(TwoDimBaseType auto const& A) {
      return seqN{0, A.cols(), 1};
   }
};


template <>
class SliceWrapper<Even> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(Even) {
   }

   STRICT_CONSTEXPR_INLINE static auto get(OneDimBaseType auto const& A) {
      return seqN{0, (A.size() + 1_sl) / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE static auto get_row(TwoDimBaseType auto const& A) {
      return seqN{0, (A.rows() + 1_sl) / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE static auto get_col(TwoDimBaseType auto const& A) {
      return seqN{0, (A.cols() + 1_sl) / 2_sl, 2};
   }
};


template <>
class SliceWrapper<Odd> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(Odd) {
   }

   STRICT_CONSTEXPR_INLINE static auto get(OneDimBaseType auto const& A) {
      return seqN{1, A.size() / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE static auto get_row(TwoDimBaseType auto const& A) {
      return seqN{1, A.rows() / 2_sl, 2};
   }

   STRICT_CONSTEXPR_INLINE static auto get_col(TwoDimBaseType auto const& A) {
      return seqN{1, A.cols() / 2_sl, 2};
   }
};


template <>
class SliceWrapper<Reverse> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(Reverse) {
   }

   STRICT_CONSTEXPR_INLINE static auto get(OneDimBaseType auto const& A) {
      return seqN{A.empty() ? 0_sl : A.size() - 1_sl, A.size(), -1};
   }

   STRICT_CONSTEXPR_INLINE static auto get_row(TwoDimBaseType auto const& A) {
      return seqN{A.empty() ? 0_sl : A.rows() - 1_sl, A.rows(), -1};
   }

   STRICT_CONSTEXPR_INLINE static auto get_col(TwoDimBaseType auto const& A) {
      return seqN{A.empty() ? 0_sl : A.cols() - 1_sl, A.cols(), -1};
   }
};


template <>
class SliceWrapper<place::skipN> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(place::skipN x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      return seqN{0, (A.size() + x_.stride() - 1_sl) / x_.stride(), x_.stride()};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      return seqN{0, (A.rows() + x_.stride() - 1_sl) / x_.stride(), x_.stride()};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      return seqN{0, (A.cols() + x_.stride() - 1_sl) / x_.stride(), x_.stride()};
   }

private:
   place::skipN x_;
};


template <>
class SliceWrapper<place::firstN> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(place::firstN x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.size());
      return seqN{0, x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.rows());
      return seqN{0, x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.cols());
      return seqN{0, x_.get(), 1};
   }

private:
   place::firstN x_;
};


template <>
class SliceWrapper<place::lastN> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(place::lastN x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get(OneDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.size());
      return seqN{A.size() - x_.get(), x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_row(TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.rows());
      return seqN{A.rows() - x_.get(), x_.get(), 1};
   }

   STRICT_CONSTEXPR_INLINE auto get_col(TwoDimBaseType auto const& A) const {
      ASSERT_STRICT_DEBUG(x_.get() <= A.cols());
      return seqN{A.cols() - x_.get(), x_.get(), 1};
   }

private:
   place::lastN x_;
};


template <>
class SliceWrapper<std::vector<ImplicitInt>> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(std::vector<ImplicitInt> x) : x_{std::move(x)} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) && {
      return std::move(x_);
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) && {
      return std::move(x_);
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) && {
      return std::move(x_);
   }

private:
   std::vector<ImplicitInt> x_;
};


template <>
class SliceWrapper<use::IndexList> {
public:
   STRICT_CONSTEXPR_INLINE SliceWrapper(use::IndexList x) : x_{x} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) {
      return std::move(x_);
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) {
      return std::move(x_);
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) {
      return std::move(x_);
   }

private:
   std::vector<ImplicitInt> x_;
};


template <>
class SliceWrapper<place::complement> {
public:
   STRICT_CONSTEXPR_INLINE explicit SliceWrapper(place::complement x) : x_{std::move(x)} {
   }

   STRICT_CONSTEXPR_INLINE auto get([[maybe_unused]] OneDimBaseType auto const& A) const {
      return complement_index_vector(valid_index<RemoveCVRef<decltype(A)>>, A.size(), A, x_.get());
   }

   STRICT_CONSTEXPR_INLINE auto get_row([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return complement_index_vector(valid_row<RemoveCVRef<decltype(A)>>, A.rows(), A, x_.get());
   }

   STRICT_CONSTEXPR_INLINE auto get_col([[maybe_unused]] TwoDimBaseType auto const& A) const {
      return complement_index_vector(valid_col<RemoveCVRef<decltype(A)>>, A.cols(), A, x_.get());
   }

private:
   place::complement x_;
};


STRICT_CONSTEXPR_INLINE auto slice_helper(OneDimBaseType auto const& A, auto slice) {
   return SliceWrapper{std::move(slice)}.get(A);
}


STRICT_CONSTEXPR_INLINE auto slice_row_helper(TwoDimBaseType auto const& A, auto row_slice) {
   return SliceWrapper{std::move(row_slice)}.get_row(A);
}


STRICT_CONSTEXPR_INLINE auto slice_col_helper(TwoDimBaseType auto const& A, auto col_slice) {
   return SliceWrapper{std::move(col_slice)}.get_col(A);
}


STRICT_CONSTEXPR_INLINE auto slice_row_col_helper(TwoDimBaseType auto const& A, auto row_slice,
                                                  auto col_slice) {
   return std::pair{SliceWrapper{std::move(row_slice)}.get_row(A),
                    SliceWrapper{std::move(col_slice)}.get_col(A)};
}


template <typename SliceObj>
class SliceArrayWrapper {
public:
   SliceArrayWrapper(SliceObj slice_obj) = delete;
};


template <>
class SliceArrayWrapper<seqN> {
public:
   STRICT_CONSTEXPR explicit SliceArrayWrapper(const seqN& sl) : sl_{sl} {
   }

   STRICT_CONSTEXPR_INLINE index_t size() const {
      return sl_.size();
   }

   STRICT_CONSTEXPR StrictBool valid(BaseType auto const& A) const {
      return sl_.valid(A);
   }

   STRICT_CONSTEXPR StrictBool valid_first(TwoDimBaseType auto const& A) const {
      return sl_.valid_first(A);
   }

   STRICT_CONSTEXPR StrictBool valid_second(TwoDimBaseType auto const& A) const {
      return sl_.valid_second(A);
   }

   STRICT_CONSTEXPR_INLINE ImplicitInt map(ImplicitInt i) const {
      return sl_.start() + i.get() * sl_.stride();
   }

   STRICT_CONSTEXPR_INLINE const auto& get() const& {
      return sl_;
   }

   STRICT_CONSTEXPR_INLINE auto get() && {
      return std::move(sl_);
   }

private:
   seqN sl_;
};


template <>
class SliceArrayWrapper<std::vector<ImplicitInt>> {
public:
   STRICT_CONSTEXPR explicit SliceArrayWrapper(std::vector<ImplicitInt>&& indexes)
       : indexes_{std::move(indexes)} {
   }

   STRICT_CONSTEXPR_INLINE index_t size() const {
      return to_index_t(indexes_.size());
   }

   // Unlike specialization for seqN, specialization for vector only supports one-dimensional
   // types.
   STRICT_CONSTEXPR StrictBool valid(OneDimBaseType auto const& A) const {
      return valid_slice_vector(A, indexes_);
   }

   STRICT_CONSTEXPR StrictBool valid_first(TwoDimBaseType auto const& A) const {
      return valid_row_slice_vector(A, indexes_);
   }

   STRICT_CONSTEXPR StrictBool valid_second(TwoDimBaseType auto const& A) const {
      return valid_col_slice_vector(A, indexes_);
   }

   STRICT_CONSTEXPR_INLINE ImplicitInt map(ImplicitInt i) const {
      return indexes_[to_size_t(i.get())];
   }

   STRICT_CONSTEXPR_INLINE const auto& get() const& {
      return indexes_;
   }

   STRICT_CONSTEXPR_INLINE auto get() && {
      return std::move(indexes_);
   }

private:
   std::vector<ImplicitInt> indexes_;
};


}  // namespace detail


}  // namespace spp
