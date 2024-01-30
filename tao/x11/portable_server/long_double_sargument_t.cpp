/**
 * @file    long_double_sargument_t.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA CORE
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_LONG_DOUBLE_SARGUMENT_T_CPP
#define TAOX11_LONG_DOUBLE_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/x11/portable_server/long_double_sargument_t.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if (ACE_SIZEOF_LONG_DOUBLE != 16)

namespace TAOX11_NAMESPACE::PS
{
template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
In_Basic_SArgument_T<long double, Insert_Policy>::demarshal (TAO_InputCDR &cdr)
{
  TAO_CORBA::LongDouble ld;
  if (!(cdr >> ld))
    return false;
  this->x_ = ld;
  return true;
}

#if TAO_HAS_INTERCEPTORS == 1

template<template <typename> class Insert_Policy>
void
In_Basic_SArgument_T<long double,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_SArgument_T<long double,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  TAO_CORBA::LongDouble ld;
  return cdr << ld.assign (this->x_);
}

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_SArgument_T<long double,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  TAO_CORBA::LongDouble ld;
  if (!(cdr >> ld))
    return false;
  this->x_ = ld;
  return true;
}

#if TAO_HAS_INTERCEPTORS == 1

template<template <typename> class Insert_Policy>
void
Inout_Basic_SArgument_T<long double,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Out_Basic_SArgument_T<long double,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  TAO_CORBA::LongDouble ld;
  return cdr << ld.assign (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<template <typename> class Insert_Policy>
void
Out_Basic_SArgument_T<long double,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Ret_Basic_SArgument_T<long double,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  TAO_CORBA::LongDouble ld;
  return cdr << ld.assign (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<template <typename> class Insert_Policy>
void
Ret_Basic_SArgument_T<long double,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */
} // namespace TAOX11_NAMESPACE::PS

#endif /* ACE_SIZEOF_LONG_DOUBLE != 16 */

#endif /* TAOX11_LONG_DOUBLE_SARGUMENT_T_CPP */
