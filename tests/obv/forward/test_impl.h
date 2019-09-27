/**
 * @file    test_impl.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 servant implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "treecontrollerS.h"

/**
 * @class Test_impl
 *
 * @brief Implementation of Test interface
 *
 * This interface is provided to produce valuetypes and test
 * marshaling
 */
class Test_impl final
  : public virtual CORBA::servant_traits<Test>::base_type
{
public:
  /// ctor
  Test_impl (IDL::traits<CORBA::ORB>::ref_type orb);

  virtual IDL::traits<TreeController>::ref_type
  reflect (IDL::traits<TreeController>::ref_type tc) override;

  virtual void
  shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;

  Test_impl (const Test_impl&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (const Test_impl&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;
};

#endif /* TAO_TEST_IMPL_H */
