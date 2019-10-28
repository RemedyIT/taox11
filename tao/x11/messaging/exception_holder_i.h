/**
 * @file    exception_holder_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 ExceptionHolder implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_MESSAGING_EXCEPTION_HOLDER_I_H
#define TAOX11_MESSAGING_EXCEPTION_HOLDER_I_H

#pragma once

#include "tao/x11/messaging/taox11_messaging_export.h"
#include "tao/Exception_Data.h"
#include "tao/x11/messaging/ExceptionHolderC.h"

namespace Dynamic
{
  class ExceptionList;
}

namespace TAOX11_NAMESPACE
{
  class TAOX11_MESSAGING_Export ExceptionHolder_i final
    : public virtual IDL::traits<TAOX11_NAMESPACE::Messaging::ExceptionHolder>::obv_type
  {
  public:
    ExceptionHolder_i (
      bool is_system_exception,
      bool byte_order,
      ::TAOX11_NAMESPACE::CORBA::OctetSeq marshaled_exception,
      const ::TAO_TAO::Exception_Data* data,
      const uint32_t exceptions_count,
      ACE_Char_Codeset_Translator *char_translator,
      ACE_WChar_Codeset_Translator *wchar_translator);

    virtual void raise_exception() override;

    virtual void raise_exception_with_list (
        const ::TAOX11_NAMESPACE::Dynamic::ExceptionList& exc_list) override;

  protected:
    virtual ~ExceptionHolder_i () = default;

  private:
    ExceptionHolder_i () = delete;
    ExceptionHolder_i (const ExceptionHolder_i&) = delete;
    ExceptionHolder_i (ExceptionHolder_i&&) = delete;
    ExceptionHolder_i& operator= (const ExceptionHolder_i&) = delete;
    ExceptionHolder_i& operator= (ExceptionHolder_i&&) = delete;

    const ::TAO_TAO::Exception_Data* data_;
    const uint32_t count_;
    ACE_Char_Codeset_Translator *char_translator_;
    ACE_WChar_Codeset_Translator *wchar_translator_;
  };
}

#endif /* TAO_MESSAGING_EXCEPTIONHOLDER_I_H */
