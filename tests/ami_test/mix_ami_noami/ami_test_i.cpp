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
AmiMyFoo_i::amifoo (int32_t)
{
 return 2;
}

int32_t
AmiMyFoo_i::my_amifoo_attrib ()
{
  return 3;
}

void
AmiMyFoo_i::my_amifoo_attrib (int32_t )
{
}

int32_t
MyFoo_i::foo (int32_t)
{
 return 22;
}

int32_t
MyFoo_i::my_foo_attrib ()
{
  return 33;
}

void
MyFoo_i::my_foo_attrib (int32_t )
{
}


Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(std::move(orb)), poa_(std::move(poa))
{
}
IDL::traits<A::AmiMyFoo>::ref_type Hello::get_iAmiMyFoo()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iAmiMyFoo" << std::endl;
  CORBA::servant_traits<A::AmiMyFoo>::ref_type impl = CORBA::make_reference<AmiMyFoo_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::AmiMyFoo::_ref_type ia = IDL::traits<A::AmiMyFoo>::narrow (object);
  return ia;
}

IDL::traits<A::MyFoo>::ref_type Hello::get_iMyFoo()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iMyFoo" << std::endl;
  CORBA::servant_traits<A::MyFoo>::ref_type impl = CORBA::make_reference<MyFoo_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::MyFoo::_ref_type ia = IDL::traits<A::MyFoo>::narrow (object);
  return ia;
}


void Hello::shutdown()
{
  this->orb_->shutdown(false);
}

