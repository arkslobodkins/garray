// Arkadijs Slobodkins, 2023


#pragma once


#include <type_traits>

#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/common_traits.hpp"
#include "../StrictCommon/strict_traits.hpp"


namespace spp::expr {


template <typename T, typename F> concept UnaryOperation
    = StrictType<std::invoke_result_t<F, ValueTypeOf<T>>>;


template <typename T1, typename T2, typename F> concept BinaryOperation
    = StrictType<std::invoke_result_t<F, ValueTypeOf<T1>, ValueTypeOf<T2>>>;


}  // namespace spp::expr


namespace spp::detail {


template <typename Base1, typename Base2> concept RealExprDeleted
    = (RealBaseType<RemoveRef<Base1>> && RealBaseType<RemoveRef<Base2>>)
   && (ArrayRealTypeRvalue<Base1> || ArrayRealTypeRvalue<Base2>
       || !same_dimension_b<RemoveRef<Base1>, RemoveRef<Base2>>());


template <typename Base1, typename Base2> concept IntegerExprDeleted
    = (IntegerBaseType<RemoveRef<Base1>> && IntegerBaseType<RemoveRef<Base2>>)
   && (ArrayIntegerTypeRvalue<Base1> || ArrayIntegerTypeRvalue<Base2>
       || !same_dimension_b<RemoveRef<Base1>, RemoveRef<Base2>>());


template <typename Base1, typename Base2> concept BooleanExprDeleted
    = (BooleanBaseType<RemoveRef<Base1>> && BooleanBaseType<RemoveRef<Base2>>)
   && (ArrayBooleanTypeRvalue<Base1> || ArrayBooleanTypeRvalue<Base2>
       || !same_dimension_b<RemoveRef<Base1>, RemoveRef<Base2>>());


template <typename Base1, typename Base2> concept FloatingExprDeleted
    = (FloatingBaseType<RemoveRef<Base1>> && FloatingBaseType<RemoveRef<Base2>>)
   && (ArrayFloatingTypeRvalue<Base1> || ArrayFloatingTypeRvalue<Base2>
       || !same_dimension_b<RemoveRef<Base1>, RemoveRef<Base2>>());


}  // namespace spp::detail
