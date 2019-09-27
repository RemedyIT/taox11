/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testlib/taox11_testlog.h"
#include "hello.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

Hello::~Hello()
{
  TAOX11_TEST_INFO << "Hello::~Hello()" << std::endl;
}

std::string
Hello::get_string ()
{
  TAOX11_TEST_INFO << "Hello::get_string()" << std::endl;
  return "Hello there!";
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
