/**
 * @file    anys_impl.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 servant implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ANY_S_IMPL_H
#define TAOX11_ANY_S_IMPL_H

#include "anyS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Test_impl
  : public virtual CORBA::servant_traits< OBV_AnyTest::Test >::base_type
{
public:
  /// ctor
  Test_impl (IDL::traits<CORBA::ORB>::ref_type orb);

  CORBA::Any get_something (bool need_vb) override;

  IDL::traits<OBV_AnyTest::VB>::ref_type get_vb () override;

  CORBA::Any get_base () override;

  void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAOX11_ANY_S_IMPL_H */
