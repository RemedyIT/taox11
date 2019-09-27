/**
 * @file    test_i.cpp
 * @author  Martin Corino
 *
 * @brief   Implementation for the "test" IDL interface for the
 *          PortableInterceptor::ForwardRequest test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "test_i.h"

#include "testlib/taox11_testlog.h"

test_i::test_i (int16_t num,
                IDL::traits<CORBA::ORB>::ref_type orb)
  : number_ (num),
    orb_ (std::move (orb))
{
}

int16_t
test_i::number ()
{
  return this->number_;
}

void
test_i::shutdown ()
{
  TAOX11_TEST_INFO << "Server is shutting down via object " <<
      this->number_ << std::endl;
  this->orb_->shutdown (false);
}
