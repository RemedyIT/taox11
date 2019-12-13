/**
 * @file    hello.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testlib/taox11_testlog.h"
#include "hello.h"

Hello_impl::Hello_impl (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

Hello_impl::~Hello_impl()
{
  TAOX11_TEST_INFO << "Hello_impl::~Hello_impl()" << std::endl;
}

std::string
Hello_impl::get_string ()
{
  TAOX11_TEST_INFO << "Hello_impl::get_string()" << std::endl;
  return "Hello_impl there!";
}

void
Hello_impl::shutdown ()
{
  this->orb_->shutdown (false);
}
