/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "server_manager.h"
#include "testlib/taox11_testlog.h"

int
main (int argc, char *argv[])
{
  try
    {
      Server_i server;

      if (server.init (argc, argv) != 0)
        {
          TAOX11_TEST_ERROR << "Failure during Initialisation: init ()" << std::endl;
          return 1;
        }


      // This POA is used to test the Servant Activator interface and
      // hence has an RETAIN policy.
      IDL::traits<PortableServer::POA>::ref_type first_poa =
        server.create_poa ("firstPOA", true);

      if (first_poa == nullptr)
        {
          TAOX11_TEST_ERROR << "Failure during first POA creation: create_poa ()"
            << std::endl;
          return 1;
        }

      // This poa is used to test the Servant Activator interface and
      // hence has an NONRETAIN policy.
      IDL::traits<PortableServer::POA>::ref_type second_poa =
        server.create_poa ("secondPOA", false);

      if (second_poa == nullptr)
        {
          TAOX11_TEST_ERROR << "Failure during second POA creation: create_poa ()"
            << std::endl;
          return 1;
        }

      // The Servant Activator interface is created and initialised.
      if (server.create_activator (first_poa) != 0)
        {
          TAOX11_TEST_ERROR << "Failure during first Servant Activator creation: create_activator()"
            << std::endl;
          return 1;
        }

      // The Servant Locator interface is created and initialised.
      if (server.create_locator (second_poa) !=0)
        {
          TAOX11_TEST_ERROR << "Failure during first Servant Locator creation: create_activator()"
            << std::endl;
          return 1;
        }

      // The server begins processing requests.
      if (server.run () != 0)
        {
          TAOX11_TEST_ERROR << "Failure during request processing: run ()" << std::endl;
          return 1;
        }
    }
  catch (const CORBA::Exception &ex)
    {
      TAOX11_TEST_ERROR << "main (server) : " << ex << std::endl;
      return 1;
    }
  return 0;
}
