/**
 * @file    foo.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "foo.h"

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

bool
Foo::pass_data (const CORBA::Any & s)
{
  uint32_t s_;
  bool result = false;
  if(s>>=s_)
   result = true;
  return result;
}

void Foo::do_it ()
{
  throw Test::FooEx ("Hello world!");
}

void Foo::do_reason ()
{
  throw ReasonEx (Reason::two , false);
}


void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
