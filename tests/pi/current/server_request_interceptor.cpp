/**
 * @file    server_request_interceptor.cpp
 * @author  Martin Corino
 *
 * @brief   PICurrentTest server-side ServerRequestInterceptor
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "server_request_interceptor.h"

#include "testlib/taox11_testlog.h"

ServerRequestInterceptor::ServerRequestInterceptor (
  PortableInterceptor::SlotId id,
  IDL::traits<PortableInterceptor::Current>::ref_type pi_current)
  : slot_id_ (id),
    pi_current_ (std::move (pi_current))
{
}

std::string
ServerRequestInterceptor::name ()
{
  return "ServerRequestInterceptor";
}

void
ServerRequestInterceptor::destroy ()
{
}

void
ServerRequestInterceptor::receive_request_service_contexts (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ServerRequestInterceptor::receive_request_service_contexts (" << ri->operation () << ")" << std::endl;

  std::string const op = ri->operation ();

  if (op != "invoke_me")
    return; // Don't mess with PICurrent if not invoking test method.

  try
  {
    // Insert data into the RSC (request scope current).
    uint32_t number = 62;

    CORBA::Any data;
    data <<= number;

    ri->set_slot (this->slot_id_, data);

    TAOX11_TEST_INFO << "Inserted number <" << number << "> into RSC." << std::endl;
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in receive_request_service_contexts() : " << ex << std::endl;

    TAOX11_TEST_INFO << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }
}

void
ServerRequestInterceptor::receive_request (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ServerRequestInterceptor::receive_request (" << ri->operation () << ")" << std::endl;

  try
  {
    CORBA::OctetSeq const ai = ri->adapter_id ();
    PortableInterceptor::ObjectId const oi = ri->object_id ();
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type narrow_ri = IDL::traits<PortableInterceptor::ServerRequestInfo>::narrow (ri);
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type narrow_nullptr = IDL::traits<PortableInterceptor::ServerRequestInfo>::narrow (nullptr);

    IDL::traits<PortableInterceptor::RequestInfo>::ref_type cnarrow_ri = IDL::traits<PortableInterceptor::RequestInfo>::narrow (ri);
    IDL::traits<PortableInterceptor::RequestInfo>::ref_type cnarrow_nullptr = IDL::traits<PortableInterceptor::RequestInfo>::narrow (nullptr);

    CORBA::Any new_data;
    uint32_t number = 19;
    new_data <<= number;

    // Set a value in RSC, this should not effect TSC anymore
    ri->set_slot (this->slot_id_, new_data);

    // Now retrieve the data from the TSC again.  It should not have
    // changed to the new value
    CORBA::Any data2 =
      this->pi_current_->get_slot (this->slot_id_);

    uint32_t number2 = 0;
    if ((data2 >>= number2)
        && number == number2)
    {
      TAOX11_TEST_ERROR << "ERROR: TSC was modified because " \
                  "RSC was modified." << std::endl;

      throw CORBA::INTERNAL ();
    }

    PortableInterceptor::ORBId const orbid = ri->orb_id ();
    PortableInterceptor::AdapterName const adaptername = ri->adapter_name ();

    TAOX11_TEST_DEBUG << "Got orb id <" << orbid << "> and adapter name " << adaptername << std::endl;
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in receive_request() : " << ex << std::endl;

    TAOX11_TEST_INFO << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "Server side RSC/TSC semantics appear " \
              "to be correct." << std::endl;
}

void
ServerRequestInterceptor::send_reply (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ServerRequestInterceptor::send_reply (" << ri->operation () << ")" << std::endl;

  std::string op = ri->operation ();

  if (op != "invoke_me")
    return; // Don't mess with PICurrent if not invoking test method.

  try
  {
    CORBA::Any data;

    // Retrieve the data stored in the RSC.  This data (a string)
    // should be different from the original data (a uint32_t)
    // stored into the RSC by the receive_request_service_contexts()
    // interception point.
    data = ri->get_slot (this->slot_id_);

    // The original data in the RSC was of type uint32_t.  If the
    // following extraction from the CORBA::Any fails, then the
    // original data in the RSC was not replaced with the data in
    // the TSC after the test method completed.
    std::string str;
    if (data >>= str)
    {
      TAOX11_TEST_INFO << "Retrieved \"" << str << "\" from the RSC." << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "Unable to extract data (a string) " \
                  "from the RSC." << std::endl;

      throw CORBA::INTERNAL ();
    }

    // Now verify that the RSC is truly independent of the TSC.  In
    // particular, modifying the TSC at this point should not cause
    // the RSC to be modified.
    CORBA::Any new_data;
    uint32_t number = 19;

    new_data <<= number;

    // Now reset the contents of our slot in the thread-scope
    // current (TSC).
    this->pi_current_->set_slot (this->slot_id_,
                                 new_data);

    // Now retrieve the data from the RSC again.  It should not have
    // changed!
    CORBA::Any data2 =
      ri->get_slot (this->slot_id_);

    std::string str2;
    if (!(data2 >>= str2)
        || str != str2)
    {
      TAOX11_TEST_ERROR << "ERROR: RSC was modified after " \
                  "TSC was modified." << std::endl;

      throw CORBA::INTERNAL ();
    }
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in send_reply() : " << ex << std::endl;

    TAOX11_TEST_INFO << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "Server side RSC/TSC semantics appear " \
              "to be correct." << std::endl;
}

void
ServerRequestInterceptor::send_exception (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type ri)
{
  TAOX11_TEST_INFO << "ServerRequestInterceptor::send_exception (" << ri->operation () << ")" << std::endl;

  std::string const op = ri->operation ();

  if (op == "test_exception")
  {
    TAOX11_TEST_INFO << "ServerRequestInterceptor::send_exception, exception: (" << ri->sending_exception () << ")" << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "ServerRequestInterceptor::send_exception called for operation " << op << std::endl;
  }
}

void
ServerRequestInterceptor::send_other (
    IDL::traits<PortableInterceptor::ServerRequestInfo>::ref_type)
{
}
