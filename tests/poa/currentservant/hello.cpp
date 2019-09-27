/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 POA test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testlib/taox11_testlog.h"
#include "hello.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb) :
    orb_ (orb)
{
}

Hello::~Hello()
{
  TAOX11_TEST_INFO << "Hello::~Hello()" << std::endl;
}

std::string
Hello::writePoaName()
{
  IDL::traits<CORBA::Object>::ref_type obj =
      this->orb_->resolve_initial_references ("POACurrent");
  IDL::traits<PortableServer::Current>::ref_type current =
      IDL::traits<PortableServer::Current>::narrow (obj);

  TAOX11_TEST_INFO << "call get_POA" << std::endl;
  IDL::traits<PortableServer::POA>::ref_type poa = current->get_POA ();
  std::string poa_name = poa->the_name ();
  TAOX11_TEST_INFO << "Method invoked on servant in POA = " << poa_name
      << std::endl;
  if (poa_name != "RootPOA")
  {
    TAOX11_TEST_ERROR << "ERROR: Expected 'RootPOA' instead of '" << poa_name
        << "'." << std::endl;
    return nullptr;
  }
  TAOX11_TEST_INFO << "call get_object_id";
  PortableServer::ObjectId oid = current->get_object_id ();
  TAOX11_TEST_INFO << " -> received oid:" << oid << std::endl;

  TAOX11_TEST_INFO << "call get_reference";
  IDL::traits<CORBA::Object>::ref_type obj_ref = current->get_reference ();
  TAOX11_TEST_INFO << " -> received oid:" << oid << std::endl;

  TAOX11_TEST_INFO << "call get_servant" << std::endl;
  CORBA::servant_reference<PortableServer::Servant> servant_ref =
    current->get_servant ();

  TAOX11_TEST_INFO << "call servant_to_reference on current servant" << std::endl;
  IDL::traits<CORBA::Object>::ref_type obj_ref3 =
      poa->servant_to_reference (servant_ref);

  TAOX11_TEST_INFO << "call servant_to_reference on ourself." << std::endl;
  IDL::traits<CORBA::Object>::ref_type obj_ref4 =  poa->servant_to_reference (this->_lock());

  return poa_name;
}

void
Hello::shutdown()
{
  this->orb_->shutdown (false);
}
