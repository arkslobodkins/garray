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

class ArrayBase {};
class OneDimArrayBase : protected ArrayBase {};
class TwoDimArrayBase : protected ArrayBase {};
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


namespace detail {


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T, bool cnd> concept RvalueOf = !std::is_lvalue_reference_v<T> && cnd;


template <typename T> concept ArrayType = BaseOf<ArrayBase, T>;
template <typename T> concept ArrayRealType = ArrayType<T> && Real<BuiltinTypeOf<T>>;
template <typename T> concept ArrayBooleanType = ArrayType<T> && Boolean<BuiltinTypeOf<T>>;
template <typename T> concept ArrayIntegerType = ArrayType<T> && Integer<BuiltinTypeOf<T>>;
template <typename T> concept ArrayFloatingType = ArrayType<T> && Floating<BuiltinTypeOf<T>>;


template <typename T> concept ArrayTypeRvalue = RvalueOf<T, ArrayType<RemoveRef<T>>>;
template <typename T> concept ArrayRealTypeRvalue = RvalueOf<T, ArrayRealType<RemoveRef<T>>>;
template <typename T> concept ArrayBooleanTypeRvalue = RvalueOf<T, ArrayBooleanType<RemoveRef<T>>>;
template <typename T> concept ArrayIntegerTypeRvalue = RvalueOf<T, ArrayIntegerType<RemoveRef<T>>>;
template <typename T> concept ArrayFloatingTypeRvalue = RvalueOf<T, ArrayFloatingType<RemoveRef<T>>>;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> concept ArrayOneDimType = BaseOf<OneDimArrayBase, T>;
template <typename T> concept ArrayOneDimRealType = ArrayOneDimType<T> && Real<BuiltinTypeOf<T>>;
template <typename T> concept ArrayOneDimBooleanType = ArrayOneDimType<T> && Boolean<BuiltinTypeOf<T>>;
template <typename T> concept ArrayOneDimIntegerType = ArrayOneDimType<T> && Integer<BuiltinTypeOf<T>>;
template <typename T> concept ArrayOneDimFloatingType = ArrayOneDimType<T> && Floating<BuiltinTypeOf<T>>;


template <typename T> concept ArrayOneDimTypeRvalue = RvalueOf<T, ArrayOneDimType<RemoveRef<T>>>;
template <typename T> concept ArrayOneDimRealTypeRvalue = RvalueOf<T, ArrayOneDimRealType<RemoveRef<T>>>;
template <typename T> concept ArrayOneDimBooleanTypeRvalue = RvalueOf<T, ArrayOneDimBooleanType<RemoveRef<T>>>;
template <typename T> concept ArrayOneDimIntegerTypeRvalue = RvalueOf<T, ArrayOneDimIntegerType<RemoveRef<T>>>;
template <typename T> concept ArrayOneDimFloatingTypeRvalue = RvalueOf<T, ArrayOneDimFloatingType<RemoveRef<T>>>;


////////////////////////////////////////////////////////////////////////////////////////////////////
template <typename T> concept ArrayTwoDimType = BaseOf<TwoDimArrayBase, T>;
template <typename T> concept ArrayTwoDimRealType = ArrayTwoDimType<T> && Real<BuiltinTypeOf<T>>;
template <typename T> concept ArrayTwoDimBooleanType = ArrayTwoDimType<T> && Boolean<BuiltinTypeOf<T>>;
template <typename T> concept ArrayTwoDimIntegerType = ArrayTwoDimType<T> && Integer<BuiltinTypeOf<T>>;
template <typename T> concept ArrayTwoDimFloatingType = ArrayTwoDimType<T> && Floating<BuiltinTypeOf<T>>;


template <typename T> concept ArrayTwoDimTypeRvalue = RvalueOf<T, ArrayTwoDimType<RemoveRef<T>>>;
template <typename T> concept ArrayTwoDimRealTypeRvalue = RvalueOf<T, ArrayTwoDimRealType<RemoveRef<T>>>;
template <typename T> concept ArrayTwoDimBooleanTypeRvalue = RvalueOf<T, ArrayTwoDimBooleanType<RemoveRef<T>>>;
template <typename T> concept ArrayTwoDimIntegerTypeRvalue = RvalueOf<T, ArrayTwoDimIntegerType<RemoveRef<T>>>;
template <typename T> concept ArrayTwoDimFloatingTypeRvalue = RvalueOf<T, ArrayTwoDimFloatingType<RemoveRef<T>>>;


template <typename T> concept PointerConvertibleLvalue
    = std::is_lvalue_reference_v<T> && requires(RemoveRef<T> p) {
         { p } -> std::convertible_to<std::add_pointer_t<decltype(p[0])>>;
         requires CompatibleBuiltin<RemoveCVRef<decltype(p[0])>>;
         requires !IsConst<RemoveRef<decltype(p[0])>>;
      };


template <typename T> concept PointerConvertibleLvalueConst
    = std::is_lvalue_reference_v<T> && requires(RemoveRef<T> p) {
         { p } -> std::convertible_to<std::add_pointer_t<decltype(p[0])>>;
         requires CompatibleBuiltin<RemoveCVRef<decltype(p[0])>>;
         requires IsConst<RemoveRef<decltype(p[0])>>;
      };


}  // namespace detail


}  // namespace spp

