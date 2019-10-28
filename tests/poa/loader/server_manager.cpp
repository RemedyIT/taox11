/**
 * @file    server_manager.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "server_manager.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/portable_server/portableserver_functions.h"

#include <fstream>

Server_i::Server_i() :
    ior_output_file_ (""), policies_ (4)
{
}

Server_i::~Server_i()
{
  this->orb_->destroy ();
}

// This method parses the input.

bool
Server_i::parse_args(int argc, char **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("f:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
    {
      case 'f':
        this->ior_output_file_ = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n" << "[-f ior_output_file]" << std::endl;
        return false;
        break;
    }

  // Indicates successful parsing of command line.
  return true;
}

// The IORs obtained are written into files for further use.

int
Server_i::write_iors_to_file(const std::string& first_ior,
    const std::string& second_ior)
{
  const std::string& ior_output_file_1 = this->ior_output_file_ + "_1";
  const std::string& ior_output_file_2 = this->ior_output_file_ + "_2";

  filebuf fb1, fb2, fb3;
  fb1.open (ior_output_file_1, ios::out);
  fb2.open (ior_output_file_2, ios::out);

  ostream output_file_1 (std::addressof(fb1));
  ostream output_file_2 (std::addressof(fb2));

  if (output_file_1.bad () || output_file_2.bad ())
    {
      TAOX11_TEST_ERROR << "Cannot open output files for writing IORs: "
          << ior_output_file_1 << ", " << ior_output_file_2 << std::endl;
      return 1;
    }

  output_file_1 << first_ior;

  if (output_file_1.bad ())
    {
      TAOX11_TEST_ERROR << "failed to write " << first_ior << " to "
          << ior_output_file_1 << std::endl;
      return 1;
    }
  fb1.close ();

  output_file_2 << second_ior;
  if (output_file_2.bad ())
    {
      TAOX11_TEST_ERROR << "failed to write " << second_ior << " to "
          << ior_output_file_2 << std::endl;
      return 1;
    }
  fb2.close ();

  return 0;
}

// Initialisation of the ORB and POA.
int
Server_i::init(int argc, char **argv)
{
  try
  {
    // Initialize the ORB.
    this->orb_ = CORBA::ORB_init (argc, argv);

    if (!parse_args (argc, argv))
      return 1;

    // Obtain the RootPOA.
    IDL::traits<CORBA::Object>::ref_type obj =
        this->orb_->resolve_initial_references ("RootPOA");

    // Narrow the Object reference to a POA reference
    this->root_poa_ = IDL::traits<PortableServer::POA>::narrow (obj);

    // Get the POAManager of RootPOA
    this->poa_manager_ = this->root_poa_->the_POAManager ();

  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Server_i::init () : " << ex << std::endl;
    return 1;
  }

  return 0;
}

// This method creates an poa with 4 policies of which the servent
// retention policy decides whether the Servant Activator or the
// Servant Locator would be used by the Servant Manager.

IDL::traits<PortableServer::POA>::ref_type
Server_i::create_poa(const std::string& name, bool servant_retention_policy)
{
  try
  {
    // ID Assignment Policy.
    IDL::traits<PortableServer::IdAssignmentPolicy>::ref_type id_pol =
        this->root_poa_->create_id_assignment_policy (
            PortableServer::IdAssignmentPolicyValue::USER_ID);
    this->policies_[0] = id_pol;

    // Lifespan Policy.
    IDL::traits<PortableServer::LifespanPolicy>::ref_type ls_pol =
        this->root_poa_->create_lifespan_policy (
            PortableServer::LifespanPolicyValue::PERSISTENT);
    this->policies_[1] = ls_pol;

    // Request Processing Policy.
    IDL::traits<PortableServer::RequestProcessingPolicy>::ref_type rp_pol =
        this->root_poa_->create_request_processing_policy (
            PortableServer::RequestProcessingPolicyValue::USE_SERVANT_MANAGER);
    this->policies_[2] = rp_pol;

    // Servant Retention Policy.
    if (servant_retention_policy)
      {
        IDL::traits<PortableServer::ServantRetentionPolicy>::ref_type srp_pol =
            this->root_poa_->create_servant_retention_policy (
                PortableServer::ServantRetentionPolicyValue::RETAIN);
        this->policies_[3] = srp_pol;
      }
    else
      {
        IDL::traits<PortableServer::ServantRetentionPolicy>::ref_type srp_pol =
            this->root_poa_->create_servant_retention_policy (
                PortableServer::ServantRetentionPolicyValue::NON_RETAIN);
        this->policies_[3] = srp_pol;
      }

    // Create myPOA as the child of RootPOA with the above
    // policies_. myPOA will use SERVANT_ACTIVATOR or
    // SERVANT_LOCATOR depending upon the servant retention policy
    // being RETAIN or NONRETAIN respectively.
    TAOX11_TEST_DEBUG << "Creating POA <" << name << ">" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type my_poa =
        this->root_poa_->create_POA (name, this->poa_manager_, this->policies_);
    // Destroy the policy objects as they have been passed to
    // create_POA and no longer needed.
    for (IDL::traits<CORBA::Policy>::ref_type _pol : this->policies_)
      _pol->destroy ();

    return my_poa;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "ERROR: exception caught while creating POA <" << name
        << "> : " << ex << std::endl;
    return 0;
  }

  return nullptr;
}

// The Servant Activator object is created and initialised.
int
Server_i::create_activator(
    IDL::traits<PortableServer::POA>::ref_type first_poa)
{
  int result = 0;
  try
  {
    TAOX11_TEST_INFO << "Server_i::create_activator : "
        << "start creation of a ServantActivator" << std::endl;
    // An Servant Activator object is created which will activate
    // the servant on demand.
    IDL::traits<PortableServer::ServantActivator>::ref_type servant_activator =
        CORBA::make_reference<ServantActivator> (this->orb_,
            "Generic_Servant", "supply_servant");
    // implicitly widen
    IDL::traits<CORBA::Object>::ref_type obj = servant_activator;
    // (re-)narrow
    servant_activator = IDL::traits<PortableServer::ServantActivator>::narrow (obj);
    if (!servant_activator)
    {
      TAOX11_TEST_ERROR << "Unable to (re-)narrow PortableServer::ServantActivator from CORBA::Object reference"
          << std::endl;
      return 1;
    }

    // Check if the dll file can still be opened.
    ACE_DLL dll;
    if (dll.open ("Generic_Servant") == -1)
    {
      TAOX11_TEST_ERROR << "Error opening DLL again : " << dll.error () << std::endl;
      return 2;
    }

    // Set ServantActivator object as the servant_manager of
    // firstPOA.
    first_poa->set_servant_manager (servant_activator);
    TAOX11_TEST_DEBUG << "Server_i::create_activator : "
        << "New created ServantActivator set as Servant "
        << "Manager on first POA" << std::endl;

    IDL::traits<PortableServer::ServantManager>::ref_type got_servant_manager =
        first_poa->get_servant_manager ();
    if (got_servant_manager == nullptr)
      {
        TAOX11_TEST_ERROR << "Server_i::create_activator : "
            << "Error retrieving newly created ServantActivator." << std::endl;
        return 1;
      }

    PortableServer::ObjectId first_test_oid =
        PortableServer::string_to_ObjectId ("first test");

    this->first_test_ = first_poa->create_reference_with_id (first_test_oid,
        "IDL:test:1.0");
    if (this->first_test_ == nullptr)
      {
        TAOX11_TEST_ERROR << "Server_i::create_activator : "
            << "Error creating the first Test object." << std::endl;
        return 1;
      }
    else
      {
        TAOX11_TEST_DEBUG << "Server_i::create_activator : "
            << "Successfully created first Test object." << std::endl;
      }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Server_i:create_activator (): " << ex << std::endl;
    return 1;
  }

  TAOX11_TEST_INFO << "Server_i::create_activator : "
      << "Successfully created and set Servant Activator" << std::endl;
  return result;
}

// The Servant Locator object is created and initialised.

int
Server_i::create_locator(
    IDL::traits<PortableServer::POA>::ref_type second_poa)
{
  try
  {
    TAOX11_TEST_INFO << "Server_i::create_locator : "
        << "start creation of a ServantLocator" << std::endl;
    // An Servant Locator object is created which will activate
    // the servant on demand.
    IDL::traits<PortableServer::ServantLocator>::ref_type servant_locator =
        CORBA::make_reference<ServantLocator> (this->orb_, "Generic_Servant",
            "supply_servant");
    // implicitly widen
    IDL::traits<CORBA::Object>::ref_type obj = servant_locator;
    // (re-)narrow
    servant_locator = IDL::traits<PortableServer::ServantLocator>::narrow (obj);
    if (!servant_locator)
    {
      TAOX11_TEST_ERROR << "Unable to (re-)narrow PortableServer::ServantLocator from CORBA::Object reference"
          << std::endl;
      return 1;
    }
    // Set ServantLocator object as the servant Manager of
    // secondPOA.
    second_poa->set_servant_manager (servant_locator);
    TAOX11_TEST_DEBUG << "Server_i::create_locator : "
        << "New created ServantLocator set as Servant "
        << "Manager on second POA" << std::endl;

    IDL::traits<PortableServer::ServantManager>::ref_type got_servant_manager =
        second_poa->get_servant_manager ();
    if (got_servant_manager == nullptr)
      {
        TAOX11_TEST_ERROR << "Server_i::create_locator : "
            << "Error retrieving newly created ServantLocator." << std::endl;
        return 1;
      }

    PortableServer::ObjectId second_test_oid =
        PortableServer::string_to_ObjectId ("second test");

    this->second_test_ = second_poa->create_reference_with_id (second_test_oid,
        "IDL:test:1.0");
    if (this->second_test_ == nullptr)
      {
        TAOX11_TEST_ERROR << "Server_i::create_locator : "
            << "Error creating the second Test object." << std::endl;
        return 1;
      }
    else
      {
        TAOX11_TEST_DEBUG << "Server_i::create_locator : "
            << "Successfully created second Test object." << std::endl;
      }
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Server_i:create_locator () : " << ex << std::endl;
    return 1;
  }

  return 0;
}

// The execution process of the server.

int
Server_i::run()
{

  try
  {
    // Invoke object_to_string on the references created in firstPOA
    // and secondPOA.
    TAOX11_TEST_DEBUG << "Server_i::run : about to start" << std::endl;

    std::string first_test_ior = orb_->object_to_string (this->first_test_);

    std::string second_test_ior = orb_->object_to_string (this->second_test_);

    // Print the ior's of first_test and second_test.
    TAOX11_TEST_DEBUG << "First IOR : " << first_test_ior << std::endl;
    TAOX11_TEST_DEBUG << "Second IOR : " << second_test_ior << std::endl;

    int write_result = this->write_iors_to_file (first_test_ior,
        second_test_ior);
    if (write_result != 0)
      return write_result;

    // Set the poa_manager state to active, ready to process
    // requests.
    this->poa_manager_->activate ();

    // Run the ORB.
    this->orb_->run ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Server_i:run ():" << ex << std::endl;
    return 1;
  }

  return 0;
}
