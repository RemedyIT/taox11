/**
 * @file    server.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "supports_test_impl.h"
#include "testlib/taox11_testlog.h"
#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

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

      IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

      if (!poaman)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
        return 1;
      }

      /* Create, register factories */

      node_init_impl::_ref_type node_factory = CORBA::make_reference<node_init_impl> ();

      CORBA::ValueFactoryBase::_ref_type returned_factory =
          orb->register_value_factory (node_factory->_obv_repository_id (), node_factory);

      ACE_ASSERT (returned_factory == nullptr);

      vt_graph_init_impl::_ref_type vt_graph_factory = CORBA::make_reference<vt_graph_init_impl> ();

      returned_factory =
          orb->register_value_factory (vt_graph_factory->_obv_repository_id (), vt_graph_factory);

      ACE_ASSERT (returned_factory == nullptr);

      /* create and activate test servant */

      test_impl::_ref_type a_test_impl =
        CORBA::make_reference<test_impl> (orb);

      PortableServer::ObjectId id =
        root_poa->activate_object (a_test_impl);

      IDL::traits<CORBA::Object>::ref_type object = root_poa->id_to_reference (id);

      Supports_Test::test::_ref_type a_test = IDL::traits<Supports_Test::test>::narrow (object);

      std::string ior = orb->object_to_string (a_test);

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

      poaman->activate ();

      orb->run ();

      root_poa->destroy (false, false);

      orb->destroy ();

      TAOX11_TEST_DEBUG << "Server completed test successfully" << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }

  return 0;
}
