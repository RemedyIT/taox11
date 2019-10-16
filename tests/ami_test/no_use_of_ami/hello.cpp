/**
 * @file    hello.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "hello.h"

B_i::B_i(IDL::traits<CORBA::ORB>::ref_type orb) :
  orb_(orb)
{
}

int32_t
B_i::op_B (int32_t in_l)
{
  return (in_l + 100);
}

void B_i::shutdown()
{
  this->orb_->shutdown(false);
}

