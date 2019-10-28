/**
 * @file    default_servant.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testS.h"
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/portable_server/portableserver_functions.h"
#include "testlib/taox11_testlog.h"

int _result = 0;

class test_i : public CORBA::servant_traits<test>::base_type
{
};

void
test_get_servant_manager(
    IDL::traits<PortableServer::POA>::weak_ref_type poa)
{
  IDL::traits<PortableServer::POA>::ref_type _poa = poa.lock ();
  if (!_poa)
  {
    TAOX11_TEST_ERROR << "ERROR: Obtaining reference to POA failed."
        << std::endl;
    ++_result;
    return;
  }

  try
  {
    // Getting the servant manager should give a wrong policy exception exception
    IDL::traits<PortableServer::ServantManager>::ref_type servant_manager =
        _poa->get_servant_manager ();
    X11_UNUSED_ARG(servant_manager);
  }
  catch (const PortableServer::POA::WrongPolicy&)
  {
    TAOX11_TEST_INFO
        << "Call get_servant_manager -> Caught POA::WrongPolicy exception as expected."
        << std::endl;
    return;
  }
  ++_result;
  TAOX11_TEST_ERROR
      << "ERROR, get servant manager failed, should give an exception."
      << std::endl;
}

void
test_set_servant_manager(IDL::traits<PortableServer::POA>::weak_ref_type poa)
{
  IDL::traits<PortableServer::POA>::ref_type _poa = poa.lock ();
  if (!_poa)
  {
    TAOX11_TEST_ERROR << "ERROR: Obtaining reference to POA failed."
        << std::endl;
    ++_result;
    return;
  }

  try
  {
    // Setting the servant manager should give a wrong policy exception exception
    _poa->set_servant_manager (nullptr);
  }
  catch (const PortableServer::POA::WrongPolicy&)
  {
    TAOX11_TEST_INFO
        << "Call set_servant_manager -> Caught POA::WrongPolicy exception as expected."
        << std::endl;
    return;
  }
  catch (const CORBA::Exception&)
  {
  }

  TAOX11_TEST_ERROR
      << "ERROR, set servant manager failed, should give an exception"
      << std::endl;
  ++_result;
}

void
test_get_servant_with_no_set(
    IDL::traits<PortableServer::POA>::weak_ref_type poa)
{
  IDL::traits<PortableServer::POA>::ref_type _poa = poa.lock ();
  if (!_poa)
  {
    TAOX11_TEST_ERROR << "ERROR: Obtaining reference to POA failed."
        << std::endl;
    ++_result;
    return;
  }

  try
  {
    // Getting the default servant without one set would give a NoServant exception
    CORBA::servant_reference<PortableServer::Servant> servant =
        _poa->get_servant ();
    X11_UNUSED_ARG(servant);
  }
  catch (const PortableServer::POA::NoServant&)
  {
    TAOX11_TEST_INFO
        << "Call get_servant -> Caught POA::NoServant exception as expected."
        << std::endl;
    return;
  }
  catch (const CORBA::Exception&)
  {
  }

  TAOX11_TEST_ERROR << "ERROR, get servant without one set failed"
      << std::endl;
  ++_result;
}

int
main(int argc, char *argv[])
{
  try
  {
    // Initialize the ORB.
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    // Obtain the RootPOA.
    IDL::traits<CORBA::Object>::ref_type root_poa_object =
        orb->resolve_initial_references ("RootPOA");

    // Narrow to POA.
    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (root_poa_object);

    // Get the POAManager of the RootPOA.
    IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

    // Policies for the -new- POA.
    CORBA::PolicyList policies (3);

    // Request Processing Policy.
    IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type rp_pol =
        root_poa->create_request_processing_policy (
            PortableServer::RequestProcessingPolicyValue::USE_DEFAULT_SERVANT);
    policies[0] = rp_pol;

    // Id Uniqueness Policy.
    IDL::traits<PortableServer::IdUniquenessPolicy>::ref_type iu_pol =
        root_poa->create_id_uniqueness_policy (
            PortableServer::IdUniquenessPolicyValue::MULTIPLE_ID);
    policies[1] = iu_pol;

    // Servant Retention Policy.
    IDL::traits<PortableServer::ServantRetentionPolicy>::ref_type srp_pol =
        root_poa->create_servant_retention_policy (
            PortableServer::ServantRetentionPolicyValue::NON_RETAIN);
    policies[2] = srp_pol;

    // Create POA to host default servant.
    std::string name = "Default Servant";
    IDL::traits<PortableServer::POA>::ref_type default_servant_poa =
        root_poa->create_POA (name.c_str (), poa_manager, policies);

    // Destroy policies.
    for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
      _pol->destroy ();

    // Activate POA manager.
    poa_manager->activate ();

    // Test servant.
    CORBA::servant_traits<test>::ref_type test =
        CORBA::make_reference<test_i> ();

    test_get_servant_with_no_set (default_servant_poa.weak_reference ());

    test_get_servant_manager (default_servant_poa.weak_reference ());

    test_set_servant_manager (default_servant_poa.weak_reference ());

    // Register default servant.
    default_servant_poa->set_servant (test);

    // Create dummy id.
    PortableServer::ObjectId id = PortableServer::string_to_ObjectId ("id");

    // Create dummy object.
    IDL::traits<CORBA::Object>::ref_type object =
        default_servant_poa->create_reference ("IDL:test:1.0");

    // Invoke id_to_servant(). Should retrieve default servant.
    CORBA::servant_reference<PortableServer::Servant> servant =
        default_servant_poa->id_to_servant (id);

    // Invoke reference_to_servant(). Should retrieve default servant.
    servant = default_servant_poa->reference_to_servant (object);
    // Report result.
    if (servant)
    {
      TAOX11_TEST_INFO << "Call reference_to_servant() test successful"
          << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR
          << "ERROR : Return value of reference_to_servant seems nil."
          << std::endl;
      return ++_result;
    }

    CORBA::servant_reference<PortableServer::Servant> servant2 =
        default_servant_poa->get_servant ();
    if (servant2)
    {
      TAOX11_TEST_INFO << "Call get_servant() successful" << std::endl;
    }
    else
    {
      TAOX11_TEST_ERROR << "ERROR : Return value of get_servant seems nil."
          << std::endl;
      return ++_result;
    }

    // Invoke reference_to_id(). Should throw WrongAdapter exception.
    TAOX11_TEST_INFO << "Call reference_to_id ";
    try
    {
      PortableServer::ObjectId id2 = root_poa->reference_to_id (object);
      TAOX11_TEST_ERROR << "ERROR: Exception expected." << std::endl;
      return ++_result;
    }
    catch (const PortableServer::POA::WrongAdapter&)
    {
      TAOX11_TEST_INFO
          << "-> Caught POA::WrongAdapter exception as expected."
          << std::endl;
    }

    // Destroy the ORB.
    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR: Exception caught in main : " << ex
        << std::endl;
    return ++_result;
  }

  return _result;
}
