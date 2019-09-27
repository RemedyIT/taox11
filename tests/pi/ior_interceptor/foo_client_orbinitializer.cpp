/**
 * @file    foo_client_orbinitializer.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" IOR test interceptor ORB
 *          initializer.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "foo_client_orbinitializer.h"
#include "foo_client_request_interceptor.h"
#include "tao/x11/orb_constants.h"
#include "tao/x11/codecfactory/codecfactory.h"

#include "testlib/taox11_testlog.h"

void
FOO_Client_ORBInitializer::pre_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type /* info */)
{
  TAOX11_TEST_INFO << "FOO_Client_ORBInitializer::pre_init called" << std::endl;
}

void
FOO_Client_ORBInitializer::post_init (
    IDL::traits<PortableInterceptor::ORBInitInfo>::ref_type info)
{
  TAOX11_TEST_INFO << "FOO_Client_ORBInitializer::post_init called" << std::endl;

  IDL::traits<IOP::CodecFactory>::ref_type codec_factory =
    info->codec_factory ();

  // Set up a structure that contains information necessary to
  // create a GIOP 1.2 CDR encapsulation Codec.
  IOP::Encoding encoding (
      IOP::ENCODING_CDR_ENCAPS, 1, 2);

  // Obtain the CDR encapsulation Codec.
  IDL::traits<IOP::Codec>::ref_type codec =
    codec_factory->create_codec (encoding);

  IDL::traits<PortableInterceptor::ClientRequestInterceptor>::ref_type interceptor =
      CORBA::make_reference<FOO_ClientRequestInterceptor> (codec);

  info->add_client_request_interceptor (interceptor);

  TAOX11_TEST_INFO << "FOO_Client_ORBInitializer::post_init finished" << std::endl;
}
