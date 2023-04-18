/**
 * @file    codecfactory_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IOP library
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/codecfactory/codecfactory_impl.h"

#include "tao/x11/log.h"
#include "tao/x11/exception_macros.h"
#include "tao/x11/codecfactory/codec_impl.h"

namespace TAOX11_NAMESPACE
{
  namespace IOP_impl
  {
    CodecFactory::CodecFactory (TAO_IOP::CodecFactory_ptr codecfactory_ptr)
      : proxy_ (codecfactory_ptr)
    {
      this->set_proxy (new Object_proxy (codecfactory_ptr));
    }

    TAOX11_IDL::traits<::TAOX11_NAMESPACE::IOP::Codec>::ref_type
    CodecFactory::create_codec (const ::TAOX11_NAMESPACE::IOP::Encoding& enc)
    {
      try
      {
        TAO_ENCODING tao_enc;
        tao_enc.format = enc.format ();
        tao_enc.major_version = enc.major_version ();
        tao_enc.minor_version = enc.minor_version ();
        TAO_IOP::Codec_ptr codec = this->proxy_->create_codec (tao_enc);

        IDL::traits<IOP::Codec>::ref_type retval =
          CORBA::make_reference< IOP_impl::Codec > (codec);
        return retval;
      }
      catch (const TAO_CODEC_FACTORY::UnknownEncoding&)
      {
        throw IOP::CodecFactory::UnknownEncoding ();
      }
      return nullptr;
    }

    TAOX11_IDL::traits<::TAOX11_NAMESPACE::IOP::Codec>::ref_type
    CodecFactory::create_codec_with_codesets (
      const ::TAOX11_NAMESPACE::IOP::Encoding_1_2& enc)
    {
      try
      {
        TAO_ENCODING_1_2 tao_encoding_1_2;
        tao_encoding_1_2.format = enc.format ();
        tao_encoding_1_2.major_version = enc.major_version ();
        tao_encoding_1_2.minor_version = enc.minor_version ();
        tao_encoding_1_2.char_codeset = enc.char_codeset ();
        tao_encoding_1_2.wchar_codeset = enc.wchar_codeset ();

        TAO_IOP::Codec_ptr codec =
          this->proxy_->create_codec_with_codesets (tao_encoding_1_2);

        IDL::traits<IOP::Codec>::ref_type retval =
          CORBA::make_reference< IOP_impl::Codec > (codec);
        return retval;
      }
      catch (const TAO_CODEC_FACTORY::UnknownEncoding &)
      {
         throw IOP::CodecFactory::UnknownEncoding ();
      }
      catch (const TAO_CODEC_FACTORY::UnsupportedCodeset &ex)
      {
        throw IOP::CodecFactory::UnsupportedCodeset (ex.codeset);
      }
      return nullptr;
    }
  } // namespace IOP_impl

  namespace CORBA
  {
    template<>
    TAOX11_CODECFACTORY_Export object_traits<TAOX11_NAMESPACE::IOP::CodecFactory>::ref_type
    object_traits<TAOX11_NAMESPACE::IOP::CodecFactory>::narrow (
       object_traits<TAOX11_NAMESPACE::CORBA::Object>::ref_type objref)
    {
      if (objref)
      {
        if (objref->_is_local ())
        {
          return ref_type::_narrow (std::move(objref));
        }

        try
        {
          TAO_IOP::CodecFactory_ptr const codecfactory_ptr =
            TAO_IOP::CodecFactory::_narrow (objref->get_proxy ()->get ());
          if (!TAO_CORBA::is_nil (codecfactory_ptr))
          {
            return CORBA::make_reference< IOP_impl::CodecFactory > (codecfactory_ptr);
          }
        }
        catch_tao_system_ex(_ex)
      }

      return nullptr;
    }
  } // namespace CORBA
} // namespace TAOX11_NAMESPACE
