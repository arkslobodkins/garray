// Arkadijs Slobodkins, 2023


#pragma once


namespace spp {


enum AlignmentFlag { Aligned, Unaligned };


namespace detail {


template <typename T, AlignmentFlag AF>
consteval int alignment_of() {
   // Align to 512 byte boundary for AVX-512.
   return (AF == Aligned) ? 512 : 0;
}


}  // namespace detail


}  // namespace spp

