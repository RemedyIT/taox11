/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "client_orbinitializer.h"

#include "testC.h"
#include "tao/x11/pi/orb_initializer_registry.h"

#include "testlib/taox11_testlog.h"

#include <fstream>
#include <thread>

int
main(int argc, ACE_TCHAR *argv[])
{
  int status = 0;

  try
  {
    IDL::traits<PortableInterceptor::ORBInitializer>::ref_type initializer =
      CORBA::make_reference<Client_ORBInitializer> ("file://test2.ior");

    PortableInterceptor::register_orb_initializer (initializer);

    IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv, "Client ORB");

    // Start out with the first IOR.  Interaction with the second
    // IOR occurs during the various interceptions executed during
    // this test.
    IDL::traits<CORBA::Object>::ref_type obj =
      orb->string_to_object ("file://test1.ior");

    IDL::traits<ForwardRequestTest::test>::ref_type server =
        IDL::traits<ForwardRequestTest::test>::narrow (obj);

    if (!server)
    {
      TAOX11_TEST_ERROR << "Object reference <file://test1.ior> is nil." << std::endl;
      return 1;
    }

    // Invoke the operation four times.  By design, the last three
    // invocations in this test will cause
    // PortableInterceptor::ForwardRequest exceptions to be thrown,
    // thus causing the request to be forwarded to another object.

    int16_t old_number = 0;  // Previous invocation result.
    int16_t number = 0;      // New invocation result.
    for (int i = 1; i <= 5; ++i)
    {
      TAOX11_TEST_INFO << "CLIENT: Issuing request " << i << "." << std::endl;

      if (i > 1)
        old_number = number;

      number = server->number ();

      TAOX11_TEST_INFO << "CLIENT: Request " << i << " handled by object " << number << "." << std::endl;

      // Check if the new result is the same as the previous
      // result.
      //
      // This test is designed so that no two sequential
      // invocation results are the same.  If they are the same,
      // then the requests were invoked on the same object, one
      // after the other.  This means that forwarding did not
      // occur, which is of course a failure in the
      // PortableInterceptor::ForwardRequest support.
      if (i > 1 && old_number == number)
      {
        status = -1;

        TAOX11_TEST_ERROR << "TEST FAILED: Request was not forwarded." << std::endl;

        break;
      }
    }

    server->shutdown ();

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "client exception caught: " << ex << std::endl;
    return 1;
  }

  if (status == 0)
  {
    TAOX11_TEST_INFO << "PortableInterceptor::ForwardRequest test passed." << std::endl;
  }

  return (status == 0 ? 0 : 1);
}
