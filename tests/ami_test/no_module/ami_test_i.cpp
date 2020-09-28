/**
 * @file    ami_test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ami_test_i.h"

MyFoo_i::MyFoo_i ()

  :  yadda_ ((int32_t) 140474),
     in_l_ ((int32_t) 931247),
     in_l_last_ ((int32_t) 931235),
     attr_bool_(true),
     in_str_("Let's talk AMI."),
     in_str_last_("Let's talk AMI the last time.")
{
}
int32_t
MyFoo_i::foo (int32_t& out_l,
                 int32_t in_l,
                 const std::string& in_str)
{
  out_l = 931233;
  int32_t ret_ = 931234;
  if (in_l == 0)
  {
    throw DidTheRightThing(42, "Hello world");
  }

  if (in_l == in_l_last_)
  {
    out_l = in_l_last_;
  }

  if (!((in_l_ == in_l) || (in_l_last_ == in_l)))
  {
    TAOX11_TEST_ERROR << "ERROR:  MyFoo_i::foo Parameter corruption on in parameters: "
                 << "expected in_l: " << in_l_ << " or " << in_l_last_
                 << " received: " << in_l << std::endl;
    ret_ = 0;
  }

  if (!((in_str_ == in_str) || (in_str_last_ == in_str)))
  {
    TAOX11_TEST_ERROR << "ERROR:  MyFoo_i::foo Parameter corruption on in parameters: "
                 << "expected in_str: " << in_str_ << " or " << in_str_last_
                 << " received: " << in_str << std::endl;
    ret_ = 0;
  }

  return ret_;
}

int32_t
MyFoo_i::yadda ()
{
  if (yadda_ == 140474)
  {
    throw CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
  }
  return yadda_;
}

void
MyFoo_i::yadda (int32_t yadda)
{
  if (yadda == 0)
  {
    throw CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
  }
  yadda_ = yadda;
}

bool
MyFoo_i::bool_attr()
{
  if(attr_bool_ == true)
  {
    throw DidTheRightThing(42, "Hello world");
  }
  return false;
}

void
MyFoo_i::bool_attr(bool attr_bool)
{
  if(attr_bool == true)
    throw CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
  attr_bool_ = attr_bool;
}

std::string
MyFoo_i::foo_struct (structType &out_t)
{
  out_t.as(5);
  out_t.bs (6);
  return "bye from foo_struct";
}


int32_t
MyDerived_i::do_something (const std::string&)
{
  return 4;
}

int32_t
MyDerived_i::my_derived_attrib ()
{
 return 5;
}

void
MyDerived_i::my_derived_attrib (int32_t)
{
}

int32_t
MyDerived_i::foo (int32_t& /*out_l*/,
                 int32_t in_l,
                 const std::string& /*in_str*/)
{
  return in_l;
}

int32_t
MyDerived_i::yadda ()
{
  return 1;
}

void
MyDerived_i::yadda (int32_t /*yadda*/)
{
}

bool
MyDerived_i::bool_attr()
{
  return false;
}

void
MyDerived_i::bool_attr(bool /*attr_bool*/)
{
}

std::string
MyDerived_i::foo_struct (structType &out_t)
{
  out_t.as(5);
  out_t.bs (6);
  return "bye from derived foo_struct";
}


Hello_i::Hello_i(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(std::move(orb)), poa_(std::move(poa))
{
}
IDL::traits<MyFoo>::ref_type Hello_i::get_iMyFoo()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iMyFoo" << std::endl;
  CORBA::servant_traits<MyFoo>::ref_type impl = CORBA::make_reference<MyFoo_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  MyFoo::_ref_type ia = IDL::traits<MyFoo>::narrow (object);
  return ia;
}

IDL::traits<A::MyDerived>::ref_type Hello_i::get_iMyDerived()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iMyDerived" << std::endl;
  CORBA::servant_traits<A::MyDerived>::ref_type impl = CORBA::make_reference<MyDerived_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::MyDerived::_ref_type ib = IDL::traits<A::MyDerived>::narrow (object);
  if (!ib)
  {
    TAOX11_TEST_DEBUG << "Hello::get_iB: narrow failed" << std::endl;
  }
  return ib;
}

void Hello_i::shutdown()
{
  this->orb_->shutdown(false);
}
