/**
 * @file    test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_TEST_I_H
#define TAOX11_TEST_I_H

#include "testlib/taox11_testlog.h"

#include "testS.h"

class Test_i
: public virtual CORBA::servant_traits< A::Test>::base_type
{
public:
  /// ctor
  Test_i (IDL::traits<CORBA::ORB>::ref_type orb);

  // The Test methods.
  int32_t foo (int32_t in_l) override;

  void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAOX11_TEST_I_H */
