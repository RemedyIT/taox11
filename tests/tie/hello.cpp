/**
 * @file    hello.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "hello.h"
#include "testC.h"

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
  return Test::regular_foo;
}

std::string
Hello_impl::get_string2 ()
{
  TAOX11_TEST_INFO << "Hello_impl::get_string2()" << std::endl;
  return Test::foo;
}

void
Hello_impl::shutdown ()
{
  this->orb_->shutdown (false);
}

char
Hello_impl::readonly_char ()
{
  return 'a';
}

char
Hello_impl::at_char ()
{
  return 'b';
}

void
Hello_impl::at_char (char)
{
}
