/**
 * @file    special_basic_sargument_t.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 POA CORE
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_SPECIAL_BASIC_SARGUMENT_T_CPP
#define TAOX11_SPECIAL_BASIC_SARGUMENT_T_CPP

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#include "tao/x11/portable_server/special_basic_sargument_t.h"
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

namespace TAOX11_NAMESPACE::PS
{
template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
In_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
In_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ===========================================================

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << from_S (this->x_);
}

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Inout_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::demarshal (TAO_InputCDR & cdr)
{
  return cdr >> to_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
Inout_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Out_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
Out_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

// ==============================================================

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
TAO_CORBA::Boolean
Ret_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::marshal (TAO_OutputCDR & cdr)
{
  return cdr << from_S (this->x_);
}

#if TAO_HAS_INTERCEPTORS == 1

template<typename S,
         typename to_S,
         typename from_S,
         template <typename> class Insert_Policy>
void
Ret_Special_Basic_SArgument_T<S,to_S,from_S,Insert_Policy>::
interceptor_value (TAO_CORBA::Any *any) const
{
  Insert_Policy<S>::any_insert (any, this->x_);
}

#endif /* TAO_HAS_INTERCEPTORS */

} // namespace TAOX11_NAMESPACE::PS

#endif /* TAO_SPECIAL_BASIC_SARGUMENT_T_CPP */
