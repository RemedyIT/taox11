/**
 * @file    foo.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

bool
Foo::pass_struct (const Test::Simple & s)
{
  this->s_ = s;
  return true;
}

Test::Simple
Foo::return_struct ()
{
  return this->s_;
}

bool
Foo::get_struct (Test::Simple & s)
{
  s = this->s_;
  return true;
}

bool
Foo::update_struct (Test::Simple & s)
{
  s = this->s_;
  s.b (false);
  s.l (s.l () * 2);
  s.d (s.d () * 3);
  s.e (Test::EEnum::C);
  return true;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
