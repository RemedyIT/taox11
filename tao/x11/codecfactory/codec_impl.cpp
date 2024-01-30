/**
 * @file    codec_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IOP library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/codecfactory/codec_impl.h"

#include "tao/x11/log.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/anytypecode/any_util.h"
#include "tao/x11/anytypecode/typecode_impl.h"

namespace TAOX11_NAMESPACE
{
  namespace IOP_impl
  {
    Codec::Codec (TAO_IOP::Codec_ptr codec_ptr)
      : proxy_ (codec_ptr)
    {
      this->set_proxy (new Object_proxy (codec_ptr));
    }

    TAOX11_NAMESPACE::CORBA::OctetSeq
    Codec::encode (const TAOX11_NAMESPACE::CORBA::Any& data)
    {
      TAO_CORBA::Any tao_data;
      try
      {
        if (Any_Util::x11_any_to_tao_any (data, tao_data))
        {
          TAO_CORBA::OctetSeq_var tao_seq = this->proxy_->encode (tao_data);
          return seq_to_x11< TAOX11_NAMESPACE::CORBA::OctetSeq >
            (tao_seq.in ());
        }

      }
      catch (const TAO_CODEC::InvalidTypeForEncoding &)
      {
        throw IOP::Codec::InvalidTypeForEncoding ();
      }

      TAOX11_LOG_ERROR ("IOP_impl::Codec::encode - Unable to convert "
        << "a TAOX11 to a TAO Any");
      return TAOX11_NAMESPACE::CORBA::OctetSeq ();
    }

    TAOX11_NAMESPACE::CORBA::Any
    Codec::decode (const TAOX11_NAMESPACE::CORBA::OctetSeq& data)
    {
      TAOX11_NAMESPACE::CORBA::Any x11_ret;

      try
      {
        TAO_CORBA::OctetSeq tao_seq;
        tao_seq.length (ACE_Utils::truncate_cast<TAO_CORBA::ULong> (data.size ()));
        seq_from_x11< TAO_CORBA::OctetSeq, TAOX11_NAMESPACE::CORBA::OctetSeq >
           (tao_seq, data);

        TAO_CORBA::Any_var tao_ret = this->proxy_->decode (tao_seq);

        if (!Any_Util::tao_any_to_x11_any (tao_ret.in (), x11_ret))
        {
          TAOX11_LOG_ERROR ("IOP_impl::Codec::decode - Unable to convert "
              << "a TAOX11 Any to a TAO Any");
        }
      }
      catch (const TAO_CODEC::FormatMismatch &)
      {
        throw IOP::Codec::FormatMismatch ();
      }
      catch (const TAO_CODEC::TypeMismatch &)
      {
        throw IOP::Codec::TypeMismatch ();
      }

      return x11_ret;
    }

    TAOX11_NAMESPACE::CORBA::OctetSeq
    Codec::encode_value (const TAOX11_NAMESPACE::CORBA::Any& data)
    {
      try
      {
        TAO_CORBA::Any tao_data;
        if (Any_Util::x11_any_to_tao_any (data, tao_data))
        {
          TAO_CORBA::OctetSeq_var tao_seq = this->proxy_->encode_value (tao_data);
          return seq_to_x11< TAOX11_NAMESPACE::CORBA::OctetSeq >
            (tao_seq.in ());
        }
      }
      catch (const TAO_CODEC::InvalidTypeForEncoding &)
      {
        throw IOP::Codec::InvalidTypeForEncoding ();
      }
      TAOX11_LOG_ERROR ("IOP_impl::Codec::encode_value - Unable to convert "
        << "a TAOX11 Any to a TAO Any");
      return TAOX11_NAMESPACE::CORBA::OctetSeq ();
    }

    TAOX11_NAMESPACE::CORBA::Any
    Codec::decode_value (const TAOX11_NAMESPACE::CORBA::OctetSeq& data,
      TAOX11_IDL::traits<TAOX11_NAMESPACE::CORBA::TypeCode>::ref_type tc)
    {
      TAOX11_NAMESPACE::CORBA::Any x11_ret;

      try
      {
        TAO_CORBA::OctetSeq tao_seq;
        tao_seq.length (ACE_Utils::truncate_cast<TAO_CORBA::ULong> (data.size ()));
        seq_from_x11< TAO_CORBA::OctetSeq, TAOX11_NAMESPACE::CORBA::OctetSeq >
           (tao_seq, data);

        TAO_CORBA::TypeCode_var tao_tc = TAOX11_NAMESPACE::TC_helper::get_tao_tc (tc);
        if (TAO_CORBA::is_nil (tao_tc.in ()))
        {
          TAOX11_LOG_ERROR ("IOP_impl::Codec::decode_value - Unable to convert "
              << "a TAOX11 TypeCode to a TAO TypeCode");
        }
        TAO_CORBA::Any_var tao_ret = this->proxy_->decode_value (tao_seq, tao_tc.in ());

        if (!Any_Util::tao_any_to_x11_any (tao_ret.in (), x11_ret))
        {
          TAOX11_LOG_ERROR ("IOP_impl::Codec::decode_value - Unable to convert "
              << "a TAOX11 Any to a TAO Any");
        }
      }
      catch (const TAO_CODEC::FormatMismatch &)
      {
        throw IOP::Codec::FormatMismatch ();
      }
      catch (const TAO_CODEC::TypeMismatch &)
      {
        throw IOP::Codec::TypeMismatch ();
      }
      return x11_ret;
    }
  } // namespace IOP_impl

  namespace CORBA
  {
    template<>
    TAOX11_CODECFACTORY_Export object_traits< TAOX11_NAMESPACE::IOP::Codec>::ref_type
    object_traits< TAOX11_NAMESPACE::IOP::Codec >::narrow (
       object_traits< TAOX11_NAMESPACE::CORBA::Object>::ref_type obj)
    {
      if (obj)
      {
        if (obj->_is_local ())
        {
          return ref_type::_narrow (std::move(obj));
        }
        try
        {
          TAO_IOP::Codec_ptr const codec =
            TAO_IOP::Codec::_narrow (obj->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (codec))
          {
            return CORBA::make_reference< IOP_impl::Codec > (codec);
          }
        }
        catch_tao_system_ex(_ex)
      }
      return nullptr;
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
