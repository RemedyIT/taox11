/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "client_orb_initializer.h"

#include "testC.h"
#include "tao/x11/pi/orb_initializer_registry.h"
#include "tao/x11/pi/pi.h"

#include "testlib/taox11_testlog.h"

#include <fstream>
#include <thread>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<PortableInterceptor::ORBInitializer>::ref_type initializer =
      CORBA::make_reference<ClientORBInitializer> ();

    PortableInterceptor::register_orb_initializer (initializer);

    IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv, "client_orb");

    IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("PICurrent");

    if (!obj)
    {
      TAOX11_TEST_ERROR << "Unable to obtain PICurrent reference." << std::endl;
      return 1;
    }

    IDL::traits<PortableInterceptor::Current>::ref_type pi_current =
      IDL::traits<PortableInterceptor::Current>::narrow (obj);

    if (!pi_current)
    {
      TAOX11_TEST_ERROR << "Unable to narrow PICurrent reference." << std::endl;
      return 1;
    }

    // Insert some data into the allocated PICurrent slot.
    CORBA::Any data;
    uint32_t number = 46;

    data <<= number;

    // Now reset the contents of our slot in the thread-scope
    // current (TSC).
    pi_current->set_slot (::slot_id,
                          data);

    // Resolve the target object, and perform the invocation.
    obj = orb->string_to_object ("file://test.ior");

    IDL::traits<PICurrentTest::test>::ref_type server =
        IDL::traits<PICurrentTest::test>::narrow (obj);

    if (!server)
    {
      TAOX11_TEST_ERROR << "Object reference <file://test.ior> is nil." << std::endl;
      return 1;
    }

    server->invoke_me ();

    CORBA::Any new_data = pi_current->get_slot (::slot_id);

    // The original data in the TSC was of type uint32_t.  If the
    // following extraction from the CORBA::Any fails, then the
    // original data in the TSC was not replaced within the client
    // request interceptor, as this test should do.
    std::string str;
    if (new_data >>= str)
    {
      TAOX11_TEST_INFO << "Retrieved \"" << str << "\" from the TSC." << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "Unable to extract data (a string) " \
                  "from the TSC." << std::endl;
      server->shutdown ();
      return 1;
    }

    server->invoke_we ();

    try
    {
      server->test_exception ();
    }
    catch (const PICurrentTest::bar&)
    {
      TAOX11_TEST_INFO << "Caught correct exception" << std::endl;
    }

    server->shutdown ();

    std::this_thread::sleep_for (std::chrono::seconds (1));

    orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "client exception caught: " << e << std::endl;
    return 1;
  }

  return 0;
}
