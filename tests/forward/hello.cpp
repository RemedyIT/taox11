/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "hello.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb) :
  orb_ (orb)
{
}

IDL::traits<Test::Bar>::ref_type
Hello::get_bar ()
{
  return nullptr;
}

void
Hello::with_bar (
  IDL::traits<Test::Bar>::ref_type bar_in,
  IDL::traits<Test::Bar>::ref_type& bar_out,
  IDL::traits<Test::Bar>::ref_type& bar_inout)
{
  bar_out = bar_in;
  bar_inout = bar_in;
}

void
Hello::shutdown()
{
  this->orb_->shutdown (false);
}
