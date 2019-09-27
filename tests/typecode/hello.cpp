/**
 * @file    hello.cpp
 * @author  Markijke Hengstmengel
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, int & result)
  : orb_ (orb), result_ (result)
{
}

Hello::~Hello()
{
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
