/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "foo_client_orbinitializer.h"

#include "testC.h"
#include "tao/x11/pi/orb_initializer_registry.h"

#include "testlib/taox11_testlog.h"

#include <fstream>
#include <thread>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<PortableInterceptor::ORBInitializer>::ref_type initializer =
      CORBA::make_reference<FOO_Client_ORBInitializer> ();

    PortableInterceptor::register_orb_initializer (initializer);

    IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv, "client_orb");

    IDL::traits<CORBA::Object>::ref_type obj =
      orb->string_to_object ("file://test.ior");

    IDL::traits<FOO::test>::ref_type server =
        IDL::traits<FOO::test>::narrow (obj);

    if (!server)
    {
      TAOX11_TEST_ERROR << "Object reference <file://test.ior> is nil." << std::endl;
      return 1;
    }

    server->shutdown ();

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "client exception caught: " << ex.what () << std::endl;
    return 1;
  }

  return 0;
}
