// Arkadijs Slobodkins, 2023


#pragma once


#include <tuple>
#include <type_traits>
#include <utility>

#include "../StrictCommon/auxiliary_types.hpp"
#include "../StrictCommon/common_traits.hpp"
#include "../StrictCommon/strict_traits.hpp"


namespace spp {


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace detail {
class DimBase {};
class OneDimBase : protected DimBase {};
class TwoDimBase : protected DimBase {};
}  // namespace detail


// BaseType returns false for references; referenceness is not removed to prevent
// a possibility of dangling references and other misuses in generic code.
template <typename T> concept BaseType = BaseOf<detail::DimBase, T>;
template <typename T> concept RealBaseType = BaseType<T> && Real<typename T::builtin_type>;
template <typename T> concept BooleanBaseType = BaseType<T> && Boolean<typename T::builtin_type>;
template <typename T> concept IntegerBaseType = BaseType<T> && Integer<typename T::builtin_type>;
template <typename T> concept SignedIntegerBaseType
    = BaseType<T> && SignedInteger<typename T::builtin_type>;
template <typename T> concept FloatingBaseType = BaseType<T> && Floating<typename T::builtin_type>;


template <typename T> concept OneDimBaseType = BaseOf<detail::OneDimBase, T>;
template <typename T> concept OneDimRealBaseType = OneDimBaseType<T> && RealBaseType<T>;
template <typename T> concept OneDimBooleanBaseType = OneDimBaseType<T> && BooleanBaseType<T>;
template <typename T> concept OneDimIntegerBaseType = OneDimBaseType<T> && IntegerBaseType<T>;
template <typename T> concept OneDimSignedIntegerBaseType
    = OneDimBaseType<T> && SignedIntegerBaseType<T>;
template <typename T> concept OneDimFloatingBaseType = OneDimBaseType<T> && FloatingBaseType<T>;


template <typename T> concept TwoDimBaseType = BaseOf<detail::TwoDimBase, T>;
template <typename T> concept TwoDimRealBaseType = TwoDimBaseType<T> && RealBaseType<T>;
template <typename T> concept TwoDimBooleanBaseType = TwoDimBaseType<T> && BooleanBaseType<T>;
template <typename T> concept TwoDimIntegerBaseType = TwoDimBaseType<T> && IntegerBaseType<T>;
template <typename T> concept TwoDimSignedIntegerBaseType
    = TwoDimBaseType<T> && SignedIntegerBaseType<T>;
template <typename T> concept TwoDimFloatingBaseType = TwoDimBaseType<T> && FloatingBaseType<T>;


////////////////////////////////////////////////////////////////////////////////////////////////////
namespace detail {


struct ConstSliceBase {};
template <typename T> concept ConstSliceBaseType = BaseOf<ConstSliceBase, T>;


// Objects of type Array or slices of Array with non-constant semantics. Expression templates are
// excluded since they return by value and thus std::is_lvalue_reference_v evaluates to false.
template <typename T> concept NonConstBaseType
    = BaseType<T> && (requires(T A) {
         requires std::is_lvalue_reference_v<decltype(A.un(0))>;
      } && !std::is_const_v<T> && !ConstSliceBaseType<T>);
template <typename T> concept OneDimNonConstBaseType = OneDimBaseType<T> && NonConstBaseType<T>;
template <typename T> concept TwoDimNonConstBaseType = TwoDimBaseType<T> && NonConstBaseType<T>;


struct StoreByCopy {};
struct StoreByReference {};


class CopyBase1D : protected OneDimBase, protected StoreByCopy {};
class ReferenceBase1D : protected OneDimBase, protected StoreByReference {};
class CopyBase2D : protected TwoDimBase, protected StoreByCopy {};
class ReferenceBase2D : protected TwoDimBase, protected StoreByReference {};


template <typename T> concept CopyStorable = BaseOf<StoreByCopy, T> && BaseType<T>;
template <typename T> concept ReferenceStorable = BaseOf<StoreByReference, T> && BaseType<T>;


template <typename T>
struct CopyOrReferenceExpr;


template <typename T>
   requires CopyStorable<T>
struct CopyOrReferenceExpr<T> {
   using type = T;
};


template <typename T>
   requires ReferenceStorable<T>
struct CopyOrReferenceExpr<T> {
   using type = T&;
};


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename... Args>
struct LastPackTraits {
   using type = std::tuple_element_t<sizeof...(Args) - 1, std::tuple<Args...>>;
};


template <typename... Args>
using LastPack_t = LastPackTraits<Args...>::type;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T>
struct IsNonConstBase {
   static constexpr bool value = NonConstBaseType<T>;
};


template <typename... Args> concept AllNonConstBases = std::conjunction_v<IsNonConstBase<Args>...>;


}  // namespace detail


////////////////////////////////////////////////////////////////////////////////////////////////////
// Referenceness is removed so that aliases can be used for references as well.
template <typename T>
   requires(BaseType<std::remove_reference_t<T>>)
using SizeTypeOf = std::remove_reference_t<T>::size_type;


template <typename T>
   requires(BaseType<std::remove_reference_t<T>>)
using ValueTypeOf = std::remove_reference_t<T>::value_type;


template <typename T>
   requires(BaseType<std::remove_reference_t<T>>)
using BuiltinTypeOf = std::remove_reference_t<T>::builtin_type;


template <typename T>
   requires(RealBaseType<std::remove_reference_t<T>>)
using RealTypeOf = std::remove_reference_t<T>::builtin_type;


template <typename T> concept IndexType = SignedInteger<T> || SameAs<T, StrictInt>
                                       || SameAs<T, StrictLong> || SameAs<T, ImplicitInt>
                                       || SameAs<T, detail::Last>;


}  // namespace spp

