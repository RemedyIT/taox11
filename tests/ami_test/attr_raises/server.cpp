/**
 * @file    server.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 server ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ami_test_i.h"

#include "testlib/taox11_testlog.h"

#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj =
        _orb->resolve_initial_references ("RootPOA");

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
        root_poa->the_POAManager ();

      if (!poaman)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->the_POAManager () returned null object."
            << std::endl;
          return 1;
        }

      CORBA::servant_traits<A::AMI_Test>::ref_type hello_impl =
        CORBA::make_reference<AMI_Test_i> (_orb);

      TAOX11_TEST_INFO << "created Hello servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (hello_impl);

      TAOX11_TEST_INFO << "activated Hello servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type hello_obj =
        root_poa->id_to_reference (id);

      if (hello_obj == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
          return 1;
        }

      IDL::traits<A::AMI_Test>::ref_type hello =
          IDL::traits<A::AMI_Test>::narrow (hello_obj);

      if (hello == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Test::Hello>::narrow (hello_obj) returned null reference."
            << std::endl;
          return 1;
        }

      std::string ior = _orb->object_to_string (hello);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos("server.ior");
      if (!fos)
        {
          TAOX11_TEST_ERROR << "ERROR: failed to open file 'server.ior'" << std::endl;
          return 1;
        }
      fos << ior;
      fos.close ();

      TAOX11_TEST_INFO << "IOR for Hello servant written to 'server.ior' : " << ior << std::endl;

      poaman->activate ();

      TAOX11_TEST_INFO << "starting event loop" << std::endl;

      _orb->run ();

      TAOX11_TEST_INFO << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }

  return result;
}
