/**
 * @file    test_i.cpp
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "test_i.h"

test_i::test_i (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move (orb))
{
}

void
test_i::shutdown ()
{
  TAOX11_TEST_INFO << "\"test_i\" is shutting down." << std::endl;
  this->orb_->shutdown (false);
}
