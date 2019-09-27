/**
 * @file    ami_test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "ami_test_i.h"
int32_t
A_i::op_A (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(1, "Hello from A_i::op_A");
    }
  return 1;
}

int32_t
B_i::op_A (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(11, "Hello from B_i::op_A");
    }
  return 11;
}

int32_t
B_i::op_B (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(12, "Hello from B_i::op_B");
    }
  return 12;
}


int32_t
C_i::op_A (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(111, "Hello from C_i::op_A");
    }
  return 111;
}

int32_t
C_i::op_C (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(113, "Hello from C_i::op_C");
    }
  return 113;
}

int32_t
D_i::op_A (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(1111, "Hello from D_i::op_A");
    }
  return 1111;
}

int32_t
D_i::op_B (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(1112, "Hello from D_i::op_B");
    }
  return 1112;
}

int32_t
D_i::op_C (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(1113, "Hello from D_i::op_C");
    }
  return 1113;
}

int32_t
D_i::op_D (int32_t in_l)
{
  if (in_l== 0)
    {
      throw Test::IntErr(1114, "Hello from D_i::op_D");
    }
  return 1114;
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

IDL::traits<Test::C>::ref_type Hello::get_iC()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iC" << std::endl;
  CORBA::servant_traits<Test::C>::ref_type impl = CORBA::make_reference<C_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::C::_ref_type ia = IDL::traits<Test::C>::narrow (object);
  return ia;
}

IDL::traits<Test::D>::ref_type Hello::get_iD()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iD" << std::endl;
  CORBA::servant_traits<Test::D>::ref_type impl = CORBA::make_reference<D_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::D::_ref_type ia = IDL::traits<Test::D>::narrow (object);
  return ia;
}


void Hello::shutdown()
{
  this->orb_->shutdown(false);
}

