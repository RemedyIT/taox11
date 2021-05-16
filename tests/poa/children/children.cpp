/**
 * @file    children.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/portable_server/portableserver_impl.h"
#include "tao/x11/orb.h"

#include "testlib/taox11_testlog.h"

// Define the names of the POA's
const std::string ROOT_POA = "RootPOA";
const std::string FIRST_POA = "firstPOA";
const std::string SECOND_POA = "secondPOA";
const std::string THIRD_POA = "thirdPOA";

uint16_t
print_poa (IDL::traits<PortableServer::POA>::ref_type poa, uint16_t result)
{
  std::string poa_name = poa->the_name ();

  CORBA::OctetSeq poa_id = poa->id ();

  PortableServer::POAList children =
    poa->the_children ();

  // check the number of children first.
  // - ROOT_POA should have two
  // - FIRST_POA should have one
  // - SECOND_POA and THIRD_POA should have none
  if (poa_name == ROOT_POA && children.size () != 2)
    {
      TAOX11_TEST_ERROR << "ERROR: unexpected number of children found for <"
                   << poa_name << ">. found <" << children.size ()
                   << "> - expected <2>." << std::endl;
      ++result;
    }
  if (poa_name == FIRST_POA && children.size () != 1)
    {
      TAOX11_TEST_ERROR << "ERROR: unexpected number of children found for <"
                   << poa_name << ">. found <" << children.size ()
                   << "> - expected <1>." << std::endl;
      ++result;
    }
  if ((poa_name == SECOND_POA || poa_name == THIRD_POA) &&
      children.size () != 0)
    {
      TAOX11_TEST_ERROR << "ERROR: unexpected number of children found for <"
                   << poa_name << ">. found <" << children.size ()
                   << "> - expected <0>." << std::endl;
      ++result;
    }

  TAOX11_TEST_INFO << "POA <"
        << poa_name << "> has <"
        << children.size () << "> child(ren). ID = "
        << poa_id << std::endl;

  for (IDL::traits<PortableServer::POA>::ref_type _poa : children)
    {
      if (poa_name == FIRST_POA)
        {
          // for logging purposes.
          TAOX11_TEST_INFO << "   " << std::flush;
        }
      TAOX11_TEST_INFO << "   Child : " << std::flush;
      result = print_poa (_poa, result);
    }
  return result;
}

int
main (int argc, char *argv[])
{
  uint16_t errors = 0;

  try
    {
      // The first step Initialize the ORB
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      // Obtain the RootPOA.
      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references (ROOT_POA);

      // _narrow () the Object to get the POA object, i.e., the root_poa.
      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) "
                       << "returned nil."
                       << std::endl;
          return 1;
        }

      // Policies for the _new_ POAs
      CORBA::PolicyList policies (2);
      policies.resize (2);

      // Threading policy
      IDL::traits<PortableServer::ThreadPolicy>::ref_type tp_pol =
        root_poa->create_thread_policy  (
          PortableServer::ThreadPolicyValue::ORB_CTRL_MODEL);
      policies[0] = tp_pol;

      // Lifespan policy
      IDL::traits<PortableServer::LifespanPolicy>::ref_type ls_pol =
        root_poa->create_lifespan_policy (
          PortableServer::LifespanPolicyValue::PERSISTENT);
      policies[1] = ls_pol;

      if (!tp_pol || !ls_pol)
        {
          TAOX11_TEST_ERROR << "ERROR: Failed to create policies."
                       << std::endl;
          return 1;
        }

      // Creation of the firstPOA
      std::string name = FIRST_POA;
      IDL::traits<PortableServer::POA>::ref_type first_poa =
        root_poa->create_POA (name,
                              nullptr,
                              policies);
      if (first_poa)
        {
          TAOX11_TEST_INFO << "Succesfully created <" << FIRST_POA
                      << ">." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Failed to create <"
                       << FIRST_POA << ">"
                       << std::endl;
          return 1;
        }

      // Creation of the _new_ POA, i.e. firstPOA/secondPOA
      name = SECOND_POA;
      IDL::traits<PortableServer::POA>::ref_type second_poa =
        first_poa->create_POA (name,
                               nullptr,
                               policies);
      if (second_poa)
        {
          TAOX11_TEST_INFO << "Succesfully created <" << SECOND_POA
                      << ">." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Failed to create <"
                       << SECOND_POA << ">"
                       << std::endl;
          return 1;
        }


      // Creation of the _new_ POAs is done, so destroy the Policy's.
      for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
        {
          _pol->destroy ();
        }

      // Creating thirdPOA.
      name = THIRD_POA;

      IDL::traits<PortableServer::POA>::ref_type third_poa =
        root_poa->create_POA (name,
                              nullptr,
                              CORBA::PolicyList ());
      if (third_poa)
        {
          TAOX11_TEST_INFO << "Succesfully created <" << THIRD_POA
                      << ">." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Failed to create <"
                       << THIRD_POA << ">"
                       << std::endl;
          return 1;
        }


      // Check the names of all created POA's
      const std::string root_poa_name = root_poa->the_name ();
      if (root_poa_name != ROOT_POA)
        {
          TAOX11_TEST_ERROR << "ERROR: found unexpected name "
                       << "for <" << ROOT_POA << ": found <"
                       << root_poa_name << ">"
                       << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_INFO << "Correct name for <" << root_poa_name
                      << "> found." << std::endl;
        }

      std::string const first_poa_name = first_poa->the_name ();
      if (first_poa_name != FIRST_POA)
        {
          TAOX11_TEST_ERROR << "ERROR: found unexpected name "
                       << "for <" << FIRST_POA << ": found <"
                       << first_poa_name << ">"
                       << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_INFO << "Correct name for <" << first_poa_name
                      << "> found." << std::endl;
        }

      std::string const second_poa_name = second_poa->the_name ();
      if (second_poa_name != SECOND_POA)
        {
          TAOX11_TEST_ERROR << "ERROR: found unexpected name "
                       << "for <" << SECOND_POA << ": found <"
                       << second_poa_name << ">"
                       << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_INFO << "Correct name for <" << second_poa_name
                      << "> found." << std::endl;
        }

      std::string const third_poa_name = third_poa->the_name ();
      if (third_poa_name != THIRD_POA)
        {
          TAOX11_TEST_ERROR << "ERROR: found unexpected name "
                       << "for <" << THIRD_POA << ": found <"
                       << third_poa_name << ">"
                       << std::endl;
          ++errors;
        }
      else
        {
          TAOX11_TEST_INFO << "Correct name for <" << third_poa_name
                      << "> found." << std::endl;
        }

      errors += print_poa (root_poa, 0);

      if (errors > 0)
        {
          TAOX11_TEST_ERROR << "Finished tests with errors. <"
                        << errors << "> errors were found."
                        << std::endl;
        }
      else
        {
          TAOX11_TEST_DEBUG << "Finished tests. No errors were found"
                        << std::endl;
        }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << ex;
      return 1;
    }

  return errors;
}
