// Arkadijs Slobodkins, 2023


#pragma once


#include "../ArrayCommon/array_traits.hpp"
#include "../StrictCommon/strict_common.hpp"


namespace spp::detail {


template <typename Base, typename... BaseAndStrict> concept RandomRealBasesAndStrict
    = NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
   && StrictType<RemoveRef<SecondLastPack_t<BaseAndStrict...>>>
   && StrictType<RemoveRef<LastPack_t<BaseAndStrict...>>>;


template <typename Base, typename... BasesAndLowHigh> concept RandomRealBasesAndLowHigh
    = NonConstBaseType<RemoveRef<Base>> && Real<BuiltinTypeOf<Base>>
   && SameAs<Low<RealTypeOf<Base>>, RemoveRef<SecondLastPack_t<BasesAndLowHigh...>>>
   && SameAs<High<RealTypeOf<Base>>, RemoveRef<LastPack_t<BasesAndLowHigh...>>>;


template <typename... Base> concept RandomBases = AllNonConstBases<RemoveRef<Base>...>;


template <typename... Base> concept RandomRealBases
    = AllNonConstBases<RemoveRef<Base>...> && Real<BuiltinTypeOf<LastPack_t<Base...>>>;


}  // namespace spp::detail

