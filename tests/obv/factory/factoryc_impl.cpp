/**
 * @file    factoryc_impl.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 valuetype factory application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "factoryc_impl.h"
#include "factoryC.h"
#include "factoryS.h"

IDL::traits<OBV_Test::Value2>::ref_type
Value2_init_impl::create_default(uint32_t id)
{
  IDL::traits<OBV_Test::Value2>::ref_type ret_val = CORBA::make_reference<
      Value2_impl>();
  if (ret_val != nullptr)
  {
    ret_val->id(id);
  }
  return ret_val;
}

IDL::traits<OBV_Test::Value2>::ref_type
Value2_init_impl::create(uint32_t id, const OBV_Test::BasValue::BV_Data& data)
{
  IDL::traits<OBV_Test::Value2>::ref_type ret_val = CORBA::make_reference<
      Value2_impl>();
  if (ret_val != nullptr)
  {
    ret_val->id(id);
    ret_val->data(data);
  }
  return ret_val;
}

IDL::traits<CORBA::ValueBase>::ref_type
Value2_init_impl::create_for_unmarshal()
{
  IDL::traits<CORBA::ValueBase>::ref_type ret_val = CORBA::make_reference<
      Value2_impl>();
  return ret_val;
}
