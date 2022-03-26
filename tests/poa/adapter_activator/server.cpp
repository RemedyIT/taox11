/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "ace/Get_Opt.h"
#include "test_i.h"
#include "tao/x11/portable_server/AdapterActivatorC.h"
#include "tao/x11/portable_server/portableserver_functions.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

class MyAdapter_Activator final
  : public virtual IDL::traits<PortableServer::AdapterActivator>::base_type
{
public:
  MyAdapter_Activator (IDL::traits<PortableServer::POAManager>::ref_type poa_manager,
                       IDL::traits<CORBA::ORB>::ref_type orb,
                       CORBA::PolicyList& first_policies,
                       CORBA::PolicyList& second_policies);

  bool unknown_adapter (IDL::traits<PortableServer::POA>::ref_type parent,
                        const std::string& name) override;

private:
  IDL::traits<PortableServer::POAManager>::ref_type poa_manager_;
  IDL::traits<CORBA::ORB>::ref_type orb_;
  CORBA::PolicyList& first_poa_policies_;
  CORBA::PolicyList& second_poa_policies_;
};

MyAdapter_Activator::MyAdapter_Activator (IDL::traits<PortableServer::POAManager>::ref_type poa_manager,
                                          IDL::traits<CORBA::ORB>::ref_type orb,
                                          CORBA::PolicyList& first_policies,
                                          CORBA::PolicyList& second_policies)
  : poa_manager_ (poa_manager)
  , orb_ (std::move(orb))
  , first_poa_policies_(first_policies)
  , second_poa_policies_(second_policies)
{
}

bool
MyAdapter_Activator::unknown_adapter (IDL::traits<PortableServer::POA>::ref_type parent,
                                      const std::string& name)
{
  TAOX11_TEST_DEBUG << "Invoking unknown_adapter for <"
            << name << ">"
            << std::endl;

  if (name == "firstPOA")
    {
      IDL::traits<PortableServer::POA>::ref_type child =
        parent->create_POA (name,
                            this->poa_manager_,
                            this->first_poa_policies_);

      // Creation of firstPOA is over. Destroy the Policy objects.
      for (IDL::traits<CORBA::Policy>::ref_type _pol :
          this->first_poa_policies_)
        {
          _pol->destroy ();
        }
      IDL::traits<PortableServer::AdapterActivator>::ref_type aa = parent->the_activator ();
      if (aa)
        {
          child->the_activator (aa);
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unable to retrieve Adapter Activator from parent POA." << std::endl;
          child->destroy (true, true);
          return false;
        }

      CORBA::servant_traits<test>::ref_type servant =
          CORBA::make_reference<test_i> (this->orb_, child);

      child->set_servant (servant);
    }
  else if (name == "secondPOA")
    {
      IDL::traits<PortableServer::POA>::ref_type child =
        parent->create_POA (name,
                            this->poa_manager_,
                            this->second_poa_policies_);

      // Creation of secondPOA is over. Destroy the Policy objects.
      for (IDL::traits<CORBA::Policy>::ref_type _pol :
          this->second_poa_policies_)
        {
          _pol->destroy ();
        }

      CORBA::servant_traits<test>::ref_type servant =
          CORBA::make_reference<test_i> (this->orb_, child);

      PortableServer::ObjectId oid =
        PortableServer::string_to_ObjectId ("third test");

      child->activate_object_with_id (oid, servant);
    }
  else
    {
      TAOX11_TEST_DEBUG << "Unknown POA : "
                << name << std::endl;
      return false;
    }
  return true;
}

std::string ior_output_file = "ior";

static bool
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
      case 'f':
        ior_output_file = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:  [-f ior_output_file]" << std::endl;
        return false;
    }

  return true;
}

static bool
write_iors_to_file (const std::string& first_ior,
                    const std::string& second_ior,
                    const std::string& third_ior)
{
  const std::string& ior_output_file_1 = ior_output_file + "_1";
  const std::string& ior_output_file_2 = ior_output_file + "_2";
  const std::string& ior_output_file_3 = ior_output_file + "_3";

  filebuf fb1, fb2, fb3;
  fb1.open (ior_output_file_1, ios::out);
  fb2.open (ior_output_file_2, ios::out);
  fb3.open (ior_output_file_3, ios::out);

  ostream output_file_1(std::addressof(fb1));
  ostream output_file_2(std::addressof(fb2));
  ostream output_file_3(std::addressof(fb3));

  if (output_file_1.bad() ||
      output_file_2.bad() ||
      output_file_3.bad())
  {
    TAOX11_TEST_ERROR << "Cannot open output files for writing IORs: "
              << ior_output_file_1 << ", "
              << ior_output_file_2 << ", "
              << ior_output_file_3 << std::endl;
    return false;
  }

  output_file_1 << first_ior;

  if (output_file_1.bad ())
  {
    TAOX11_TEST_ERROR << "failed to write "
              << first_ior << " to "
              <<ior_output_file_1 << std::endl;
    return false;
  }
  fb1.close ();

  output_file_2 << second_ior;
  if (output_file_2.bad ())
  {
    TAOX11_TEST_ERROR << "failed to write "
              << second_ior << " to "
              << ior_output_file_2 << std::endl;
    return false;
  }
  fb2.close ();

  output_file_3 << third_ior;
  if (output_file_3.bad ())
  {
    TAOX11_TEST_ERROR << "failed to write "
              << third_ior << " to "
              << ior_output_file_3 << std::endl;
    return false;
  }
  fb3.close ();

  return true;
}

int
main (int argc, char *argv[])
{

  try
    {
      // Initialize the ORB first.
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      // Obtain the RootPOA.
      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

      // Get the POA object from Object.
      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      // Get the POAManager of the RootPOA.
      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      IDL::traits<PortableServer::POA>::ref_type first_poa;
      IDL::traits<PortableServer::POA>::ref_type second_poa;

      CORBA::PolicyList first_policies;
      first_policies.resize (4);

      // Id Assignment Policy
      IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type id_pol =
        root_poa->create_id_assignment_policy (
          PortableServer::IdAssignmentPolicyValue::SYSTEM_ID);
      first_policies[0] = id_pol;

      IDL::traits<PortableServer::LifespanPolicy>::ref_type ls_pol =
        root_poa->create_lifespan_policy (
          PortableServer::LifespanPolicyValue::PERSISTENT);
      first_policies[1] = ls_pol;

      // Request Processing policy
      IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type rp_pol =
        root_poa->create_request_processing_policy (
          PortableServer::RequestProcessingPolicyValue::USE_DEFAULT_SERVANT);
      first_policies[2] = rp_pol;

      IDL::traits<PortableServer::IdUniquenessPolicy>::ref_type iu_pol =
        root_poa->create_id_uniqueness_policy (
          PortableServer::IdUniquenessPolicyValue::MULTIPLE_ID);
      first_policies[3] = iu_pol;

      // Create the firstPOA under the RootPOA.
      first_poa = root_poa->create_POA ("firstPOA",
                                        poa_manager,
                                        first_policies);

      CORBA::PolicyList second_policies;
      // Policies for the secondPOA to be created.
      second_policies.resize (2);

      IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type id_pol2 =
        root_poa->create_id_assignment_policy (
          PortableServer::IdAssignmentPolicyValue::USER_ID);
      second_policies[0] = id_pol2;

      // Lifespan policy
      IDL::traits<PortableServer::LifespanPolicy>::ref_type ls_pol2 =
        root_poa->create_lifespan_policy (
          PortableServer::LifespanPolicyValue::PERSISTENT);
      second_policies[1] = ls_pol2;

      // Create the secondPOA under the firstPOA.
      second_poa = first_poa->create_POA ("secondPOA",
                                          poa_manager,
                                          second_policies);

      IDL::traits<PortableServer::AdapterActivator>::ref_type
        adapter_activator =
          CORBA::make_reference<MyAdapter_Activator> (poa_manager, orb,
                                                      first_policies, second_policies);

      root_poa->the_activator (adapter_activator);

      // Create a servant.
      CORBA::servant_traits<test>::ref_type first_servant =
          CORBA::make_reference<test_i> (orb, root_poa);

      PortableServer::ObjectId first_oid =
        root_poa->activate_object (first_servant);

      IDL::traits<CORBA::Object>::ref_type first_servant_obj =
        root_poa->id_to_reference (first_oid);

      // Get Object Reference for the first_servant object.
      IDL::traits<test>::ref_type first_test =
        IDL::traits<test>::narrow (first_servant_obj);

      IDL::traits<CORBA::Object>::ref_type second_test =
        first_poa->create_reference ("IDL:test:1.0");

      PortableServer::ObjectId third_oid =
        PortableServer::string_to_ObjectId ("third test");

      IDL::traits<CORBA::Object>::ref_type third_test =
        second_poa->create_reference_with_id (third_oid,
                                              "IDL:test:1.0");

      // Stringify all the object references and print them out.
      std::string const first_ior =
        orb->object_to_string (first_test);

      std::string const second_ior =
        orb->object_to_string (second_test);

      std::string const third_ior =
        orb->object_to_string (third_test);

      TAOX11_TEST_DEBUG << first_ior << std::endl
                << second_ior << std::endl
                << third_ior << std::endl;

      bool const write_result = write_iors_to_file (first_ior,
                                                    second_ior,
                                                   third_ior);
      if (!write_result)
        return 1;

      first_poa->destroy (true, true);

      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << ex;
      return 1;
    }
  return 0;
}
