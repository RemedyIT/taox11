/**
 * @file    test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_i.h"

A_i::A_i (IDL::traits<CORBA::ORB>::ref_type orb)
  :  orb_ (std::move(orb))
{
}

int32_t
A_i::foo (int32_t in_l)
{
  int32_t ret_l;
  if (in_l == 10)
    {
      ret_l = 931235;
    }
  else
    {
      ret_l =  3;
    }
  return ret_l;
}

int32_t
A_i::yadda ()
{
  //Your code here
 return 4;
}

void
A_i::yadda (int32_t )
{
  //Do somethhing
}

void
A_i::shutdown ()
{
  this->orb_->shutdown (false);
}
