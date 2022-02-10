/**
 * @file    poa.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA  basic POA test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/portable_server/portableserver.h"

#include "testlib/taox11_testlog.h"

int main(int argc, char* argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb =
      CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
          << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj =
      _orb->resolve_initial_references("RootPOA");
    if (!obj)
    {
      TAOX11_TEST_ERROR
          << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type root_poa =
      IDL::traits<PortableServer::POA>::narrow (obj);
    if (!root_poa)
    {
      TAOX11_TEST_ERROR
          << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "narrowed POA interface" << std::endl;

    IDL::traits<PortableServer::POAManager>::ref_type poaman =
      root_poa->the_POAManager();
    if (!poaman)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "retrieved POAManager" << std::endl;

    CORBA::PolicyList plist;
    IDL::traits<PortableServer::ThreadPolicy>::ref_type tp =
      root_poa->create_thread_policy(
        PortableServer::ThreadPolicyValue::ORB_CTRL_MODEL);
    plist.push_back(tp);
    TAOX11_TEST_DEBUG << "initialized policy list" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type child_poa =
      root_poa->create_POA("child_poa", poaman, plist);
    if (!child_poa)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->create_POA () returned null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "created child POA" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type root_parent =
        root_poa->the_parent ();
    if (root_parent)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->the_parent () should return null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "retrieved root POA parent null" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type child_parent =
        child_poa->the_parent ();
    if (!child_parent)
    {
      TAOX11_TEST_ERROR << "ERROR: child_poa->the_parent () returned null object."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "retrieved child parent POA" << std::endl;

    CORBA::servant_reference<PortableServer::Servant> servant_ref;
    try
    {
      IDL::traits<CORBA::Object>::ref_type obj_ref = child_poa->servant_to_reference(servant_ref);
      TAOX11_TEST_ERROR << "ERROR: poa->servant_to_reference () should throw exception."
            << std::endl;
        return 1;
    }
    catch (const CORBA::INV_OBJREF&)
    {
      TAOX11_TEST_DEBUG << "poa->servant_to_reference(empty): caught CORBA::INV_OBJREF as expected" << std::endl;
    }
    try
    {
      PortableServer::ObjectId id = child_poa->servant_to_id(servant_ref);
      TAOX11_TEST_ERROR << "ERROR: poa->servant_to_id () should throw exception."
            << std::endl;
      return 1;
    }
    catch (const CORBA::INV_OBJREF&)
    {
      TAOX11_TEST_DEBUG << "poa->servant_to_id(empty): caught CORBA::INV_OBJREF as expected" << std::endl;
    }

    poaman->activate();

    TAOX11_TEST_DEBUG << "activated POAManager" << std::endl;

    TAOX11_TEST_DEBUG << "shutting down...";

    _orb->destroy ();

    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught: " << ex << std::endl;
    return 1;
  }
  return 0;
}
