/**
 * @file    test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "test_i.h"

Test_i::Test_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
  yadda_ = 0;
}

int32_t
Test_i::foo (int32_t in_l)
{
  int32_t ret_l;
  if (in_l == 0)
    {
      TAOX11_TEST_INFO << "Test_i::foo throws the exception ." << std::endl;
      throw A::DidTheRightThing(42, "Hello world");
    }
  if (in_l == 10)
    {
      ret_l = 931235;
    }
  else
    {
      ret_l = in_l;
    }
  return ret_l;
}

int32_t
Test_i::yadda ()
{
 return yadda_;
}

void
Test_i::yadda (int32_t yadda )
{
  yadda_ = yadda;
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
