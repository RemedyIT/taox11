/**
 * @file    foo.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "foo.h"
#include "testlib/taox11_testlog.h"

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

bool
Foo::pass_array (const Test::F& p1,
                 Test::V& /*p2*/,
                 Test::M& /*p3*/)
{
  Test::F expected = { { 1, 2, 3, 4, 5 } };
  this->f_ = p1;

  // Compile test for write on ostringstream
  std::ostringstream ss;
  ss << IDL::traits<Test::F>::write (p1) << std::endl;

  TAOX11_TEST_DEBUG << "pass_array p1: " << p1 <<  std::endl;

  return (p1 == expected);
}

Test::F
Foo::return_array ()
{
  return this->f_;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
