/**
 * @file    test_i.cpp
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "test_i.h"

test_i::test_i (IDL::traits<PortableInterceptor::Current>::ref_type current,
                PortableInterceptor::SlotId id,
                IDL::traits<CORBA::ORB>::ref_type orb)
  : current_ (std::move (current)),
    slot_id_ (id),
    orb_ (std::move (orb))
{
}

void
test_i::invoke_me ()
{
  static uint32_t invoke_count = 0;

  TAOX11_TEST_INFO << "test_i::invoke_me method invoked (" << ++invoke_count << ")." << std::endl;

  // @note "TSC" is "thread scope current"
  //       "RSC" is "request scope current"

  // ----------------------------------------------------

  // Verify that the following RSC->TSC->RSC copying scenario works:
  //
  // 1.    ServerRequestInterceptor::receive_request_service_contexts()
  //   a.       ServerRequestInfo::set_slot()
  //   b.       RSC->TSC shallow copy
  // 2.    servant implementation invokes method on another server
  //   a.       TSC->RSC shallow copy
  //   b.       ClientRequestInterceptor::send_request()
  //     i.          ClientRequestInfo::get_slot()

  // By this point all of step 1 has occurred.  Step 2 will now
  // occur.
  IDL::traits<PICurrentTest::test>::ref_type my_ref = this->_this ();

  // ----------------------------------------------------

  CORBA::Any retrieved_any;

  try
  {
    // Retrieve data placed into RSC PICurrent by the
    // receive_request_service_contexts() interception point, and
    // then copied into the TSC current.
    retrieved_any =
      this->current_->get_slot (this->slot_id_);
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in ""test_i::invoke_me() when calling ""Current::get_slot : " << ex << std::endl;

    TAOX11_TEST_ERROR << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Unexpected exception" << ex << std::endl;
  }

  uint32_t retrieved;
  if (retrieved_any >>= retrieved)
  {
    TAOX11_TEST_INFO << "Retrieved number <" << retrieved << "> from TSC." << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Problem extracting data from " \
                "CORBA::Any retrieved from TSC." << std::endl;
    throw CORBA::INTERNAL ();
  }
  // ----------------------------------------------------

  // Note that the invocation must occur through the object
  // reference to force the client request interceptor
  // (ClientRequestInterceptor2) to be invoked.  This assumes that
  // DIRECT collocation (and possibly THRU_POA collocation) is
  // disabled.
  my_ref->invoke_you ();

  // ----------------------------------------------------

  // Insert some data into the TSC PICurrent object.
  const std::string str = { "Drink milk!" };

  CORBA::Any data;

  data <<= str;

  try
  {
    this->current_->set_slot (this->slot_id_,
                              data);
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in ""test_i::invoke_me() when calling ""Current::set_slot : " << ex << std::endl;

    TAOX11_TEST_ERROR << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "String \"" << str << "\" inserted into TSC." << std::endl;
}

void
test_i::invoke_you ()
{
  // Nothing to be tested here.  This method is here just so that we
  // have a different method

  TAOX11_TEST_INFO << "test_i::invoke_you method invoked." << std::endl;
}

void
test_i::invoke_we ()
{
  TAOX11_TEST_INFO << "test_i::invoke_we method invoked." << std::endl;

  // Insert some data into the TSC PICurrent object.
  const std::string str = "We drink milk!";

  CORBA::Any data;

  data <<= str;

  try
  {
    this->current_->set_slot (this->slot_id_,
                              data);
  }
  catch (const PortableInterceptor::InvalidSlot& ex)
  {
    TAOX11_TEST_ERROR << "Exception thrown in ""test_i::invoke_we() when calling ""Current::set_slot : " << ex << std::endl;

    TAOX11_TEST_ERROR << "Invalid slot: " << this->slot_id_ << std::endl;

    throw CORBA::INTERNAL ();
  }

  TAOX11_TEST_INFO << "String \"" << str << "\" inserted into TSC." << std::endl;
}

void
test_i::shutdown ()
{
  TAOX11_TEST_INFO << "Server is shutting down." << std::endl;

  this->orb_->shutdown (false);
}
