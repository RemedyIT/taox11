/**
 * @file    user_exception.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 User exception base class
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_USER_EXCEPTION_H_INCLUDED
#define TAOX11_USER_EXCEPTION_H_INCLUDED

#include "tao/x11/exception.h"

namespace TAOX11_NAMESPACE {
  namespace CORBA {

    class TAOX11_Export UserException : public Exception
    {
    public:
      /// Destructor.
      ~UserException () noexcept override = default;

      /// noop
      void _tao_encode (TAO_OutputCDR &) const override;
      void _tao_decode (TAO_InputCDR &) override;

      void _any_insert (CORBA::Any &) const override;
      void _any_insert (CORBA::Any &) override;

    protected:
      UserException () = default;
      UserException (const UserException &) = default;
      UserException (UserException &&) = default;
      UserException & operator = (const UserException &) = default;
      UserException & operator = (UserException &&) = default;

      /// Constructor using a repository id.
      UserException (const char *repository_id,
                     const char *local_name)
        : CORBA::Exception (repository_id, local_name)
      {
      }

      void _info (std::ostream& strm) const override;
    };
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_USER_EXCEPTION_H_INCLUDED
