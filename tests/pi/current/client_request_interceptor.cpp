/**
 * @file    client_request_interceptor.cpp
 * @author  Martin Corino
 *
 * @brief   PICurrentTest client-side ClientRequestInterceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "client_request_interceptor.h"

#include "testlib/taox11_testlog.h"

ClientRequestInterceptor::ClientRequestInterceptor (
  PortableInterceptor::SlotId id,
  IDL::traits<PortableInterceptor::Current>::ref_type pi_current)
  : slot_id_ (id),
    pi_current_ (std::move (pi_current))
{
}

std::string
ClientRequestInterceptor::name ()
{
  return "ClientRequestInterceptor";
}

void
ClientRequestInterceptor::destroy ()
{
}

void
ClientRequestInterceptor::send_request (
      IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ClientRequestInterceptor::send_request (" << ri->operation () << ") with id " << ri->request_id () << std::endl;

  IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type narrow_ri = IDL::traits<PortableInterceptor::ClientRequestInfo>::narrow (ri);
  IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type narrow_nullptr = IDL::traits<PortableInterceptor::ClientRequestInfo>::narrow (nullptr);
  if (!narrow_ri)
    {
      TAOX11_TEST_ERROR << "ERROR: CRI narrow failed" << std::endl;
    }
  if (narrow_nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CRI nullptr narrow failed" << std::endl;
    }

  IDL::traits<PortableInterceptor::RequestInfo>::ref_type cnarrow_ri = IDL::traits<PortableInterceptor::RequestInfo>::narrow (ri);
  IDL::traits<PortableInterceptor::RequestInfo>::ref_type cnarrow_nullptr = IDL::traits<PortableInterceptor::RequestInfo>::narrow (nullptr);
  if (!cnarrow_ri)
    {
      TAOX11_TEST_ERROR << "ERROR: rSRI narrow failed" << std::endl;
    }
  if (cnarrow_nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: rSRI nullptr narrow failed" << std::endl;
    }

  std::string const op = ri->operation ();

  if (op != "invoke_me")
    return; // Don't mess with PICurrent if not invoking test method.

  if (op == "shutdown")
  {
    if (ri->response_expected ())
    {
      TAOX11_TEST_ERROR << "ERROR: Shutdown shouldn't have a response expected." << std::endl;
      throw CORBA::INTERNAL ();
    }
  }
  else
  {
    if (!ri->response_expected ())
    {
      TAOX11_TEST_ERROR << "ERROR: Other operations shouldn't have a response expected." << std::endl;
      throw CORBA::INTERNAL ();
    }
  }

  try
  {
    // Retrieve data from the RSC (request scope current).
    uint32_t number = 0;

    CORBA::Any const data = ri->get_slot (this->slot_id_);

    if (!(data >>= number))
    {
      TAOX11_TEST_ERROR << "ERROR: Unable to extract data from Any." << std::endl;

      throw CORBA::INTERNAL ();
    }

    TAOX11_TEST_INFO << "Extracted <" << number << "> from RSC slot " << this->slot_id_ << std::endl;

    CORBA::Any new_data;
    std::string s = "Et tu Brute?";

    new_data <<= s;

    // Now reset the contents of our slot in the thread-scope
    // current (TSC).
    this->pi_current_->set_slot (this->slot_id_, new_data);

    // Now retrieve the data from the RSC again.  It should not have
    // changed!
    uint32_t number2 = -1;

    CORBA::Any data2 =
      ri->get_slot (this->slot_id_);

    if (!(data2 >>= number2)
        || number != number2)
    {
      TAOX11_TEST_ERROR << "ERROR: RSC was modified after " \
                  "TSC was modified." << std::endl;

      throw CORBA::INTERNAL ();
    }

    IDL::traits<CORBA::Object>::ref_type target = ri->target();

    IDL::traits<CORBA::Object>::ref_type effective_target = ri->effective_target();

    IOP::TaggedProfile tp = ri->effective_profile ();
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in ""send_request() : " << ex << std::endl;

    TAOX11_TEST_ERROR << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "Client side RSC/TSC semantics appear " \
              "to be correct." << std::endl;
}

void
ClientRequestInterceptor::send_poll (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
ClientRequestInterceptor::receive_reply (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}

void
ClientRequestInterceptor::receive_exception (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ClientRequestInterceptor::receive_exception (" << ri->operation () << ") with id " << ri->request_id () << std::endl;

  std::string const op = ri->operation ();

  if (op == "test_exception")
  {
    TAOX11_TEST_INFO << "ClientRequestInterceptor::receive_exception, exception: (" << ri->received_exception () << ") id: (" << ri->received_exception_id () << ")" << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "ClientRequestInterceptor::receive_exception called for operation " << op << std::endl;
  }
}

void
ClientRequestInterceptor::receive_other (
    IDL::traits<PortableInterceptor::ClientRequestInfo>::ref_type)
{
}
