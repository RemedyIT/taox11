/**
 * @file    foo_client_request_interceptor.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the "FOO" client request test
 *          interceptor.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo_client_request_interceptor.h"
#include "testC.h"

#include "testlib/taox11_testlog.h"

#include "tao/x11/anytypecode/typecode_constants.h"

FOO_ClientRequestInterceptor::FOO_ClientRequestInterceptor (
  IDL::traits<IOP::Codec>::ref_type codec)
  : codec_ (std::move (codec))
{
}

std::string
FOO_ClientRequestInterceptor::name ()
{
  return "FOO_ClientRequestInterceptor";
}

void
FOO_ClientRequestInterceptor::destroy ()
{
}

void
FOO_ClientRequestInterceptor::send_request (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri)
{
  // Verify that the tagged component that was supposed to be embedded
  // into the IOR by the FOO_IORInterceptor is actually in the IOR
  // profile.
  IOP::TaggedComponent component = ri->get_effective_component (FOO::COMPONENT_ID);

  // The correct tagged component appears to exist.  Display the
  // contents of the component data, which should be a NULL terminated
  // string.  In the case of this test, that string should be the name
  // of the IORInterceptor that added the tagged component to the IOR.

  if (component.component_data ().empty ())
  {
    // This should never happen because the IORInterceptor name is
    // not an empty string.
    TAOX11_TEST_ERROR << "Internal test error.  Empty tagged component." << std::endl;

    throw CORBA::INTERNAL ();
  }

  CORBA::OctetSeq data (std::move (component.component_data ()));

  // Extract the data from the octet sequence.
  CORBA::Any decoded_data =
    this->codec_->decode_value (data,
                                CORBA::_tc_string);

  std::string name;

  if (!(decoded_data >>= name))
  {
    TAOX11_TEST_ERROR <<  "ERROR: Unable to extract tagged component " \
                "data from Any." << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "String extracted from tagged component " \
              "data: " << name << std::endl;
}

void
FOO_ClientRequestInterceptor::send_poll (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
FOO_ClientRequestInterceptor::receive_reply (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
FOO_ClientRequestInterceptor::receive_exception (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
FOO_ClientRequestInterceptor::receive_other (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}
