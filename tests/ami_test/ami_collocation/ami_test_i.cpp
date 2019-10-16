/**
 * @file    ami_test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ami_test_i.h"

AMI_Test_i::AMI_Test_i ()

  :  in_l_last_ ((int32_t) 931235),
     in_str_("Let's talk AMI."),
     in_str_last_("Let's talk AMI the last time.")
{
}
int32_t
AMI_Test_i::foo (int32_t& out_l,
                 int32_t in_l,
                 const std::string& in_str)
{
  out_l = in_l;
  int32_t ret_ = in_l + 10;
  if (in_l == in_l_last_)
  {
    out_l = in_l_last_;
  }

  if (!((in_str_ == in_str) || (in_str_last_ == in_str)))
  {
    TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo Parameter corruption on in parameters: "
                   << "expected in_str: " << in_str_ << " or " << in_str_last_
                   << " received: " << in_str << std::endl;
    ret_ = 0;
  }

  return ret_;
}

