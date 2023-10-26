/**
 * @file    user_exception_proxy_in_t.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 wrapper for TAO user exceptions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/user_exception_proxy_in_t.h"

namespace TAOX11_NAMESPACE
{
  template <typename USEREX>
  const char *
  UserExceptionProxy::in<USEREX>::_rep_id () const
  {
    return this->user_exc_._rep_id ();
  }

  template <typename USEREX>
  const char *
  UserExceptionProxy::in<USEREX>::_name () const
  {
    return this->user_exc_._name ();
  }

  // TODO virtual CORBA::TypeCode_ptr _tao_type () const;

  template <typename USEREX>
  TAO_CORBA::Exception *
  UserExceptionProxy::in<USEREX>::_tao_duplicate () const
  {
    TAO_CORBA::Exception *result = new (std::nothrow) UserExceptionProxy::in<USEREX> (*this);
    if (!result)
      throw TAO_CORBA::NO_MEMORY ();
    return result;
  }

  template <typename USEREX>
  void
  UserExceptionProxy::in<USEREX>::_raise () const
  {
    throw USEREX (this->user_exc_);
  }

  template <typename USEREX>
  void
  UserExceptionProxy::in<USEREX>::_tao_encode (TAO_OutputCDR &/*cdr*/) const
  {
  }

  template <typename USEREX>
  void
  UserExceptionProxy::in<USEREX>::_tao_decode (TAO_InputCDR &cdr)
  {
    this->user_exc_._tao_decode (cdr);
  }
} // namespace TAOX11_NAMESPACE
