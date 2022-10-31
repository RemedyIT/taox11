/**
 * @file    ami_test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ami_test_i.h"

AMI_Test_i::AMI_Test_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb)),
    yadda_ ((int32_t) 0),
    in_l_ ((int32_t) 931247),
    inout_l_ ((int32_t) 300),
    in_l_last_ ((int32_t) 931235)
{
}
int32_t
AMI_Test_i::foo (int32_t& inout_l,
                 int32_t in_l,
                 int32_t& out_l)
{
  out_l = 931233;
  if (in_l == 0)
    {
      throw A::DidTheRightThing(42, "Hello world");
    }

  if (in_l == in_l_last_)
    {
      out_l = in_l_last_;
    }

  if (!((in_l_ == in_l || in_l_last_ == in_l) && (inout_l_ == inout_l)))
    {
      TAOX11_TEST_ERROR << "ERROR: AMI_Test_i::foo Parameter corruption on in parameters: "
                   << "expected in_l: " << in_l_ << " or " << in_l_last_
                   << " received: " << in_l << ", expected  inout_l: " << inout_l_
                   << " received: " << inout_l << std::endl;
      return 0;
    }
  inout_l = 301;
  return 931234;
}


int32_t
AMI_Test_i::yadda_no_r ()
{
  if (yadda_ == 0)
    {
      throw A::DidTheRightThing(42, "Hello world");
    }
  return yadda_;
}

void
AMI_Test_i::yadda_no_r (int32_t yadda)
{
  if(yadda == 0)
    {
      yadda_ = 0;
      throw A::DidTheRightThing(42, "Hello world");
    }
  yadda_ = (100 + yadda);
}


void
AMI_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
