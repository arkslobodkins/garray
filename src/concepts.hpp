// Arkadijs Slobodkins, 2023


#pragma once


#include "ArrayCommon/array_traits.hpp"
#include "Expr/expr_traits.hpp"
#include "StrictCommon/common_traits.hpp"
#include "StrictCommon/strict_traits.hpp"


namespace spp::concepts {


using spp::BaseOf;
using spp::DerivedFrom;
using spp::SameAs;


using spp::Integer;
using spp::SignedInteger;
using spp::StandardFloating;
using spp::UnsignedInteger;
#ifdef STRICT_QUAD_PRECISION
using spp::Floating;
using spp::Quadruple;
#else
using spp::Floating;
#endif


using spp::Boolean;
using spp::Builtin;
using spp::Real;
#ifdef STRICT_QUAD_PRECISION
using spp::NotQuadruple;
#else
using spp::NotQuadruple;
#endif


using spp::AllStrict;
using spp::StrictType;


using spp::BaseType;
using spp::BooleanBaseType;
using spp::FloatingBaseType;
using spp::IntegerBaseType;
using spp::RealBaseType;
using spp::SignedIntegerBaseType;


using spp::OneDimBaseType;
using spp::OneDimBooleanBaseType;
using spp::OneDimFloatingBaseType;
using spp::OneDimIntegerBaseType;
using spp::OneDimRealBaseType;
using spp::OneDimSignedIntegerBaseType;


using spp::TwoDimBaseType;
using spp::TwoDimBooleanBaseType;
using spp::TwoDimFloatingBaseType;
using spp::TwoDimIntegerBaseType;
using spp::TwoDimRealBaseType;
using spp::TwoDimSignedIntegerBaseType;


using spp::IndexType;
using spp::OneDimOwnerType;
using spp::TwoDimOwnerType;


using spp::expr::UnaryOperation;
using spp::expr::BinaryOperation;


}  // namespace spp::concepts

