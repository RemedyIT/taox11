/**
 * @file    newpoa.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 POA tests
 *
 *  This program demonstrates creation of _new_ POAs, as children of the root POA or the
 *  existing POA.
 *  There are three _new_ POA created in this example.
 *  The hierarchy of POAs looks like this.
 *
 *           /-->first_poa-->first_poa/second_poa
 *  RootPOA--
 *           \-->third_poa
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testlib/taox11_testlog.h"
#include "ace/Log_Msg.h"
#include "tao/x11/orb.h"
#include "tao/x11/portable_server/portableserver.h"
#include <map>

void print_poa(IDL::traits<PortableServer::POA>::ref_type poa)
{
  std::string poa_name = poa->the_name();

  CORBA::OctetSeq poa_id = poa->id();

  TAOX11_TEST_DEBUG << "POA name = " << poa_name << std::endl;

  TAOX11_TEST_DEBUG << "POA id = ";

  for (CORBA::OctetSeq::size_type id : poa_id)
  {
    TAOX11_TEST_DEBUG << id;
  }

  TAOX11_TEST_DEBUG << std::endl;

  PortableServer::POAList children = poa->the_children();

  for (IDL::traits<PortableServer::POA>::ref_type& p : children)
  {
    print_poa(p);
  }
}

int main (int argc, char *argv[])
{
  try
  {
    // The first step Initialize the ORB
    IDL::traits<CORBA::ORB>::ref_type _orb =
      CORBA::ORB_init(argc, argv);

    // Obtain the RootPOA.
    IDL::traits<CORBA::Object>::ref_type obj =
      _orb->resolve_initial_references("RootPOA");

    TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;
    // narrow the Object to get the POA object, i.e., the root_poa.
    IDL::traits<PortableServer::POA>::ref_type root_poa =
      IDL::traits<PortableServer::POA>::narrow (obj);
    if (!root_poa)
    {
      TAOX11_TEST_ERROR
          << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "narrowed root POA interface" << std::endl;

    // Policies for the _new_ POAs
    CORBA::PolicyList policies(2);
    policies.resize(2);
    // Threading policy
    IDL::traits<PortableServer::ThreadPolicy>::ref_type thread_pol =
        root_poa->create_thread_policy(
            PortableServer::ThreadPolicyValue::ORB_CTRL_MODEL);
    TAOX11_TEST_DEBUG << "created thread policy" << std::endl;
    policies[0] = thread_pol;
    TAOX11_TEST_DEBUG << "assigned thread policy" << std::endl;
    // Lifespan policy
    IDL::traits<PortableServer::LifespanPolicy>::ref_type lifespan_pol =
        root_poa->create_lifespan_policy(
            PortableServer::LifespanPolicyValue::TRANSIENT);
    TAOX11_TEST_DEBUG << "created lifespan policy" << std::endl;
    policies[1] = lifespan_pol;
    TAOX11_TEST_DEBUG << "assigned thread policy" << std::endl;

    // Creation of the firstPOA
    std::string name = "firstPOA";
    const IDL::traits<PortableServer::POAManager>::ref_type a_POAManager;
    IDL::traits<PortableServer::POA>::ref_type first_poa =
      root_poa->create_POA(name, a_POAManager, policies);
    TAOX11_TEST_DEBUG << "created " << name << std::endl;

    // Creation of the _new_ POA, i.e. firstPOA/secondPOA
    name = "secondPOA";
    IDL::traits<PortableServer::POA>::ref_type second_poa =
      first_poa->create_POA(name, a_POAManager, policies);
    TAOX11_TEST_DEBUG << "created " << name << std::endl;

    // Creating thirdPOA.
    name = "thirdPOA";
    IDL::traits<PortableServer::POA>::ref_type third_poa =
      root_poa->create_POA(name, a_POAManager, policies);
    TAOX11_TEST_DEBUG << "created " << name << std::endl;

    IDL::traits<CORBA::Object>::ref_type object_poa = third_poa;

    IDL::traits<PortableServer::POA>::ref_type fourth_poa =
      IDL::traits<PortableServer::POA>::narrow (object_poa);

    // Creation of the _new_ POAs over, so destroy the policies
    for (IDL::traits<CORBA::Policy>::ref_type policy : policies)
    {
      policy->destroy();
    }

    TAOX11_TEST_DEBUG << "Destroyed the policies" << std::endl;

    // Get the names of all the POAs and print them out.
    std::string root_poa_name = root_poa->the_name();
    std::string first_poa_name = first_poa->the_name();
    std::string second_poa_name = second_poa->the_name();
    std::string third_poa_name = third_poa->the_name();
    std::string fourth_poa_name = fourth_poa->the_name();

    TAOX11_TEST_DEBUG << root_poa_name << std::endl
                      << first_poa_name << std::endl
                      << second_poa_name << std::endl
                      << third_poa_name << std::endl
                      << fourth_poa_name << std::endl;

    print_poa(root_poa);
    _orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught: " << ex << std::endl;
    return 1;
  }

  return 0;
}
