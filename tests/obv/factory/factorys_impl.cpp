/**
 * @file    factorys_impl.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 valuetype factory application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "factoryc_impl.h"
#include "factorys_impl.h"

Factory_impl::Factory_impl(IDL::traits<CORBA::ORB>::ref_type orb) :
    orb_(std::move(orb))
{
}

IDL::traits<OBV_Test::BasValue>::ref_type Factory_impl::get_base_value()
{
  IDL::traits<OBV_Test::BasValue>::ref_type ref = CORBA::make_reference<
      IDL::traits<OBV_Test::BasValue>::obv_type>();
  ref = CORBA::make_reference < IDL::traits<OBV_Test::BasValue>::obv_type
      > (OBV_Test::BasValue::BV_Data(), 5); // value, some_data
  if (ref->some_data() != 5)
    return nullptr;
  return ref;
}

IDL::traits<OBV_Test::Value1>::ref_type Factory_impl::get_value1()
{
  IDL::traits<OBV_Test::Value1>::ref_type ref = CORBA::make_reference<
      IDL::traits<OBV_Test::Value1>::obv_type>();
  ref = CORBA::make_reference < IDL::traits<OBV_Test::Value1>::obv_type > (3); // id
  if (ref->id() != 3)
    return nullptr;
  return ref;
}

IDL::traits<OBV_Test::Value2>::ref_type Factory_impl::get_value2()
{
  IDL::traits<OBV_Test::Value2>::ref_type ref = CORBA::make_reference<
      Value2_impl>();
  return ref;
}

void Factory_impl::shutdown()
{
  this->orb_->shutdown(false);
}
