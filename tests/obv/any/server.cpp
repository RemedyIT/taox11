/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "anys_impl.h"
#include "testlib/taox11_testlog.h"
#include <fstream>

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB::_ref_type orb = CORBA::ORB_init (argc, argv);

      IDL::traits<CORBA::Object>::ref_type obj = orb->resolve_initial_references ("RootPOA");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poa_manager = root_poa->the_POAManager ();

      if (!poa_manager)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
        return 1;
      }

      Test_impl::_ref_type servant = CORBA::make_reference< Test_impl > (orb);
      PortableServer::ObjectId id = root_poa->activate_object (servant);

      IDL::traits<CORBA::Object>::ref_type object = root_poa->id_to_reference (id);

      OBV_AnyTest::Test::_ref_type server =
        IDL::traits<OBV_AnyTest::Test >::narrow (object);

      std::string ior = orb->object_to_string (server);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos("test.ior");
      if (!fos)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
        return 1;
      }
      fos << ior;
      fos.close ();

      TAOX11_TEST_DEBUG << "IOR for Supports_Test::test servant written to 'test.ior' : " << ior << std::endl;

      poa_manager->activate ();

      orb->run ();

      // Destroy the POA, waiting until the destruction terminates
      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }

  return 0;
}
