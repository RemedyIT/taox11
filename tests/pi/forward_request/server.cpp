/**
 * @file    server.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "server_orbinitializer.h"
#include "server_request_interceptor.h"

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
    IDL::traits<Server_ORBInitializer>::ref_type initializer =
      CORBA::make_reference<Server_ORBInitializer> ();

    PortableInterceptor::register_orb_initializer (initializer);

    IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv, "Server ORB");

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

    CORBA::PolicyList policies;  // Empty policy list.

    // Servant 1
    CORBA::servant_traits<test_i>::ref_type servant1 =
        CORBA::make_reference<test_i> (1, orb);

    IDL::traits<PortableServer::POA>::ref_type first_poa =
      root_poa->create_POA ("first POA",
                            poa_manager,
                            policies);

    PortableServer::ObjectId oid1 =
      first_poa->activate_object (servant1);

    IDL::traits<CORBA::Object>::ref_type obj1 =
      first_poa->servant_to_reference (servant1);

    std::string ior1 =
      orb->object_to_string (obj1);

    TAOX11_TEST_INFO << "ForwardRequestTest::test servant 1: <" << ior1 << ">" << std::endl;

    // Servant 2
    CORBA::servant_traits<test_i>::ref_type servant2 =
        CORBA::make_reference<test_i> (2, orb);

    IDL::traits<PortableServer::POA>::ref_type second_poa =
      root_poa->create_POA ("second POA",
                            poa_manager,
                            policies);

    PortableServer::ObjectId oid2 =
      second_poa->activate_object (servant2);

    IDL::traits<CORBA::Object>::ref_type obj2 =
      second_poa->servant_to_reference (servant2);

    std::string ior2 =
      orb->object_to_string (obj2);

    TAOX11_TEST_INFO << "ForwardRequestTest::test servant 2: <" << ior2 << ">" << std::endl;

    poa_manager->activate ();

    // Set the forward references in the server request interceptor.
    IDL::traits<PortableInterceptor::ServerRequestInterceptor>::ref_type
      server_interceptor = initializer->server_interceptor ();

    IDL::traits<ForwardRequestTest::ServerRequestInterceptor>::ref_type interceptor =
        IDL::traits<ForwardRequestTest::ServerRequestInterceptor>::narrow (
         server_interceptor);

    if (!interceptor)
    {
      TAOX11_TEST_ERROR << "Could not obtain reference to " \
                         "server request interceptor." << std::endl;
      return 1;
    }

    interceptor->forward_references (obj1, obj2);

    // Write each IOR to a file.

    // IOR 1
    std::ofstream fos("test1.ior");
    if (!fos)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test1.ior'" << std::endl;
        return 1;
      }
    fos << ior1;
    fos.close ();

    std::ofstream fos2("test2.ior");
    if (!fos2)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test2.ior'" << std::endl;
        return 1;
      }
    fos2 << ior2;
    fos2.close ();

    // Run the ORB event loop.
    orb->run ();

    root_poa->destroy (true, true);

    orb->destroy ();

    TAOX11_TEST_INFO << "event loop finished" << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "server exception caught: " << ex.what () << std::endl;
    return 1;
  }

  return 0;
}
