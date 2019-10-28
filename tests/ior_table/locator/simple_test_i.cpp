/**
 * @file    simple_test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "simple_test_i.h"

Simple_Test_i::Simple_Test_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

void
Simple_Test_i::shutdown ()
{
  this->orb_->shutdown (false);
}
