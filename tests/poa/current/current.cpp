/**
 * @file    current.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "tao/x11/corba.h"
#include "tao/x11/orb.h"
#include "tao/x11/portable_server/portableserver.h"

int main (int argc, char *argv[])
{

  try
  {
    // Initialize the ORB first.
    IDL::traits<CORBA::ORB>::ref_type _orb =
      CORBA::ORB_init(argc, argv);
    IDL::traits<CORBA::Object>::ref_type obj =
      _orb->resolve_initial_references("RootPOA");
    if (!obj)
    {
      TAOX11_TEST_ERROR
          << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
          << std::endl;
      return 1;
    }
    TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;
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
    TAOX11_TEST_INFO << "narrowed root POA interface" << std::endl;

    obj = _orb->resolve_initial_references ("POACurrent");
    if (!obj)
    {
      TAOX11_TEST_ERROR << "ERROR: resolve_initial_references('POACurrent')returned nullptr." << std::endl;
      return 1;
    }

    IDL::traits<PortableServer::Current>::ref_type current =
      IDL::traits<PortableServer::Current>::narrow (obj);
    if (current == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: _narrow 'POACurrent' returned nullptr." << std::endl;
      return 1;
    }
    TAOX11_TEST_INFO << "narrowed current POA interface" << std::endl;

    try
    {
      TAOX11_TEST_INFO << "try get_POA" << std::endl;
      current->get_POA ();
    }
    catch (const PortableServer::Current::NoContext&)
    {
      TAOX11_TEST_INFO << " NoContext exception caught as expected." << std::endl;
    }

    try
    {
      TAOX11_TEST_INFO << "try get_object_id" << std::endl;
      current->get_object_id ();
    }
    catch (const PortableServer::Current::NoContext&)
    {
      TAOX11_TEST_INFO << " NoContext exception caught as expected." << std::endl;
    }

    try
    {
      TAOX11_TEST_INFO << "try get_reference" << std::endl;
      current->get_reference ();
    }
    catch (const PortableServer::Current::NoContext&)
    {
      TAOX11_TEST_INFO << " NoContext exception caught as expected." << std::endl;
    }

    try
    {
      TAOX11_TEST_INFO << "try get_servant" << std::endl;
      current->get_servant ();
    }
    catch (const PortableServer::Current::NoContext&)
    {
      TAOX11_TEST_INFO << " NoContext exception caught as expected." << std::endl;
    }

    _orb->destroy();
    TAOX11_TEST_INFO << "Test successful" << std::endl;
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught: " << ex << std::endl;
    return 1;
  }

  return 0;
}
