/**
 * @file    hello.cpp
 * @author  Markijke Hengstmengel
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "hello.h"
#include "testlib/taox11_testlog.h"

Hello::Hello (IDL::traits<CORBA::ORB>::ref_type orb, int & result)
  : orb_ (std::move(orb)), result_ (result)
{
}

void
Hello::shutdown ()
{
  this->orb_->shutdown (false);
}
