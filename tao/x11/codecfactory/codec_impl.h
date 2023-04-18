/**
 * @file    codec_impl.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IOP::CodecFactory library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_CODEC_IMPL_H_INCLUDE
#define TAOX11_CODEC_IMPL_H_INCLUDE

#pragma once

#include "tao/x11/codecfactory/tao_codecfactory.h"
#include "tao/x11/objproxy.h"
#include "tao/x11/codecfactory/codecfactory.h"

namespace TAOX11_NAMESPACE
{
  namespace IOP_impl
  {
    /**
     * @class Codec
     *
     * @brief TAOX11 wrapper for the TAO IOP::Codec implementation
     *
     */
    class TAOX11_CODECFACTORY_Export Codec final
      : public IDL::traits<IOP::Codec>::base_type
    {
    public:
      Codec (TAO_IOP::Codec_ptr codec_ptr);
      ~Codec () = default;

      ::TAOX11_NAMESPACE::CORBA::OctetSeq
      encode (const TAOX11_NAMESPACE::CORBA::Any& data) override;

      TAOX11_NAMESPACE::CORBA::Any
      decode (const ::TAOX11_NAMESPACE::CORBA::OctetSeq& data) override;

      ::TAOX11_NAMESPACE::CORBA::OctetSeq
      encode_value (const TAOX11_NAMESPACE::CORBA::Any& data) override;

      TAOX11_NAMESPACE::CORBA::Any
      decode_value (const ::TAOX11_NAMESPACE::CORBA::OctetSeq& data,
        TAOX11_IDL::traits<::TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type tc) override;

    private:
      Codec (const Codec&) = delete;
      Codec (Codec&&) = delete;
      Codec& operator= (const Codec&) = delete;
      Codec& operator= (Codec&&) = delete;

      TAO_IOP::Codec_ptr proxy_;
    };
  } // namespace IOP_impl
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_CODEC_IMPL_H_INCLUDE */
