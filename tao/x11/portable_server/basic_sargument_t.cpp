/**
 * @file    basic_sargument_t.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_BASIC_SARGUMENT_T_CPP
#define TAOX11_BASIC_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/x11/portable_server/basic_sargument_t.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

namespace TAOX11_NAMESPACE::PS
{
template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
In_Basic_SArgument_T<S,Insert_Policy>::demarshal (TAO_InputCDR &cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
In_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Basic_SArgument_T<S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
Inout_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Out_Basic_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
Out_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ============================================================

template<typename S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Ret_Basic_SArgument_T<S,Insert_Policy>::marshal (TAO_OutputCDR &cdr)
{
  return cdr << this->x_;
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         template <typename> class Insert_Policy>
void
Ret_Basic_SArgument_T<S,Insert_Policy>::interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

} // namespace TAOX11_NAMESPACE::PS

#endif /* TAOX11_BASIC_SARGUMENT_T_CPP */
