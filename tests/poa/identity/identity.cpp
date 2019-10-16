/**
 * @file    identity.cpp
 * @author  Mark Drijver
 *
 * @brief   Test move between id, reference, and servants.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testS.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/system_exception.h"
#include "tao/x11/portable_server/portableserver_functions.h"

class test_i : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:

  test_i(IDL::traits<PortableServer::POA>::ref_type poa) :
      poa_ (std::move(poa))
  {
  }

  virtual IDL::traits<PortableServer::POA>::ref_type
  _default_POA() override
  {
    return poa_;
  }

protected:
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

void
create_poas(IDL::traits<PortableServer::POA>::ref_type root_poa,
    PortableServer::LifespanPolicyValue lifespan_policy,
    IDL::traits<PortableServer::POA>::ref_type& first_poa,
    IDL::traits<PortableServer::POA>::ref_type& second_poa,
    IDL::traits<PortableServer::POA>::ref_type& third_poa,
    IDL::traits<PortableServer::POA>::ref_type& forth_poa)
{
  // Policies for the _new POAs
  CORBA::PolicyList policies (3);
  policies[0] = root_poa->create_lifespan_policy (lifespan_policy);
  policies[1] = root_poa->create_id_uniqueness_policy (
      PortableServer::IdUniquenessPolicyValue::MULTIPLE_ID);
  policies[2] = root_poa->create_id_assignment_policy (
      PortableServer::IdAssignmentPolicyValue::SYSTEM_ID);

  // Creation of the firstPOA
  std::string name = "firstPOA";
  first_poa = root_poa->create_POA (name, nullptr, policies);

  // Creation of the secondPOA
  policies[1]->destroy ();
  policies[1] = root_poa->create_id_uniqueness_policy (
      PortableServer::IdUniquenessPolicyValue::UNIQUE_ID);
  name = "secondPOA";
  second_poa = root_poa->create_POA (name, nullptr, policies);

  policies[2]->destroy ();
  policies[2] = root_poa->create_id_assignment_policy (
      PortableServer::IdAssignmentPolicyValue::USER_ID);

  // Creation of the thirdPOA
  name = "thirdPOA";
  third_poa = root_poa->create_POA (name, nullptr, policies);

  policies[1]->destroy ();
  policies[1] = root_poa->create_id_uniqueness_policy (
      PortableServer::IdUniquenessPolicyValue::MULTIPLE_ID);

  // Creation of the forthPOA
  name = "forthPOA";
  forth_poa = root_poa->create_POA (name, nullptr, policies);

  // Creation of the nevv POAs over, so destroy the policies
  for (IDL::traits<CORBA::Policy>::ref_type policy : policies)
  {
    policy->destroy();
  }
}

bool
test_poas1(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type root_poa)
{
  TAOX11_TEST_DEBUG << "test_poas 1" << std::endl;
  bool perform_deactivation_test = true;

  {
    CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
        CORBA::make_reference<test_i> (root_poa);

    IDL::traits<CORBA::Object>::ref_type obj = root_poa->create_reference (
        "IDL:test:1.0");
    std::string string = orb->object_to_string (obj);
    TAOX11_TEST_DEBUG << string << std::endl;

    PortableServer::ObjectId id = root_poa->reference_to_id (obj);

    root_poa->activate_object_with_id (id, servant_ref);

    obj = root_poa->id_to_reference (id);
    string = orb->object_to_string (obj);
    TAOX11_TEST_DEBUG << string << std::endl;

    CORBA::servant_reference<PortableServer::Servant> servant_from_reference =
        root_poa->reference_to_servant (obj);

    CORBA::servant_reference<PortableServer::Servant> servant_from_id = root_poa->id_to_servant (
        id);

    // Compare servants
    PortableServer::ObjectId servant_from_reference_id =
        root_poa->servant_to_id (servant_from_reference);
    PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
        servant_ref);
    PortableServer::ObjectId servant_from_id_id = root_poa->servant_to_id (
        servant_from_id);
    TAOX11_TEST_DEBUG
        << "Print servant_from_reference, servant_from_id and servant_ref: "
        << servant_from_reference_id << servant_from_id_id << servant_ref_id
        << std::endl;
    if (servant_from_reference_id != id
        || servant_from_reference_id != servant_ref_id)
    {
      TAOX11_TEST_ERROR
          << "Mismatched servant_from_reference, servant_from_id and servant_ref\n";
      return false;
    }

    obj = root_poa->servant_to_reference (servant_ref);
    string = orb->object_to_string (obj);
    TAOX11_TEST_DEBUG << string << std::endl;

    // Print id
    TAOX11_TEST_DEBUG << "servant_ref_id: " << servant_ref_id << std::endl;

    PortableServer::ObjectId id_from_servant = root_poa->servant_to_id (
        servant_ref);

    if (id_from_servant != id)
    {
      TAOX11_TEST_ERROR << "Assert failed: id_from_servant == id" << std::endl;
      return false;
    }
    root_poa->deactivate_object (id);

    if (perform_deactivation_test)
    {
      root_poa->activate_object_with_id (id, servant_ref);

      servant_from_reference = root_poa->reference_to_servant (obj);

      // Compare servants
      PortableServer::ObjectId servant_from_reference_id =
          root_poa->servant_to_id (servant_from_reference);
      PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
          servant_ref);
      if (servant_from_reference_id != servant_ref_id)
      {
        TAOX11_TEST_ERROR << "Assert failed: servant_from_reference == servant"
            << std::endl;
        return false;
      }

      root_poa->deactivate_object (id);
    }
  }

  {
    CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
        CORBA::make_reference<test_i> (root_poa);

    PortableServer::ObjectId id = root_poa->activate_object (servant_ref);

    IDL::traits<CORBA::Object>::ref_type obj = root_poa->id_to_reference (id);
    std::string string = orb->object_to_string (obj);
    TAOX11_TEST_DEBUG << string << std::endl;

    obj = root_poa->create_reference_with_id (id, "IDL:test:1.0");
    string = orb->object_to_string (obj);
    TAOX11_TEST_DEBUG << string << std::endl;

    root_poa->deactivate_object (id);
  }
  return true;
}

bool
test_poas2(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type root_poa,
    IDL::traits<PortableServer::POA>::ref_type first_poa,
    IDL::traits<PortableServer::POA>::ref_type second_poa,
    IDL::traits<PortableServer::POA>::ref_type third_poa)
{
  try
  {
    TAOX11_TEST_DEBUG << "test_poas 2" << std::endl;
    bool perform_deactivation_test = true;
    {
      CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
          CORBA::make_reference<test_i> (root_poa);

      IDL::traits<CORBA::Object>::ref_type obj;
      try
      {
        obj = first_poa->create_reference ("IDL:test:1.0");
        std::string string = orb->object_to_string (obj);
        TAOX11_TEST_DEBUG << string << std::endl;
      }
      catch (const CORBA::SystemException& ex)
      {
        TAOX11_TEST_DEBUG << "SystemException : " << ex << std::endl;
      }

      PortableServer::ObjectId id = first_poa->reference_to_id (obj);

      first_poa->activate_object_with_id (id, servant_ref);

      obj = first_poa->id_to_reference (id);
      std::string string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      CORBA::servant_reference<PortableServer::Servant> servant_from_reference =
          first_poa->reference_to_servant (obj);

      CORBA::servant_reference<PortableServer::Servant> servant_from_id = first_poa->id_to_servant (
          id);

      // Compare servants
      PortableServer::ObjectId servant_from_reference_id =
          root_poa->servant_to_id (servant_from_reference);
      PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
          servant_ref);
      PortableServer::ObjectId servant_from_id_id = root_poa->servant_to_id (
          servant_from_id);
      TAOX11_TEST_DEBUG
          << "Print servant_from_reference, servant_from_id and servant_ref: "
          << servant_from_reference_id << servant_from_id_id << servant_ref_id
          << std::endl;
      if (servant_from_reference_id != servant_from_id_id
          || servant_from_reference_id != servant_ref_id)
      {
        TAOX11_TEST_ERROR
            << "Mismatched servant_from_reference, servant_from_id and servant_ref: "
            << servant_from_reference_id << id << servant_ref_id << std::endl;
        return false;
      }

      first_poa->deactivate_object (id);

      if (perform_deactivation_test)
      {
        first_poa->activate_object_with_id (id, servant_ref);

        servant_from_reference = first_poa->reference_to_servant (obj);
        // Compare servants
        PortableServer::ObjectId servant_from_reference_id =
            root_poa->servant_to_id (servant_from_reference);
        PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
            servant_ref);
        if (servant_from_reference_id != servant_ref_id)
        {
          TAOX11_TEST_ERROR << "Assert failed: servant_from_reference == servant"
              << std::endl;
          return false;
        }

        first_poa->deactivate_object (id);
      }
    }

    {
      CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
          CORBA::make_reference<test_i> (root_poa);

      PortableServer::ObjectId id = first_poa->activate_object (servant_ref);

      IDL::traits<CORBA::Object>::ref_type obj = first_poa->id_to_reference (
          id);
      std::string string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      obj = first_poa->create_reference_with_id (id, "IDL:test:1.0");
      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      first_poa->deactivate_object (id);
    }

    {
      CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
          CORBA::make_reference<test_i> (root_poa);

      IDL::traits<CORBA::Object>::ref_type obj =
          second_poa->create_reference ("IDL:test:1.0");

      std::string string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      PortableServer::ObjectId id = second_poa->reference_to_id (obj);

      second_poa->activate_object_with_id (id, servant_ref);

      obj = second_poa->id_to_reference (id);

      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      CORBA::servant_reference<PortableServer::Servant> servant_from_reference =
          second_poa->reference_to_servant (obj);

      CORBA::servant_reference<PortableServer::Servant> servant_from_id =
          second_poa->id_to_servant (id);

      PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
          servant_ref);
      // Print id
      TAOX11_TEST_DEBUG << "servant_ref_id: " << servant_ref_id << std::endl;

      // Compare servants
      PortableServer::ObjectId servant_from_reference_id =
          root_poa->servant_to_id (servant_from_reference);
      servant_ref_id = root_poa->servant_to_id (servant_ref);
      PortableServer::ObjectId servant_from_id_id = root_poa->servant_to_id (
          servant_from_id);
      TAOX11_TEST_DEBUG
          << "Print servant_from_reference, servant_from_id and servant_ref: "
          << servant_from_reference_id << servant_from_id_id << servant_ref_id
          << std::endl;
      if (servant_from_reference_id != servant_from_id_id
          || servant_from_reference_id != servant_ref_id)
      {
        TAOX11_TEST_ERROR
            << "Mismatched servant_from_reference, servant_from_id and servant_ref: "
            << servant_from_reference_id << id << servant_ref_id << std::endl;
        return false;
      }

      obj = second_poa->servant_to_reference (servant_ref);
      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      second_poa->deactivate_object (id);

      if (perform_deactivation_test)
      {
        second_poa->activate_object_with_id (id, servant_ref);

        servant_from_reference = second_poa->reference_to_servant (obj);
        // Compare servants
        PortableServer::ObjectId servant_from_reference_id =
            root_poa->servant_to_id (servant_from_reference);
        PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
            servant_ref);
        if (servant_from_reference_id != servant_ref_id)
        {
          TAOX11_TEST_ERROR << "Assert failed: servant_from_reference == servant"
              << std::endl;
          return false;
        }

        second_poa->deactivate_object (id);
      }
    }

    {
      CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
          CORBA::make_reference<test_i> (second_poa);

      PortableServer::ObjectId id = second_poa->activate_object (servant_ref);

      IDL::traits<CORBA::Object>::ref_type obj = second_poa->id_to_reference (
          id);
      std::string string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      obj = second_poa->create_reference_with_id (id, "IDL:test:1.0");
      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      second_poa->deactivate_object (id);
    }

    {
      CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
          CORBA::make_reference<test_i> (third_poa);
      PortableServer::ObjectId id = PortableServer::string_to_ObjectId ("hello");
      TAOX11_TEST_DEBUG << "string_to_ObjectId: hello => " << id << std::endl;

      IDL::traits<CORBA::Object>::ref_type obj =
          third_poa->create_reference_with_id (id, "IDL:test:1.0");
      std::string string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      PortableServer::ObjectId id_from_reference = third_poa->reference_to_id (
          obj);

      string = PortableServer::ObjectId_to_string (id_from_reference);
      TAOX11_TEST_DEBUG << string << std::endl;

      if (id_from_reference != id)
        {
          TAOX11_TEST_ERROR << "Assertion id_from_reference == id failed: "
            << id_from_reference << " <> " << id << std::endl;
          return false;
        }

      third_poa->activate_object_with_id (id, servant_ref);

      obj = third_poa->id_to_reference (id);
      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      CORBA::servant_reference<PortableServer::Servant> servant_from_reference =
          third_poa->reference_to_servant (obj);

      CORBA::servant_reference<PortableServer::Servant> servant_from_id = third_poa->id_to_servant (
          id);

      // Compare servants
      PortableServer::ObjectId servant_from_id_id = root_poa->servant_to_id (
          servant_from_id);
      PortableServer::ObjectId servant_from_reference_id =
          root_poa->servant_to_id (servant_from_reference);
      PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
          servant_ref);
      TAOX11_TEST_DEBUG
          << "Print servant_from_reference, servant_from_id and servant_ref: "
          << servant_from_reference_id << servant_from_id_id << servant_ref_id
          << std::endl;
      if (servant_from_reference_id != servant_from_id_id
          || servant_from_reference_id != servant_ref_id)
        {
          TAOX11_TEST_ERROR
            << "Mismatched servant_from_reference, servant_from_id and servant_ref: "
            << servant_from_reference_id << servant_from_id_id << servant_ref_id
            << std::endl;
          return false;
        }

      obj = third_poa->servant_to_reference (servant_ref);
      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      PortableServer::ObjectId id_from_servant = third_poa->servant_to_id (
          servant_ref);

      string = PortableServer::ObjectId_to_string (id_from_servant);
      TAOX11_TEST_DEBUG << string << std::endl;

      if (id_from_servant != id)
        {
          TAOX11_TEST_ERROR << "Assertion id_from_servant == id failed." << std::endl;
          return false;
        }

      third_poa->deactivate_object (id);

      if (perform_deactivation_test)
      {
        third_poa->activate_object_with_id (id, servant_ref);

        servant_from_reference = third_poa->reference_to_servant (obj);

        PortableServer::ObjectId servant_from_reference_id =
            root_poa->servant_to_id (servant_from_reference);
        PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
            servant_ref);
        if (servant_from_reference_id != servant_ref_id)
        {
          TAOX11_TEST_ERROR << "Assert failed: servant_from_reference == servant: "
              << servant_from_reference_id << servant_ref_id << std::endl;
          return false;
        }

        third_poa->deactivate_object (id);
      }
    }
  }
  catch (const CORBA::SystemException& ex)
  {
    TAOX11_TEST_DEBUG << "SystemException : " << ex << std::endl;
    throw;
  }
  catch (const PortableServer::POA::WrongPolicy& ex)
  {
    TAOX11_TEST_DEBUG << "WrongPolicy exception : " << ex << std::endl;
    throw;
  }

  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_DEBUG << "CORBA::Exception : " << ex << std::endl;
    throw;
  }

  return true;
}

bool
test_poas4(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type root_poa,
    IDL::traits<PortableServer::POA>::ref_type forth_poa)
{
  TAOX11_TEST_DEBUG << "test_poas 4" << std::endl;
  bool perform_deactivation_test = true;
  try
  {
    {
      CORBA::servant_traits<Test::Hello>::ref_type servant_ref =
          CORBA::make_reference<test_i> (forth_poa);

      PortableServer::ObjectId id = PortableServer::string_to_ObjectId ("hello");

      IDL::traits<CORBA::Object>::ref_type obj =
          forth_poa->create_reference_with_id (id, "IDL:test:1.0");

      std::string string = orb->object_to_string (obj);

      TAOX11_TEST_DEBUG << string << std::endl;

      PortableServer::ObjectId id_from_reference = forth_poa->reference_to_id (
          obj);
      string = PortableServer::ObjectId_to_string (id_from_reference);
      TAOX11_TEST_DEBUG << string << std::endl;

      if (id_from_reference != id)
      {
        TAOX11_TEST_ERROR << "Assert failed: id_from_reference == id" << std::endl;
        return false;
      }

      forth_poa->activate_object_with_id (id, servant_ref);

      obj = forth_poa->id_to_reference (id);
      string = orb->object_to_string (obj);
      TAOX11_TEST_DEBUG << string << std::endl;

      CORBA::servant_reference<PortableServer::Servant> servant_from_reference =
          forth_poa->reference_to_servant (obj);

      CORBA::servant_reference<PortableServer::Servant> servant_from_id = forth_poa->id_to_servant (
          id);

      // Compare servants
      PortableServer::ObjectId servant_from_id_id = root_poa->servant_to_id (
          servant_from_id);
      PortableServer::ObjectId servant_from_reference_id =
          root_poa->servant_to_id (servant_from_reference);
      PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
          servant_ref);
      TAOX11_TEST_DEBUG
          << "Print servant_from_reference, servant_from_id and servant_ref: "
          << servant_from_reference_id << servant_from_id_id << servant_ref_id
          << std::endl;
      if (servant_from_reference_id != servant_from_id_id
          || servant_from_reference_id != servant_ref_id)
      {
        TAOX11_TEST_ERROR
            << "Mismatched servant_from_reference, servant_from_id and servant_ref: "
            << servant_from_reference_id << servant_from_id_id << servant_ref_id
            << std::endl;
        return false;
      }

      forth_poa->deactivate_object (id);

      if (perform_deactivation_test)
      {
        forth_poa->activate_object_with_id (id, servant_ref);

        servant_from_reference = forth_poa->reference_to_servant (obj);

        PortableServer::ObjectId servant_from_reference_id =
            root_poa->servant_to_id (servant_from_reference);
        PortableServer::ObjectId servant_ref_id = root_poa->servant_to_id (
            servant_ref);
        if (servant_from_reference_id != servant_ref_id)
        {
          TAOX11_TEST_ERROR << "Assert failed: servant_from_reference == servant: "
              << servant_from_reference_id << servant_ref_id << std::endl;
          return false;
        }

        forth_poa->deactivate_object (id);
      }
    }
    return true;
  }
  catch (const CORBA::SystemException& ex)
  {
    TAOX11_TEST_DEBUG << "SystemException : " << ex << std::endl;
    throw;
  }
  catch (const PortableServer::POA::WrongPolicy& ex)
  {
    TAOX11_TEST_DEBUG << "WrongPolicy exception : " << ex << std::endl;
    throw;
  }

  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_DEBUG << "CORBA::Exception : " << ex << std::endl;
    throw;
  }
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    // Obtain the RootPOA.
    IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

    IDL::traits<PortableServer::POA>::ref_type first_poa;
    IDL::traits<PortableServer::POA>::ref_type second_poa;
    IDL::traits<PortableServer::POA>::ref_type third_poa;
    IDL::traits<PortableServer::POA>::ref_type forth_poa;

    create_poas (root_poa, PortableServer::LifespanPolicyValue::TRANSIENT,
        first_poa, second_poa, third_poa, forth_poa);

    if (!test_poas1 (orb, root_poa))
      return 1;
    if (!test_poas2 (orb, root_poa, first_poa, second_poa, third_poa))
      return 1;
    if (!test_poas4 (orb, root_poa, forth_poa))
      return 1;

    first_poa->destroy (true, true);
    second_poa->destroy (true, true);
    third_poa->destroy (true, true);
    forth_poa->destroy (true, true);

    create_poas (root_poa, PortableServer::LifespanPolicyValue::PERSISTENT,
        first_poa, second_poa, third_poa, forth_poa);

    if (!test_poas1 (orb, root_poa))
      return 1;
    if (!test_poas2 (orb, root_poa, first_poa, second_poa, third_poa))
      return 1;
    if (!test_poas4 (orb, root_poa, forth_poa))
      return 1;

    root_poa->destroy (true, true);

    orb->destroy ();
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception! " << ex.what () << std::endl;
    return 1;
  }

  return 0;
}
