/**
 * @file    client_request_interceptor2.cpp
 * @author  Martin Corino
 *
 * @brief   PICurrentTest server-side ClientRequestInterceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "client_request_interceptor2.h"

#include "testlib/taox11_testlog.h"

ClientRequestInterceptor2::ClientRequestInterceptor2 (
  PortableInterceptor::SlotId id)
  : slot_id_ (id)
{
}

std::string
ClientRequestInterceptor2::name ()
{
  return "ClientRequestInterceptor2";
}

void
ClientRequestInterceptor2::destroy ()
{
}

void
ClientRequestInterceptor2::send_request (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ClientRequestInterceptor2::send_request (" << ri->operation () << ")" << std::endl;

  std::string op = ri->operation ();

  if (op != "invoke_you")
    return; // Don't mess with PICurrent if not invoking test method.

  try
  {
    // The goal of this test is to verify that a request scope
    // current was successfully shallow copied from a TSC that
    // itself was shallow copied from a ServerRequestInfo's RSC.
    // Specifically, verify that the RSC->TSC->RSC sequence of
    // copies occurred correctly.

    // Retrieve data from the RSC (request scope current).
    uint32_t number = 0;

    CORBA::Any data =
      ri->get_slot (this->slot_id_);

    if (!(data >>= number))
    {
      TAOX11_TEST_ERROR << "ERROR: Unable to extract data from " \
                  "CORBA::Any retrieved from RSC." << std::endl;

      throw CORBA::INTERNAL ();
    }

    TAOX11_TEST_INFO << "Extracted <" << number << "> from RSC slot " << this->slot_id_ << std::endl;
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in ""send_request() : " << ex << std::endl;

    TAOX11_TEST_ERROR << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "RSC->TSC->RSC copying appears to be working." << std::endl;
}

void
ClientRequestInterceptor2::send_poll (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
ClientRequestInterceptor2::receive_reply (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
ClientRequestInterceptor2::receive_exception (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
ClientRequestInterceptor2::receive_other (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}
