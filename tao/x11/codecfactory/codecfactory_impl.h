/**
 * @file    codecfactory_impl.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IOP::CodecFactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_CODECFACTORY_IMPL_H_INCLUDE
#define TAOX11_CODECFACTORY_IMPL_H_INCLUDE

#pragma once

#include "tao/x11/codecfactory/tao_codecfactory.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/codecfactory/codecfactory.h"

namespace TAOX11_NAMESPACE
{
  namespace IOP_impl
  {
    /**
     * @class CodecFactory
     *
     * @brief TAOX11 wrapper for the TAO IOP::CodecFactory implementation
     *
     */
    class TAOX11_CODECFACTORY_Export CodecFactory final
      : public IDL::traits<IOP::CodecFactory>::base_type
    {
    public:
      CodecFactory (TAO_IOP::CodecFactory_ptr codecfactory_ptr);
      ~CodecFactory () = default;

      TAOX11_IDL::traits<::TAOX11_NAMESPACE::IOP::Codec>::ref_type
      create_codec (const ::TAOX11_NAMESPACE::IOP::Encoding& enc) override;

      TAOX11_IDL::traits<::TAOX11_NAMESPACE::IOP::Codec>::ref_type
      create_codec_with_codesets (const ::TAOX11_NAMESPACE::IOP::Encoding_1_2& enc) override;

    private:
      CodecFactory (const CodecFactory&) = delete;
      CodecFactory (CodecFactory&&) = delete;
      CodecFactory& operator= (const CodecFactory&) = delete;
      CodecFactory& operator= (CodecFactory&&) = delete;

      TAO_IOP::CodecFactory_ptr proxy_;
    };
  } // namespace IOP_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_CODECFACTORY_IMPL_H_INCLUDE */
