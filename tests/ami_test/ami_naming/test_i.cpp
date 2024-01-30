/**
 * @file    test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_i.h"

int32_t
Test_i::foo (int32_t in_l)
{
  int32_t ret_l;
  if (in_l == 10)
    {
      ret_l = 931235;
    }
  else
    {
      ret_l =  3;
    }
  return ret_l;
}

int32_t
Test_i::sendc_foo (int16_t /*in_s*/)
{
  return 4;
}

int32_t
Test_i::sendc_ami_foo (bool /*in_b*/)
{
  return 5;
}
int32_t
Test_i::yadda ()
{
 return num_l_ + 10;
}

void
Test_i::yadda (int32_t in_l)
{
  num_l_ = in_l;
}

int16_t
Test_i::sendc_get_yadda ()
{
 return num_s_ + 100;
}

void
Test_i::sendc_get_yadda (int16_t in_s)
{
  num_s_ = in_s;
}
bool
Test_i::sendc_ami_get_yadda ()
{
 return true;
}

void
Test_i::sendc_ami_get_yadda (bool )
{
}

int32_t
Test_i::_cxx_do ()
{
 return num_l_ + 10;
}

void
Test_i::_cxx_do (int32_t in_l)
{
  num_l_ = in_l;
}

int32_t
Two_i::foo_two (int32_t in_l)
{
  return (in_l + 10);
}

int32_t
Two_i::_cxx_do (int32_t in_l)
{
  return (in_l + 10);
}

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(std::move(orb)), poa_(std::move(poa))
{
}

IDL::traits<A::Test>::ref_type Hello::get_iTest()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iTest" << std::endl;
  CORBA::servant_traits<A::Test>::ref_type impl = CORBA::make_reference<Test_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::Test::_ref_type ia = IDL::traits<A::Test>::narrow (object);
  return ia;
}

IDL::traits<A::Two>::ref_type Hello::get_iTwo()
{
  TAOX11_TEST_DEBUG << "Called Hello::get_iTwo" << std::endl;
  CORBA::servant_traits<A::Two>::ref_type impl = CORBA::make_reference<Two_i> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  A::Two::_ref_type ib = IDL::traits<A::Two>::narrow (object);
  if (!ib)
  {
    TAOX11_TEST_DEBUG << "Hello::get_iTwo: narrow failed" << std::endl;
  }
  return ib;
}

void Hello::shutdown()
{
  this->orb_->shutdown(false);
}
