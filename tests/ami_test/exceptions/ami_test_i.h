/**
 * @file    ami_test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_AMI_TEST_I_H
#define TAOX11_AMI_TEST_I_H

#include "testlib/taox11_testlog.h"

#include "ami_testS.h"

class AMI_Test_i
: public virtual CORBA::servant_traits< A::AMI_Test>::base_type
{
public:
  AMI_Test_i (IDL::traits<CORBA::ORB>::ref_type orb);

  // The AMI_Test methods.
  int32_t foo (int32_t in_l) override;

  int32_t foo_no_raises (int32_t in_l) override;

  int32_t bar (int32_t in_l) override;

  void shutdown () override;

private:
  IDL::traits<CORBA::ORB>::ref_type  orb_;
};

#endif /* TAOX11_AMI_TEST_I_H */
