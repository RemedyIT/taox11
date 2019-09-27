/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"
#include "testlib/taox11_testlog.h"

// Test


// Interface A

std::string iA_impl::do_something_A()
{
  TAOX11_TEST_DEBUG << "Called iA_impl::do_something_A" << std::endl;
  return "A.A";
}

// Interface B

std::string iB_impl::do_something_A()
{
  TAOX11_TEST_DEBUG << "Called iB_impl::do_something_A" << std::endl;
  return "B.A";
}

std::string iB_impl::do_something_B()
{
  TAOX11_TEST_DEBUG << "Called iB_impl::do_something_B" << std::endl;
  return "B.B";
}

// Interface C

std::string iC_impl::do_something_A()
{
  TAOX11_TEST_DEBUG << "Called iC_impl::do_something_A" << std::endl;
  return "C.A";
}

std::string iC_impl::do_something_B()
{
  TAOX11_TEST_DEBUG << "Called iC_impl::do_something_B" << std::endl;
  return "C.B";
}

std::string iC_impl::do_something_C()
{
  TAOX11_TEST_DEBUG << "Called iC_impl::do_something_C" << std::endl;
  return "C.C";
}

// Interface D

std::string iD_impl::do_something_D()
{
  TAOX11_TEST_DEBUG << "Called iD_impl::do_something_D" << std::endl;
  return "D.D";
}

// Interface E

std::string iE_impl::do_something_A()
{
  TAOX11_TEST_DEBUG << "Called iE_impl::do_something_A" << std::endl;
  return "E.A";
}

std::string iE_impl::do_something_B()
{
  TAOX11_TEST_DEBUG << "Called iE_impl::do_something_B" << std::endl;
  return "E.B";
}

std::string iE_impl::do_something_C()
{
  TAOX11_TEST_DEBUG << "Called iE_impl::do_something_C" << std::endl;
  return "E.C";
}

std::string iE_impl::do_something_D()
{
  TAOX11_TEST_DEBUG << "Called iE_impl::do_something_D" << std::endl;
  return "E.D";
}

std::string iE_impl::do_something_E()
{
  TAOX11_TEST_DEBUG << "Called iE_impl::do_something_E" << std::endl;
  return "E.E";
}

// Hello

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(orb), poa_(poa)
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

IDL::traits<Test::iB>::ref_type Hello::get_iB()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iB" << std::endl;
  CORBA::servant_traits<Test::iB>::ref_type impl = CORBA::make_reference<iB_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::iB::_ref_type ib = IDL::traits<Test::iB>::narrow (object);
  if (!ib)
  {
    TAOX11_TEST_DEBUG << "Hello::get_iB: narrow failed" << std::endl;
  }
  return ib;
}

IDL::traits<Test::iC>::ref_type Hello::get_iC()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iC" << std::endl;
  CORBA::servant_traits<Test::iC>::ref_type impl = CORBA::make_reference<iC_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::iC::_ref_type ia = IDL::traits<Test::iC>::narrow (object);
  return ia;
}

IDL::traits<Test::iD>::ref_type Hello::get_iD()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iD" << std::endl;
  CORBA::servant_traits<Test::iD>::ref_type impl = CORBA::make_reference<iD_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::iD::_ref_type i = IDL::traits<Test::iD>::narrow (object);
  return i;
}

IDL::traits<Test::iE>::ref_type Hello::get_iE()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iE" << std::endl;
  CORBA::servant_traits<Test::iE>::ref_type impl = CORBA::make_reference<iE_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::iE::_ref_type i = IDL::traits<Test::iE>::narrow (object);
  return i;
}

// Hello End

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
