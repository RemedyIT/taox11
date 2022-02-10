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

// Interface A
std::string iA_impl::do_something_A()
{
  TAOX11_TEST_DEBUG << "Called iA_impl::do_something_A" << std::endl;
  return "A.A";
}

// Hello
Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa,
    int& result) :
  orb_(std::move(orb)), poa_(std::move(poa)), result_(result)
{
}

IDL::traits<Test::iA>::ref_type Hello::get_iA()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iA" << std::endl;
  CORBA::servant_traits<Test::iA>::ref_type impl = CORBA::make_reference<iA_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::iA::_ref_type ia = IDL::traits<Test::iA>::narrow (object);
  return ia;
}

IDL::traits<Test::iA>::ref_type Hello::get_null()
{
  return nullptr;
}

IDL::traits<Test::iA>::ref_type Hello::inout_null(
    IDL::traits<Test::iA>::ref_type in_v,
    IDL::traits<Test::iA>::ref_type& out_v,
    IDL::traits<Test::iA>::ref_type& inout_v)
{
  if (in_v != nullptr)
  {
    try
    {
      in_v->do_something_A ();
    }
    catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR: Usage of received reference failed: " << e << std::endl;
      ++result_;
    }
  }
  if (inout_v != nullptr)
  {
    try
    {
      inout_v->do_something_A ();
    }
    catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "ERROR: Usage of received reference failed: " << e << std::endl;
      ++result_;
    }
  }
  out_v = in_v;
  inout_v = in_v;
  return in_v;
}

// Hello End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
