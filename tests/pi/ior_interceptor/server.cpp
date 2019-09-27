/**
 * @file    server.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "foo_iorinterceptor_orbinitializer.h"

#include "test_i.h"
#include "tao/x11/pi/orb_initializer_registry.h"
#include "tao/x11/pi/pi.h"

#include "testlib/taox11_testlog.h"

#include <fstream>
#include <thread>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<PortableInterceptor::ORBInitializer>::ref_type initializer =
      CORBA::make_reference<FOO_IORInterceptor_ORBInitializer> ();

    PortableInterceptor::register_orb_initializer (initializer);

    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv, "test_orb");

    IDL::traits<CORBA::Object>::ref_type obj =
      orb->resolve_initial_references ("RootPOA");

    IDL::traits<PortableServer::POA>::ref_type root_poa =
      IDL::traits<PortableServer::POA>::narrow (obj);

    if (!root_poa)
    {
      TAOX11_TEST_ERROR << "Unable to obtain RootPOA reference." << std::endl;
      return 1;
    }

    IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
      root_poa->the_POAManager ();

    CORBA::servant_traits<test_i>::ref_type server_impl
      = CORBA::make_reference<test_i> (orb);


    PortableServer::ObjectId id =
        root_poa->activate_object (server_impl);

    obj = root_poa->id_to_reference (id);

    if (!obj)
    {
      TAOX11_TEST_ERROR
        << "ERROR: root_poa->id_to_reference (id) returned null reference."
        << std::endl;
      return 1;
    }

    IDL::traits<FOO::test>::ref_type server =
      IDL::traits<FOO::test>::narrow (obj);

    if (!server)
    {
      TAOX11_TEST_ERROR
        << "ERROR: IDL::traits<PICurrentTest::test>::narrow (obj) returned null reference."
        << std::endl;
      return 1;
    }

    std::string ior =
      orb->object_to_string (server);

    // Output the IOR to the <ior_output_file>
    std::ofstream fos("test.ior");
    if (!fos)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
        return 1;
      }
    fos << ior;
    fos.close ();

    TAOX11_TEST_INFO << "IOR for test_i servant written to 'test.ior' : " << ior
      << std::endl;

    poa_manager->activate ();

    orb->run ();


    TAOX11_TEST_INFO << "event loop finished" << std::endl;

    root_poa->destroy (true, true);

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "server exception caught: " << ex.what () << std::endl;
    return 1;
  }

  return 0;
}
