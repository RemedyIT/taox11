/**
 * @file    server.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

void testActiveObject(IDL::traits<CORBA::ORB>::ref_type _orb, IDL::traits<PortableServer::POA>::ref_type root_poa)
{
  CORBA::servant_traits<Test::Hello>::ref_type hello_impl =
    CORBA::make_reference < Hello > (_orb, root_poa);

  TAOX11_TEST_INFO << "created Hello servant" << std::endl;

  TAOX11_TEST_DEBUG << "Test on implicit activated object." << std::endl;
  IDL::traits<Test::Hello>::ref_type hello = hello_impl->_this();
}

IDL::traits<PortableServer::POA>::ref_type createChildPoa(IDL::traits<PortableServer::POA>::ref_type root_poa)
{
  TAOX11_TEST_DEBUG << "Create child poa with NO_IMPLICIT_ACTIVATION policy."
    << std::endl;
  // Policies for the childPOA to be created.
  CORBA::PolicyList policies(2);

  // Tell the POA to use a servant manager.
  IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type rp_pol =
    root_poa->create_request_processing_policy(
      PortableServer::RequestProcessingPolicyValue::USE_SERVANT_MANAGER);
  policies[0] = rp_pol;

  // Disallow implicit activation.
  IDL::traits<PortableServer::ImplicitActivationPolicy>::ref_type nia_pol =
    root_poa->create_implicit_activation_policy(
      PortableServer::ImplicitActivationPolicyValue::NO_IMPLICIT_ACTIVATION);
  policies[1] = nia_pol;

  IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
    root_poa->the_POAManager();
  IDL::traits<PortableServer::POA>::ref_type child_poa = root_poa->create_POA(
    "child_poa", poa_manager, policies);

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (IDL::traits<CORBA::Policy>::ref_type& _pol : policies)
  {
    _pol->destroy();
  }
  return child_poa;
}

int testInactiveObject(IDL::traits<CORBA::ORB>::ref_type _orb, IDL::traits<PortableServer::POA>::ref_type child_poa)
{
  TAOX11_TEST_INFO << "Create Hello servant" << std::endl;
  CORBA::servant_traits<Test::Hello>::ref_type hello_impl2 =
      CORBA::make_reference < Hello > (_orb, child_poa);

  TAOX11_TEST_DEBUG << "Test on inactive object with inactive poa_manager."
      << std::endl;
  try
  {
    IDL::traits<Test::Hello>::ref_type hello = hello_impl2->_this();

    TAOX11_TEST_ERROR << "ERROR: Implicit activation should have "
      << "thrown an exception with ServantNotActive." << std::endl;
    return 1;
  }
  catch (const PortableServer::POA::ServantNotActive&)
  {
    TAOX11_TEST_DEBUG << "Caught ServantNotActive as expected." << std::endl;
  }

  IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
    child_poa->the_POAManager();
  poa_manager->activate();
  TAOX11_TEST_DEBUG << "Test on inactive object." << std::endl;
  try
  {
    IDL::traits<Test::Hello>::ref_type hello = hello_impl2->_this();

    TAOX11_TEST_ERROR << "ERROR: Implicit activation should have "
      << "thrown an exception with ServantNotActive." << std::endl;
    return 1;
  }
  catch (const PortableServer::POA::ServantNotActive&)
  {
    TAOX11_TEST_DEBUG << "Caught ServantNotActive as expected." << std::endl;
  }
  return 0;
}

int main(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
          << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj = _orb->resolve_initial_references(
        "RootPOA");

    if (!obj)
    {
      TAOX11_TEST_ERROR
          << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
          << std::endl;
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

    IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager();
    if (!poa_manager)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object."
          << std::endl;
      return 1;
    }

    testActiveObject(_orb, root_poa);
    IDL::traits<PortableServer::POA>::ref_type child_poa = createChildPoa(root_poa);
    if (!child_poa)
    {
      TAOX11_TEST_ERROR << "ERROR: create_POA returned null reference." << std::endl;
      return 1;
    }
    int result = testInactiveObject(_orb, child_poa);
    if (result != 0)
      return result;
    TAOX11_TEST_DEBUG << "Done." << std::endl;

    root_poa->destroy(1, 1);
    _orb->destroy();
  } catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    return 1;
  }

  return 0;
}

