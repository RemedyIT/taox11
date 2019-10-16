/**
 * @file    factorys_impl.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 valuetype factory application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAO_FACTORY_S_IMPL_H
#define TAO_FACTORY_S_IMPL_H

#include "factoryS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Factory_impl  : public CORBA::servant_traits< OBV_Test::IFactory >::base_type
{
public:
  /// ctor
  Factory_impl (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual IDL::traits<OBV_Test::BasValue>::ref_type get_base_value() override;
  virtual IDL::traits<OBV_Test::Value1>::ref_type get_value1() override;
  virtual IDL::traits<OBV_Test::Value2>::ref_type get_value2() override;

  virtual void shutdown () override;

private:
  /// The ORB
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAO_FACTORY_S_IMPL_H */
