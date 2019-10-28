/**
 * @file    foo_iorinterceptor_orbinitializer.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" IOR test interceptor ORB
 *          initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo_iorinterceptor.h"
#include "foo_iorinterceptor_orbinitializer.h"
#include "tao/x11/orb_constants.h"
#include "tao/x11/codecfactory/codecfactory.h"

void
FOO_IORInterceptor_ORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type /* info */)
{
}

void
FOO_IORInterceptor_ORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  IDL::traits<IOP::CodecFactory>::ref_type codec_factory =
    info->codec_factory ();

  // Set up a structure that contains information necessary to
  // create a GIOP 1.2 CDR encapsulation Codec.
  IOP::Encoding encoding (
      IOP::ENCODING_CDR_ENCAPS, 1, 2);

  // Obtain the CDR encapsulation Codec.
  IDL::traits<IOP::Codec>::ref_type codec =
    codec_factory->create_codec (encoding);

  IDL::traits<PortableInterceptor::IORInterceptor>::ref_type ior_interceptor =
      CORBA::make_reference<FOO_IORInterceptor> (codec);

  info->add_ior_interceptor (ior_interceptor);
}
