/**
 * @file    user_exception_proxy_out.cpp
 * @author  Martin Corino
 *
 * @brief   TAOX11 wrapper for TAO user exceptions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/user_exception_proxy_out.h"

namespace TAOX11_NAMESPACE
{
  UserExceptionProxy::out::~out ()
  {
    delete this->user_exc_;
  }

  const char *
  UserExceptionProxy::out::_rep_id () const
  {
    return this->user_exc_->_rep_id ();
  }

  const char *
  UserExceptionProxy::out::_name () const
  {
    return this->user_exc_->_name ();
  }

  // TODO virtual CORBA::TypeCode_ptr _tao_type () const;

  TAO_CORBA::Exception *
  UserExceptionProxy::out::_tao_duplicate () const
  {
    TAO_CORBA::Exception *result = new (std::nothrow) UserExceptionProxy::out (*this->user_exc_);
    if (!result)
      throw TAO_CORBA::NO_MEMORY ();
    return result;
  }

  void
  UserExceptionProxy::out::_raise () const
  {
    throw *this;
  }

  void
  UserExceptionProxy::out::_raise_x11 () const
  {
    this->user_exc_->_raise ();
  }

  void
  UserExceptionProxy::out::_tao_encode (TAO_OutputCDR &cdr) const
  {
    this->user_exc_->_tao_encode (cdr);
  }

  void
  UserExceptionProxy::out::_tao_decode (TAO_InputCDR &/*cdr*/)
  {
  }
} // namespace TAOX11_NAMESPACE
