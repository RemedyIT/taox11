/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "test_i.h"

Simple_Server_i::Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb)
 : orb_ (orb)
{
}

bool
Simple_Server_i::test_is_a (const std::string& )
{
  return false;
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
