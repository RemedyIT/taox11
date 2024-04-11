/**
 * @file    exception_holder_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 ExceptionHolder implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/base/tao_corba.h"
#include "tao/x11/anytypecode/typecode.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/messaging/exception_holder_i.h"
#include "tao/x11/system_exception_x11.h"
#include "tao/x11/exception_macros.h"

namespace TAOX11_NAMESPACE
{
  ExceptionHolder_i::ExceptionHolder_i (
    bool is_system_exception,
    bool byte_order,
    TAOX11_NAMESPACE::CORBA::OctetSeq marshaled_exception,
    const TAO_TAO::Exception_Data* data,
    const uint32_t exceptions_count,
    ACE_Char_Codeset_Translator *char_translator,
    ACE_WChar_Codeset_Translator *wchar_translator) :
      IDL::traits<Messaging::ExceptionHolder>::obv_type (is_system_exception, byte_order, marshaled_exception),
      data_ (data),
      count_ (exceptions_count),
      char_translator_ (char_translator),
      wchar_translator_ (wchar_translator)
  {
  }

  void ExceptionHolder_i::raise_exception ()
  {
    TAO_InputCDR _tao_in ((const char*) this->marshaled_exception ().data (),
                          this->marshaled_exception ().size (),
                          this->byte_order ());

    _tao_in.char_translator (this->char_translator_);
    _tao_in.wchar_translator (this->wchar_translator_);

    std::string type_id;

    if (!(_tao_in >> type_id))
      {
        // Could not demarshal the exception id, raise a local
        // CORBA::MARSHAL
        throw TAOX11_NAMESPACE::CORBA::MARSHAL (0, TAOX11_NAMESPACE::CORBA::COMPLETED_YES);
      }

    if (this->is_system_exception ())
      {
        TAO_CORBA::ULong minor {};
        TAO_CORBA::ULong completion {};
        if (!(_tao_in >> minor) ||
            !(_tao_in >> completion))
          {
            throw TAOX11_NAMESPACE::CORBA::MARSHAL (0, TAOX11_NAMESPACE::CORBA::COMPLETED_MAYBE);
          }

        TAO_CORBA::SystemException* exception =
          TAO_TAO::create_system_exception (type_id.c_str());

        if (!exception)
          {
            // @@ We should raise a CORBA::NO_MEMORY, but we ran out
            //    of memory already. We need a pre-allocated, TSS,
            //    CORBA::NO_MEMORY instance
            ACE_NEW (exception, TAO_CORBA::UNKNOWN);
          }
        exception->minor (minor);
        exception->completed (TAO_CORBA::CompletionStatus (completion));

        // Raise the exception.
        std::unique_ptr<TAO_CORBA::SystemException> e_ptr (exception);

        try
          {
            exception->_raise ();
          }
        catch_tao_system_ex(_ex)
        return;
      }

    // Match the exception interface repository id with the
    // exception in the exception list.
    // This is important to decode the exception.
    for (TAO_CORBA::ULong i = 0; i != this->count_; ++i)
      {
        if (std::strcmp (type_id.c_str(), this->data_[i].id) == 0)
          {
            TAO_CORBA::Exception * const exception = this->data_[i].alloc ();

            if (exception == nullptr)
              {
                throw TAOX11_NAMESPACE::CORBA::NO_MEMORY (0, TAOX11_NAMESPACE::CORBA::COMPLETED_YES);
              }
            else
              {
                exception->_tao_decode (_tao_in);
              }

            // Raise the exception.
            std::unique_ptr<TAO_CORBA::Exception> e_ptr (exception);
            exception->_raise ();
          }
      }

    // If we couldn't find the right exception, report it as
    // CORBA::UNKNOWN.

    // @@ It would seem like if the remote exception is a
    //    UserException we can assume that the request was
    //    completed.
    throw TAOX11_NAMESPACE::CORBA::UNKNOWN (0, TAOX11_NAMESPACE::CORBA::COMPLETED_YES);
  }

  void ExceptionHolder_i::raise_exception_with_list (
      const ::TAOX11_NAMESPACE::Dynamic::ExceptionList& )
  {
    //TODO convert exceptionlist to something we can really use?
    this->raise_exception ();
  }
}

