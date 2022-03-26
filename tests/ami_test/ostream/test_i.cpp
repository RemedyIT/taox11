/**
 * @file    test_i.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_i.h"

Test_i::Test_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

int32_t
Test_i::foo (int32_t in_l)
{
 return in_l;
}

void
Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
