/**
 * @file    ami_test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "ami_test_i.h"

AMI_Test_i::AMI_Test_i (IDL::traits<CORBA::ORB>::ref_type orb)

  :  orb_ (orb)
{
}
int32_t
AMI_Test_i::foo (int32_t in_l)
{
  return (in_l + 10);
}

void
AMI_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
