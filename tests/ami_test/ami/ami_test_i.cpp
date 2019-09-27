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
  :  orb_ (orb),
     yadda_ ((int32_t) 140474),
     in_l_ ((int32_t) 931247),
     in_l_last_ ((int32_t) 931235),
     attr_bool_(true),
     in_str_("Let's talk AMI."),
     in_str_last_("Let's talk AMI the last time.")
{
}

int32_t
AMI_Test_i::foo (int32_t& out_l,
                 int32_t in_l,
                 const std::string& in_str)
{
  out_l = 931233;
  int32_t ret_ = 931234;
  if (in_l == 0)
    {
      throw A::DidTheRightThing(42, "Hello world");
    }

  if (in_l == in_l_last_)
    {
      out_l = in_l_last_;
    }

  if (!((in_l_ == in_l) || (in_l_last_ == in_l)))
    {
      TAOX11_TEST_ERROR << "ERROR:  AMI_Test_i::foo Parameter corruption on in parameters: "
                   << "expected in_l: " << in_l_ << " or " << in_l_last_
                   << " received: " << in_l << std::endl;
      ret_ = 0;
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

void AMI_Test_i::bar (A::enumType &)
{
}

void
AMI_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

int32_t
AMI_Test_i::yadda ()
{
  if (yadda_ == 140474)
   {
     throw CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
   }
  return yadda_;
}

void
AMI_Test_i::yadda (int32_t yadda)
{
  if (yadda == 0)
  {
     throw CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
  }
  yadda_ = yadda;
}

bool
AMI_Test_i::bool_attr()
{
  if(attr_bool_ == true)
    {
      throw A::DidTheRightThing(42, "Hello world");
    }
  return false;
}

void
AMI_Test_i::bool_attr(bool attr_bool)
{
  if(attr_bool == true)
    {
      throw CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
    }
  attr_bool_ = attr_bool;
}

 std::string AMI_Test_i::foo_struct (A::structType &out_t)
 {
   out_t.as(5);
   out_t.bs (6);
   return "bye from foo_struct";
 }
