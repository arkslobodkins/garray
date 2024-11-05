// Arkadijs Slobodkins, 2023


#pragma once


#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/common_traits.hpp"
#include "../derived1D.hpp"


namespace spp::detail {


template <typename Base1, typename Base2> concept OneDimRealExprRvalues
    = (OneDimRealBaseType<RemoveRef<Base1>> && OneDimRealBaseType<RemoveRef<Base2>>)
   && (ArrayOneDimRealTypeRvalue<Base1> || ArrayOneDimRealTypeRvalue<Base2>);


template <typename Base1, typename Base2> concept OneDimIntegerExprRvalues
    = (OneDimIntegerBaseType<RemoveRef<Base1>> && OneDimIntegerBaseType<RemoveRef<Base2>>)
   && (ArrayOneDimIntegerTypeRvalue<Base1> || ArrayOneDimIntegerTypeRvalue<Base2>);


template <typename Base1, typename Base2> concept OneDimBooleanExprRvalues
    = (OneDimBooleanBaseType<RemoveRef<Base1>> && OneDimBooleanBaseType<RemoveRef<Base2>>)
   && (ArrayOneDimBooleanTypeRvalue<Base1> || ArrayOneDimBooleanTypeRvalue<Base2>);


template <typename Base1, typename Base2> concept OneDimFloatingExprRvalues
    = (OneDimFloatingBaseType<RemoveRef<Base1>> && OneDimFloatingBaseType<RemoveRef<Base2>>)
   && (ArrayOneDimFloatingTypeRvalue<Base1> || ArrayOneDimFloatingTypeRvalue<Base2>);


}  // namespace spp::detail

