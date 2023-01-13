/**
 * @file    foo.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"
#include "testlib/taox11_testlog.h"

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

bool
Foo::pass_data (const CORBA::Any & s) //in
{
  bool result = false;
  if(s>>=this->s_)
   result = true;
  else if (s>>=this->d_)
   result = true;
  return result;
}

CORBA::Any
Foo::return_data (bool str)
{
  CORBA::Any tmp;
  if (str)
    tmp<<=this->s_;
  else
    tmp<<=this->d_;
  return tmp;
}

bool
Foo::get_data (CORBA::Any & s, bool str) //out
{
  if (str)
    s<<=this->s_;
  else
    s<<=this->d_;
  return true;
}

bool
Foo::update_struct (CORBA::Any & s) //inout
{
  Test::Simple t;
  bool result = false;
  if(s>>=t)
  {
    t = this->s_;
    t.c ('B');
    t.b (false);
    t.l (t.l () * 2);
    t.d (t.d () * 2);
    t.e (Test::EEnum::C);
    s<<=t;
    result = true;
  }
  return result;
}

bool
Foo::update_union (CORBA::Any & s) //inout
{
  Test::Data d_in;
  Test::Data d_out;
  bool result = false;
  Test::Point p(20,30);

  if(s>>=d_in)
  {
    if (d_in.LongData() != 1200)
    {
      TAOX11_TEST_ERROR << "ERROR in Foo::update_union for -in- value, expected 1200, received " << d_in.LongData() << std::endl;
    }
    else
    {
      //set -new- union with value in Any.
      d_out.pointData(p);
      s<<=d_out;
      result = true;
    }
  }
  return result;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
