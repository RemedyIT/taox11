/**
 * @file    user_exception_proxy_out.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 wrapper for TAO user exceptions
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_USER_EXCEPTION_PROXY_OUT_H
#define TAOX11_USER_EXCEPTION_PROXY_OUT_H

#pragma once

#include "tao/x11/user_exception.h"
#include "tao/x11/base/tao_corba.h"
#include "tao/UserException.h"

// --------------------------------------------------------------

namespace TAOX11_NAMESPACE
{
  namespace UserExceptionProxy
  {
    // Used to wrap/encode outgoing user exceptions.
    // TAOX11_NAMESPACE user exceptions will be caught by the TAOX11_NAMESPACE
    // upcall mechanism, copied into an out proxy instance which
    // is rethrown.
    // TAO will catch the out proxy instance and call _tao_encode
    // which is routed to the _encode() method of the enclosed
    // exception.
    class TAOX11_Export out final
      : public TAO_CORBA::UserException
    {
    public:
      out (const CORBA::UserException &ux11)
        : TAO_CORBA::UserException (),
          user_exc_ (dynamic_cast<CORBA::UserException *> (
                        ux11._tao_duplicate()))
      {}
      out (const out &rhs)
        : TAO_CORBA::UserException (rhs),
          user_exc_ (dynamic_cast<CORBA::UserException *> (
                        rhs.user_exc_->_tao_duplicate()))
      {}
      ~out () override;

      out &operator= (const out &uex)
      {
        if (this->user_exc_)
          delete this->user_exc_;
        this->user_exc_ = dynamic_cast<CORBA::UserException *> (
                              uex.user_exc_->_tao_duplicate());
        return *this;
      }

      /// Return the repository ID of the Exception.
      const char * _rep_id () const override;

      /// Return the name of the Exception.
      const char * _name () const override;

      // TODO virtual CORBA::TypeCode_ptr _tao_type () const;

      TAO_CORBA::Exception *_tao_duplicate () const override;

      void _raise () const override;

      void _raise_x11 () const;

      void _tao_encode (TAO_OutputCDR &cdr) const override;
      void _tao_decode (TAO_InputCDR &cdr) override; // noop

    private:
      TAOX11_NAMESPACE::CORBA::UserException *user_exc_;
    }; // out

  } // namespace UserExceptionProxy
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_USER_EXCEPTION_PROXY_OUT_H */
