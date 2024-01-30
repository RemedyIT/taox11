/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testlib/taox11_testlog.h"

#include "hello.h"

int
TestEqual (Test::enumType a, Test::enumType exp)
{
  if (a != exp)
    {
      TAOX11_TEST_ERROR << "ERROR: value expected " << exp << " , received "
        << a << std::endl;
      return 1;
    }
  return 0;
}

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, int& result) :
  orb_ (std::move(orb)), result_ (result)
{
}

Test::enumType
Hello::get_enum ()
{
  Test::enumType sds = Test::enumType::b;
  return sds;
}

Test::enumType Hello::inout_enum (Test::enumType pin,
                                  Test::enumType& pout, Test::enumType& pinout)
{
  result_ += TestEqual (pin, Test::enumType::c);
  result_ += TestEqual (pinout, Test::enumType::b);
  pout = pin;
  Test::enumType p = pinout;
  pinout = pin;

  // Compile test ostringstream insertion
  std::ostringstream ss;
  ss << IDL::traits<Test::enumType>::write (pin) << std::endl;

  return p;
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
