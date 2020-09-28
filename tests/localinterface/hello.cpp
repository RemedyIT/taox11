/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"
#include "testlib/taox11_testlog.h"

// Test

// Implementation interface iLocal. Method to test if instance is OK.
std::string iLocal_impl::do_something()
{
  TAOX11_TEST_DEBUG << "Called iLocal_impl::do_something" << std::endl;
  return "iLocal_impl::do_something";
}

// Implementation interface A.
// Create instance of local interface and test the created instance.
std::string iA_impl::do_A(int32_t& result)
{
  TAOX11_TEST_DEBUG << "Called iA_impl::do_A" << std::endl;
  IDL::traits<Test::iLocal_A>::ref_type lia = CORBA::make_reference<iLocal_A_impl> ();
  if (lia->do_A(result) != "iLocal_A_impl::do_A") {
    TAOX11_TEST_ERROR << "ERROR: Retrieve of iA returned nil object." << std::endl;
    ++result;
    return "ERROR: Retrieve of iA returned nil object.";
  }
  IDL::traits<Test::iLocal>::ref_type li = CORBA::make_reference<iLocal_impl> ();
  return "iA_impl::do_A-->" + li->do_something();
}

// Hello

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa, int& result) :
    orb_(std::move(orb)), poa_(std::move(poa)), result_(result)
{
}

IDL::traits<Test::iA>::ref_type Hello::get_iA()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iA" << std::endl;
  CORBA::servant_traits<Test::iA>::ref_type impl = CORBA::make_reference<iA_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(
      id);
  IDL::traits<Test::iA>::ref_type ia = IDL::traits<Test::iA>::narrow (object);
  return ia;
}

IDL::traits<Test::iA>::ref_type Hello::get_localnull()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_localnull" << std::endl;
  IDL::traits<Test::iLocal_A>::ref_type lia;
  return lia;
}

IDL::traits<Test::iA>::ref_type Hello::get_local()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_local" << std::endl;
  IDL::traits<Test::iLocal_A>::ref_type lia = CORBA::make_reference<iLocal_A_impl> () ;
  return lia;
}

IDL::traits<Test::iA>::ref_type Hello::inout_local(
    IDL::traits<Test::iA>::ref_type in_v,
    IDL::traits<Test::iA>::ref_type& out_v,
    IDL::traits<Test::iA>::ref_type& inout_v)
{
  //in_v->do_A() not possible;
  inout_v->do_A(result_);
  inout_v = in_v;
  out_v = in_v;
  return m_lia;
}

IDL::traits<Test::iA>::ref_type Hello::attrIa()
{
  return m_lia;
}

IDL::traits<Test::iA>::ref_type temp;
void Hello::attrIa(IDL::traits<Test::iA>::ref_type _v)
{
  temp = _v;
  //_v->do_A() not possible;
}

// Hello End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
