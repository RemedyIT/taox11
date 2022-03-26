/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_i.h"

Simple_Server_i::Simple_Server_i (IDL::traits<CORBA::ORB>::ref_type orb)
  :  orb_ (std::move(orb))
{
}

void
Simple_Server_i::test_method ()
{
}

void
Simple_Server_i::shutdown ()
{
  this->orb_->shutdown (false);
}
