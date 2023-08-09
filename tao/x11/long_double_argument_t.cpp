/* -------------------------------------------------------------------
 * @file    long_double_argument_t.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 CORE
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * -------------------------------------------------------------------*/
#ifndef TAOX11_LONG_DOUBLE_ARGUMENT_T_CPP_INCLUDED
#define TAOX11_LONG_DOUBLE_ARGUMENT_T_CPP_INCLUDED

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/x11/long_double_argument_t.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#if (ACE_SIZEOF_LONG_DOUBLE != 16)

namespace TAOX11_NAMESPACE
{

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
In_Basic_Argument_T<long double, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  TAO_CORBA::LongDouble ld;
  return cdr << ld.assign (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<template <typename> class Insert_Policy>
void
In_Basic_Argument_T<long double, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

template<template <typename> class Insert_Policy>
In_Basic_Clonable_Argument_T<long double,Insert_Policy>::~In_Basic_Clonable_Argument_T ()
{
  if (this->is_clone_)
    {
      long double* tmp = const_cast<long double*> (std::addressof(this->x_));
      delete tmp;
    }
}

template<template <typename> class Insert_Policy>
TAO_TAO::Argument*
In_Basic_Clonable_Argument_T<long double,Insert_Policy>::clone ()
{
  long double* clone_x = new long double (this->x_);
  In_Basic_Clonable_Argument_T<long double, Insert_Policy>* clone_arg =
          new In_Basic_Clonable_Argument_T<long double,Insert_Policy> (*clone_x);
  clone_arg->is_clone_ = true;
  return clone_arg;
}

// ===========================================================

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_Argument_T<long double, Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  TAO_CORBA::LongDouble ld;
  return cdr << ld.assign (this->x_);
}

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_Argument_T<long double, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
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
Inout_Basic_Argument_T<long double, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Out_Basic_Argument_T<long double, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
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
Out_Basic_Argument_T<long double, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Ret_Basic_Argument_T<long double, Insert_Policy>::demarshal (TAO_InputCDR & cdr)
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
Ret_Basic_Argument_T<long double, Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<long double>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

} // namespace TAOX11_NAMESPACE

#endif /* ACE_SIZEOF_LONG_DOUBLE != 16 */

#endif /* TAOX11_LONG_DOUBLE_ARGUMENT_T_CPP_INCLUDED */
