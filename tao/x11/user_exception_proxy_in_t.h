/**
 * @file    user_exception_proxy_in_t.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 wrapper for TAO user exceptions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_USER_EXCEPTION_PROXY_IN_T_H
#define TAOX11_USER_EXCEPTION_PROXY_IN_T_H

#pragma once

#include "tao/x11/user_exception.h"
#include "tao/x11/tao_corba.h"
#include "tao/UserException.h"

// --------------------------------------------------------------

namespace TAOX11_NAMESPACE
{
  namespace UserExceptionProxy
  {
    // Used to wrap/decode incoming (through CDR stream) user exceptions.
    // The address of the _alloc() method is passed in the exception data
    // to the TAO invocation mechanism where it will be used to create
    // an exception object when a user exception is detected in an incoming
    // CDR stream.
    // The allocator creates a typed in<user_ex_type> instance which routes
    // the _tao_decode() method to the _decode() method of the enclosed
    // TAOX11_NAMESPACE exception.
    // The _raise() throws a stack based duplicate of the enclosed TAOX11_NAMESPACE
    // exception.
    template <typename USEREX>
    class in final
      : public TAO_CORBA::UserException
    {
    public:
      ~in () override = default;

      in &operator= (const in &uex)
      {
        this->TAO_CORBA::UserException::operator= (uex);
        this->user_exc_ = uex.user_exc_;
        return *this;
      }

      /// Return the repository ID of the Exception.
      const char * _rep_id () const override;

      /// Return the name of the Exception.
      const char * _name () const override;

      static TAO_CORBA::Exception *_alloc ()
      {
        TAO_CORBA::Exception *result = new (std::nothrow) UserExceptionProxy::in<USEREX> ();
        if (!result)
          throw TAO_CORBA::NO_MEMORY ();
        return result;
      }

      // TODO virtual CORBA::TypeCode_ptr _tao_type () const;

      TAO_CORBA::Exception *_tao_duplicate () const override;

      void _raise () const override;

      void _tao_encode (TAO_OutputCDR &cdr) const override;  // noop
      void _tao_decode (TAO_InputCDR &cdr) override;

    private:
      in () = default;
      in (const in& rhs)
        : TAO_CORBA::UserException (rhs),
          user_exc_ (rhs.user_exc_)
      {}

      USEREX user_exc_;
    }; // in

  } // namespace UserExceptionProxy
} // namespace TAOX11_NAMESPACE

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/user_exception_proxy_in_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("tao/x11/user_exception_proxy_in_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAOX11_USER_EXCEPTION_PROXY_IN_T_H */
