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

class AMI_TEST_Export AMI_Test_i
: public virtual CORBA::servant_traits< A::AMI_Test>::base_type
{
public:
  /// ctor
  AMI_Test_i ();

  // The AMI_Test methods.
  int32_t foo (int32_t& out_l,
               int32_t in_l,
               const std::string& in_str) override;

private:
  int32_t in_l_last_;
  std::string const in_str_;
  std::string const in_str_last_;

};
#endif /* TAOX11_AMI_TEST_I_H */
