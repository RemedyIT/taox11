/**
 * @file    hello2.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello2.h"

int32_t
B_i::op_B (int32_t in_l)
{
  return (in_l);
}


Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(orb), poa_(poa)
{
}
IDL::traits<Test::A>::ref_type Hello::get_iA()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iA" << std::endl;
  CORBA::servant_traits<Test::A>::ref_type impl = CORBA::make_reference<A_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::A::_ref_type ia = IDL::traits<Test::A>::narrow (object);
  return ia;
}

IDL::traits<Test::B>::ref_type Hello::get_iB()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iB" << std::endl;
  CORBA::servant_traits<Test::B>::ref_type impl = CORBA::make_reference<B_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::B::_ref_type ib = IDL::traits<Test::B>::narrow (object);
  if (!ib)
  {
    TAOX11_TEST_DEBUG << "Hello::get_iB: narrow failed" << std::endl;
  }
  return ib;
}


void Hello::shutdown()
{
  this->orb_->shutdown(false);
}

