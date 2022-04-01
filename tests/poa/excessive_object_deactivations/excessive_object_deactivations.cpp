/**
 * @file    excessive_object_deactivations.cpp
 * @author  Johnny Willemsen
 *
 * @brief   Test object deactivation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testS.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/portable_server/portableserver_functions.h"

int result {};

class test_i final : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  test_i (IDL::traits<PortableServer::POA>::ref_type& poa, PortableServer::ObjectId& id)
  : poa_ (std::move(poa))
  , id_ (id)
  {
  }

  ~test_i () = default;

  void deactivate_self () override;

  IDL::traits<PortableServer::POA>::ref_type _default_POA () override;

  IDL::traits<PortableServer::POA>::ref_type& poa_;
  PortableServer::ObjectId& id_;
};

IDL::traits<PortableServer::POA>::ref_type
test_i::_default_POA ()
{
  return this->poa_;
}

void
test_i::deactivate_self ()
{
  TAOX11_TEST_DEBUG << "Testing deactivate self" << std::endl;

  this->poa_->deactivate_object (this->id_);

  // Exception flag
  bool expected_exception_raised = false;

  try
    {
      this->poa_->deactivate_object (this->id_);
    }
  catch (const PortableServer::POA::ObjectNotActive& )
    {
      // This is the correct exception! Ignore
      expected_exception_raised = true;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Exception: " << ex << std::endl;
    }

  if (!expected_exception_raised)
    {
      ++result;
      TAOX11_TEST_ERROR << "ERROR: Expected exception not raised" << std::endl;
    }
}

void
test_object_deactivation (IDL::traits<PortableServer::POA>::ref_type poa,
                          const PortableServer::ObjectId &id)
{
  TAOX11_TEST_DEBUG << "Testing deactivation" << std::endl;

  bool expected_exception_raised = false;
  IDL::traits<PortableServer::POA>::ref_type servant_poa;
  PortableServer::ObjectId servant_id;

  CORBA::servant_traits<Test::Hello>::ref_type servant =
    CORBA::make_reference<test_i> (servant_poa, servant_id);

  PortableServer::ObjectId invalid_id =
    PortableServer::string_to_ObjectId ("invalid id");

  try
    {
      poa->deactivate_object (invalid_id);
    }
  catch (const PortableServer::POA::ObjectNotActive& )
    {
      // This is the correct exception! Ignore
      expected_exception_raised = true;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Exception : " << ex << std::endl;
    }

  // Make sure an exception was raised and it was of the correct
  // type.
  if (!expected_exception_raised)
    {
      ++result;
      TAOX11_TEST_ERROR << "ERROR: Expected exception not raised" << std::endl;
    }

  poa->activate_object_with_id (id, servant);

  poa->deactivate_object (id);

  // Reset flag
  expected_exception_raised = false;

  try
    {
      poa->deactivate_object (id);
    }
  catch (const PortableServer::POA::ObjectNotActive& )
    {
      // This is the correct exception! Ignore
      expected_exception_raised = 1;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Exception : " << ex << std::endl;
    }

  // Make sure an exception was raised and it was of the correct
  // type.
  if (!expected_exception_raised)
    {
      ++result;
      TAOX11_TEST_ERROR << "ERROR: Expected exception not raised" << std::endl;
    }

  poa->activate_object_with_id (id, servant);

  servant_poa = poa;
  servant_id = id;

  IDL::traits<CORBA::Object>::ref_type object = poa->id_to_reference (id);

  IDL::traits<Test::Hello>::ref_type test =
    IDL::traits<Test::Hello>::narrow (object);

  test->deactivate_self ();
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB first.
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA_var object from Object_var.
      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      // Get the POAManager of the RootPOA.
      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      CORBA::PolicyList empty_policies;
      IDL::traits<PortableServer::POA>::ref_type child_poa =
        root_poa->create_POA ("child",
                              poa_manager,
                              empty_policies);

      poa_manager->activate ();

      obj = root_poa->create_reference ("IDL:Test/Hello:1.0");

      PortableServer::ObjectId id =
        root_poa->reference_to_id (obj);

      test_object_deactivation (root_poa, id);

      id = PortableServer::string_to_ObjectId ("good id");

      test_object_deactivation (child_poa, id);

      orb->destroy ();
    }
  catch (const std::exception& ex)
    {
      TAOX11_TEST_ERROR << "Exception! " << ex << std::endl;
      return 1;
    }

  return result;
}
