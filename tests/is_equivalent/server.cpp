/**
 * @file    server.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"

#include "testlib/taox11_testlog.h"
#include "tao/x11/portable_server/portableserver_functions.h"
#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::Object>::ref_type obj = _orb->resolve_initial_references ("RootPOA");

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

      CORBA::PolicyList policies;
      policies.push_back (root_poa->create_lifespan_policy (PortableServer::LifespanPolicyValue::PERSISTENT));
      policies.push_back (root_poa->create_id_assignment_policy (PortableServer::IdAssignmentPolicyValue::USER_ID));

      IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

      if (poaman->_is_equivalent (nullptr))
      {
        TAOX11_TEST_ERROR << "ERROR: POAManager should not be equivalent to null object." << std::endl;
        return 1;
      }
      if (poaman->_is_equivalent (root_poa))
      {
        TAOX11_TEST_ERROR << "ERROR: POAManager should not be equivalent to root poa." << std::endl;
        return 1;
      }

      IDL::traits<PortableServer::POA>::ref_type child_poa =
        root_poa->create_POA ("ChildPOA", poaman, policies);

      for (IDL::traits<CORBA::Policy>::ref_type& _pol : policies)
        _pol->destroy();

      if (!poaman)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
        return 1;
      }

      CORBA::servant_traits<Test::Hello_Factory>::ref_type hello_factory_impl =
          CORBA::make_reference<Hello_Factory> (_orb, child_poa);

      TAOX11_TEST_DEBUG << "created Hello_Factory servant" << std::endl;

      PortableServer::ObjectId oid =
        PortableServer::string_to_ObjectId ("hellofactory");
      child_poa->activate_object_with_id (oid, hello_factory_impl);

      TAOX11_TEST_DEBUG << "activated Hello_Factory servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type hello_factory_obj =
        child_poa->id_to_reference (oid);

      if (!hello_factory_obj)
      {
        TAOX11_TEST_ERROR << "ERROR: child_poa->id_to_reference (id) returned null reference." << std::endl;
        return 1;
      }

      IDL::traits<Test::Hello_Factory>::ref_type hello_factory = IDL::traits<Test::Hello_Factory>::narrow (hello_factory_obj);

      if (!hello_factory)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello_Factory>::narrow (hello_factory_obj) returned null reference." << std::endl;
        return 1;
      }

      std::string ior = _orb->object_to_string (hello_factory);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos("test.ior");
      if (!fos)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
        return 1;
      }
      fos << ior;
      fos.close ();

      TAOX11_TEST_DEBUG << "IOR for Hello_Factory servant written to 'test.ior' : " << ior << std::endl;

      poaman->activate ();

      TAOX11_TEST_DEBUG << "starting event loop" << std::endl;

      _orb->run ();

      TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }

  return 0;
}
