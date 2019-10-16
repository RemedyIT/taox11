/**
 * @file    root_poa.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA  basic POA test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/portable_server/portableserver.h"

#include "testlib/taox11_testlog.h"

int
main (int argc, char *argv[])
{
  try
    {
      // Initialize the ORB
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      // Resolve the initial references for the name RootPOA thus getting
      // an object of type CORBA::Object.
      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

      // apply narrow on the object of type CORBA::Object, to make it
      // a POA class Object.
      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
      {
        TAOX11_TEST_ERROR << "Unable to narrow PortableServer::POA from CORBA::Object reference"
            << std::endl;
        return 1;
      }

      // implicitly widen
      obj = root_poa;

      // (re-)narrow
      root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
      {
        TAOX11_TEST_ERROR << "Unable to (re-)narrow PortableServer::POA from CORBA::Object reference"
            << std::endl;
        return 1;
      }

      // Get the name of the root POA.
      std::string poa_name = root_poa->the_name ();

      TAOX11_TEST_DEBUG << "The RootPOA is : " << poa_name << std::endl;

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << ex;
      return 1;
    }

  return 0;
}
