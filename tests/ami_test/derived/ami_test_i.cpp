/**
 * @file    ami_test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ami_test_i.h"
int32_t
MyFoo_i::foo (int32_t in_l)
{
  if (in_l== 0)
   {
     throw A::InternalError(2, "Hello from foo");
   }
  return 2;
}

int32_t
MyFoo_i::my_foo_attrib ()
{
  return 3;
}

void
MyFoo_i::my_foo_attrib (int32_t )
{
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
MyDerived_i::foo (int32_t)
{
  return 6;
}

int32_t
MyDerived_i::my_foo_attrib ()
{
  return 7;
}

void
MyDerived_i::my_foo_attrib (int32_t )
{
}

int32_t
MyBar_i::bye (int32_t& in_l)
{
  in_l = 100;
  return 8;
}

int32_t
MyBar_i::my_bar_attrib ()
{
  return 9;
}

void
MyBar_i::my_bar_attrib (int32_t)
{
}

int32_t
MyBar_i::foo (int32_t in_l)
{
  if (in_l== 0)
     {
       throw A::InternalError(3, "Hello from bar");
     }
  return 10;
}

int32_t
MyBar_i::my_foo_attrib ()
{
  return 11;
}

void
MyBar_i::my_foo_attrib (int32_t)
{
}

int32_t
MyBar_i::do_something (const std::string&)
{
  return 12;
}

int32_t
MyBar_i::my_derived_attrib ()
{
  return 13;
}

void
MyBar_i::my_derived_attrib (int32_t)
{
}

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(std::move(orb)), poa_(std::move(poa))
{
}

IDL::traits<A::MyFoo>::ref_type
Hello::get_iMyFoo()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iMyFoo" << std::endl;
  CORBA::servant_traits<A::MyFoo>::ref_type impl = CORBA::make_reference<MyFoo_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::MyFoo::_ref_type ia = IDL::traits<A::MyFoo>::narrow (object);
  if (!ia)
    {
      TAOX11_TEST_ERROR << "Hello::get_iMyFoo: narrow failed" << std::endl;
    }
  return ia;
}

IDL::traits<A::MyDerived>::ref_type
Hello::get_iMyDerived ()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iMyDerived" << std::endl;
  CORBA::servant_traits<A::MyDerived>::ref_type impl = CORBA::make_reference<MyDerived_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::MyDerived::_ref_type ib = IDL::traits<A::MyDerived>::narrow (object);
  if (!ib)
    {
      TAOX11_TEST_ERROR << "Hello::get_iMyDerived: narrow failed" << std::endl;
    }
  return ib;
}

IDL::traits<A::MyBar>::ref_type
Hello::get_iMyBar ()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iMyBar" << std::endl;
  CORBA::servant_traits<A::MyBar>::ref_type impl = CORBA::make_reference<MyBar_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::MyBar::_ref_type ia = IDL::traits<A::MyBar>::narrow (object);
  if (!ia)
    {
      TAOX11_TEST_ERROR << "Hello::get_iMyBar: narrow failed" << std::endl;
    }
  return ia;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown(false);
}

