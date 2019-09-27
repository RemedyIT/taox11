/**
 * @file    nested_non_servant_upcalls.cpp
 * @author  Mark Drijver
 *
 * @brief This program tests that nested non-servant upcalls are handled correctly.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testlib/taox11_testlog.h"
#include "servant_activator.h"

Object_Activator *global_object_activator = 0;

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    // Initialize the ORB first.
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv,
        "nested_non_servant_upcalls");

    // Obtain the RootPOA.
    IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

    // Narrow to POA.
    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);
    TAOX11_TEST_DEBUG << "narrowed root POA interface" << std::endl;

    // Get the POAManager of the RootPOA.
    IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();
    poa_manager->activate ();

    CORBA::PolicyList policies (0);
    policies.resize (1);

    policies[0] = root_poa->create_request_processing_policy (
        PortableServer::RequestProcessingPolicyValue::USE_SERVANT_MANAGER);

    IDL::traits<PortableServer::POA>::ref_type child_poa =
        root_poa->create_POA ("child", poa_manager, policies);

    IDL::traits<PortableServer::ServantActivator>::ref_type servant_activator =
        CORBA::make_reference<Servant_Activator> (child_poa);

    child_poa->set_servant_manager (servant_activator);
    IDL::traits<PortableServer::ServantManager>::ref_type got_servant_manager =
      child_poa->get_servant_manager ();
    if (got_servant_manager == nullptr)
    {
      TAOX11_TEST_ERROR << "Get_servant_manager returned nil." << std::endl;
    }

    IDL::traits<CORBA::Object>::ref_type first_object =
        child_poa->create_reference ("IDL:test:1.0");

    IDL::traits<test>::ref_type first_test = IDL::traits<test>::narrow (first_object);

    PortableServer::ObjectId id = child_poa->reference_to_id (first_object);

    ACE_Thread_Manager thread_manager;

    Object_Activator object_activator (thread_manager, child_poa);
    global_object_activator = &object_activator;

    first_test->method ();

    child_poa->deactivate_object (id);
    // Wait for the Object_Activator thread to exit.
    thread_manager.wait ();

    root_poa->destroy (true, true);
    orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    return 1;
  }

  return 0;
}
