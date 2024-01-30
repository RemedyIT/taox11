/**
 * @file    factoryc_impl.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 valuetype factory application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAO_FACTORY_FACTORY_C_IMPL_H
#define TAO_FACTORY_FACTORY_C_IMPL_H

#include "factoryC.h"

/**
 * @class Value2_impl
 *
 * @brief Value2 implementation.
 *
 * Implements the Value2 valuetype.
 *
 * @note Just need to mix-in RefCounter
 */
class Value2_impl : public IDL::traits<OBV_Test::Value2>::obv_type
{
public:
  Value2_impl () = default;
};

/**
 * @class Value2_init_impl
 *
 * @brief OBV factory implementation.
 *
 * Implements the Value2 factory implementation.
 */
class Value2_init_impl : public IDL::traits<OBV_Test::Value2>::factory_type
{
public:
  Value2_init_impl ()  = default;
  ~Value2_init_impl () override = default;
  IDL::traits<OBV_Test::Value2>::ref_type create_default (uint32_t id) override;

  IDL::traits<OBV_Test::Value2>::ref_type create (
      uint32_t id,
      const OBV_Test::BasValue::BV_Data& data) override;

  IDL::traits<CORBA::ValueBase>::ref_type create_for_unmarshal () override;
};

#endif /* TAO_FACTORY_FACTORY_C_IMPL_H */
