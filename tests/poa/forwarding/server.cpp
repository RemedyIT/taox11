/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA forwarding test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"
#include "test_i.h"
#include "servant_activator.h"
#include <fstream>

static std::string ior_output_file ("");
static std::string forward_to_ior ("");

static bool
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:o:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
      case 'f':
        forward_to_ior = get_opts.opt_arg ();
        break;

      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << std::endl << "usage:  " << std::endl << "-f forward to IOR"
            << std::endl << "-o output file for IOR" << std::endl;
        return false;
    }

  if (ior_output_file.length() == 0)
  {
    TAOX11_TEST_ERROR << "output IOR file not specified" << std::endl;
    return false;
  }

  // Indicates successful parsing of command line.
  return true;
}

IDL::traits<PortableServer::POA>::ref_type
setup_poa (IDL::traits<PortableServer::POA>::ref_type root_poa)
{
  // Policies for the childPOA to be created.
  CORBA::PolicyList policies (2);

  // Tell the POA to use a servant manager.
  IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type rp_pol =
    root_poa->create_request_processing_policy (
      PortableServer::RequestProcessingPolicyValue::USE_SERVANT_MANAGER);
  policies[0] = rp_pol;

  // Allow implicit activation.
  IDL::traits<PortableServer::ImplicitActivationPolicy>::ref_type ia_pol =
    root_poa->create_implicit_activation_policy (
      PortableServer::ImplicitActivationPolicyValue::IMPLICIT_ACTIVATION);
  policies[1] = ia_pol;

  IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
    root_poa->the_POAManager ();

  // Create POA as child of RootPOA with the above policies.  This POA
  // will use a SERVANT_ACTIVATOR because of RETAIN policy.
  IDL::traits<PortableServer::POA>::ref_type child_poa =
    root_poa->create_POA ("childPOA",
                          poa_manager,
                          policies);

  // Creation of childPOAs is over. Destroy the Policy objects.
  for (IDL::traits<CORBA::Policy>::ref_type& _pol : policies)
    {
      _pol->destroy ();
    }

  return child_poa;
}

IDL::traits<PortableServer::ServantActivator>::ref_type
create_servant_manager (IDL::traits<CORBA::ORB>::ref_type orb,
                        IDL::traits<PortableServer::POA>::ref_type child_poa)
{
  IDL::traits<CORBA::Object>::ref_type forward_to;
  if (!forward_to_ior.empty())
  {
    TAOX11_TEST_DEBUG << "Creating forward object, using IOR <"
              << forward_to_ior << ">" << std::endl;
    forward_to = orb->string_to_object (forward_to_ior);
  }

  IDL::traits<PortableServer::ServantActivator>::ref_type servant_activator =
      CORBA::make_reference<ServantActivator> (orb, forward_to);

  child_poa->set_servant_manager (servant_activator);
  IDL::traits<PortableServer::ServantManager>::ref_type got_servant_manager =
    child_poa->get_servant_manager ();
  if (got_servant_manager == nullptr)
  {
    TAOX11_TEST_ERROR << "Get_servant_manager returned nil." << std::endl;
    return nullptr;
  }

  CORBA::servant_traits<test>::ref_type servant =
      CORBA::make_reference<test_i> (orb,
                  child_poa,
                  servant_activator.weak_reference (),
                  127);

  PortableServer::ObjectId id = child_poa->activate_object (servant);
  IDL::traits<CORBA::Object>::ref_type test_obj = child_poa->id_to_reference (id);
  IDL::traits<test>::ref_type test_impl = IDL::traits<test>::narrow (test_obj);

  std::string ior = orb->object_to_string (test_impl);

  filebuf fb1;
  fb1.open (ior_output_file, ios::out);
  ostream out_file(std::addressof(fb1));
  out_file << ior;

  if (out_file.bad ())
  {
    TAOX11_TEST_ERROR << "failed to write " << ior << " to "
      <<ior_output_file << std::endl;
    return nullptr;
  }
  fb1.close ();

  return servant_activator;
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

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      // Get the POAManager of the RootPOA.
      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      IDL::traits<PortableServer::POA>::ref_type child_poa =
        setup_poa (root_poa);

      IDL::traits<PortableServer::ServantActivator>::ref_type manager =
        create_servant_manager (orb, child_poa);
      if (!manager)
        return 1;
      poa_manager->activate ();

      orb->run ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Error <server>: Unexpected exception caught : ";
      TAOX11_TEST_ERROR << ex;
      return 1;
    }

  return 0;
}
