/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 _this test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testlib/taox11_testlog.h"
#include "hello.h"
#include "testlib/taox11_testlog.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb) :
    orb_ (orb)
{
}

Hello::~Hello()
{
  TAOX11_TEST_INFO << "Hello::~Hello()" << std::endl;
}

void
iA_impl::do_something()
{
  TAOX11_TEST_INFO << "iA_impl::do_something called." << std::endl;
}

IDL::traits<Test::iA>::ref_type
Hello::some_function ()
{
  CORBA::servant_traits<Test::iA>::ref_type iA_servant =
  CORBA::make_reference<iA_impl> ();
  iA_servant->do_something ();
  TAOX11_TEST_INFO << "Returning iA_servant->_this." << std::endl;
  return iA_servant->_this ();
}

void
Hello::shutdown()
{
  this->orb_->shutdown (false);
}
