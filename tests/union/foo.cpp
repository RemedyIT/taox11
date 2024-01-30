/**
 * @file    foo.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"
#include "testlib/taox11_testlog.h"

#include "checks.h"

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (std::move(orb))
{
}

uint16_t
Foo::get_error_count() const
{
  return errors_;
}

bool
Foo::pass_union (const Test::Data& s)
{
  this->s_ = s;

  check_union (s, Test::DataType::dtPoint, "Foo::pass_union");

  if (this->s_.pointData ().x () != 12 || this->s_.pointData ().y () != 34)
  {
    TAOX11_TEST_ERROR << "Foo::pass_union - ERROR - incorrect pointData received - "
      << "expected <Test::Point {x=12,y=34}> - received <"
      << s.pointData () << ">" << std::endl;
    ++this->errors_;
  }

  this->s_.longData (123456);
  check_union (this->s_, Test::DataType::dtLong, "Foo::pass_union");
  return true;
}

bool
Foo::pass_default_union (const Test::DefaultData & dd)
{
  if (dd._d () != 2)
  {
    TAOX11_TEST_ERROR << "Foo::pass_default_union - ERROR - Unexpected "
      << "discriminant found: expected <2> - found <" << dd._d ()
      << ">" << std::endl;
    ++this->errors_;
  }
  else if (dd.shortData () != 12)
  {
    TAOX11_TEST_ERROR << "Foo::pass_default_union - ERROR - Unexpected "
      << "shortData found: expected <12> - found <" << dd.shortData ()
      << ">" << std::endl;
    ++this->errors_;
  }
  this->dd_ = dd;
  check_default_union (this->dd_, 2, "Foo::pass_default_union");
  return true;
}

Test::Data
Foo::return_union ()
{
  return this->s_;
}

Test::DefaultData
Foo::return_default_union ()
{
  return this->dd_;
}

Test::X_Union
Foo::return_x_union ()
{
  Test::X_Union xd;
  xd.x_long(10);
  return xd;
}

Test::Y_Union
Foo::return_y_union ()
{
  Test::Y_Union yd;
  yd.y_test("hello");
  return yd;
}

Test::Z_Union
Foo::return_z_union (const Test::Z_Union &z)
{
  // Check incoming union
  if (z._d () != 1)
  {
    TAOX11_TEST_ERROR << "Foo::return_z_union -  ERROR - incorrect discriminant received - "
      << "expected <2> - got <" << z._d () << ">." << std::endl;
    ++this->errors_;
  }
  if (z.z_string () != "Hi")
  {
    TAOX11_TEST_ERROR << "Foo::return_z_union -  ERROR - incorrect value for z_string received - "
      << "expected <\"Hi\"> - got <" << z.z_string () << ">." << std::endl;
    ++this->errors_;
  }
  Test::Z_Union ret;
  ret.z_string ("Bye");
  ret._d (3);
  return ret;
}

bool
Foo::get_union (Test::Data & s)
{
  s = this->s_;

  s.pointData (Test::Point (10, 56));
  check_union (s, Test::DataType::dtPoint, "Foo::get_union");
  return true;
}

bool
Foo::get_default_union (Test::DefaultData & dd)
{
  this->dd_.longData (10);
  dd = this->dd_;
  check_default_union (this->dd_, 1, "Foo::get_default_union");
  check_default_union (dd, 1, "Foo::get_default_union");
  return true;
}

bool
Foo::update_union (Test::Data & s)
{
  check_union (s, Test::DataType::dtPoint, "Foo::update_union");
  if (s.pointData ().x () != 10 || s.pointData ().y () != 56)
  {
    TAOX11_TEST_ERROR << "Foo::update_union -  ERROR - incorrect pointData received - "
      << "expected <Test::Point {x=12,y=34}> - received <"
      <<  s.pointData () << ">" << std::endl;
    ++this->errors_;
  }
  s = this->s_;
  s.trackData (Test::Track (99, Test::Point (23, 77)));
  check_union (s, Test::DataType::dtTrack, "Foo::update_union");
  return true;
}

bool
Foo::update_default_union (Test::DefaultData & dd)
{
  // Compile test ostringstream insertion
  std::ostringstream ss;
  ss << IDL::traits<Test::DefaultData>::write (dd) << std::endl;

  if (dd.defData () != 123456)
  {
    TAOX11_TEST_ERROR << "Foo::update_union -  ERROR - incorrect defData received - "
      << "expected <123456> - received <" <<  dd.defData () << ">" << std::endl;
    ++this->errors_;
  }
  dd.longData (99);
  check_default_union (dd, 1, "Foo::update_default_union");
  return true;
}

bool
Foo::send_unionmessage (const Test::UnionMessage & msg)
{
  TAOX11_TEST_DEBUG << "Received <" << msg << ">" << std::endl;

  if (msg.a () != Test::Assignment::D)
  {
    TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - incorrect a received - "
      << "expected <D> - received <" <<  msg.a () << ">" << std::endl;
    ++this->errors_;
  }

  try
  {
    if (msg.b_03 ()._d () != 4)
    {
      TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - "
        << "unexpected value for the discriminator detected. Expected <4> - got <"
        << msg.b_03 ()._d () << ">." << std::endl;
      ++this->errors_;
    }

    if (msg.b_03 ().b_03_5 () != "XX")
    {
      TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - "
        << "unexpected value for the last member detected. Expected <XX> - got <"
        << msg.b_03 ().b_03_5 () << ">." << std::endl;
      ++this->errors_;
    }
  }
  catch (const CORBA::BAD_PARAM&)
  {
    TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - "
      << "Caught unexpected BAD_PARAM exception whilst examining <b_03>." << std::endl;
    ++this->errors_;
  }

  // last member of d
  try
  {
    if (msg.d ()._d () != Test::DataType::dtGlobal)
    {
      TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - "
        << "unexpected value for the discriminator detected. Expected <Test::DataType::dtGlobal> - got <"
        << msg.d ()._d () << ">." << std::endl;
      ++this->errors_;
    }

    if (msg.d ().globalData ().x () != 4)
    {
      TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - "
        << "unexpected value for the last member detected. Expected <4> - got <"
        << msg.d ().globalData ().x () << ">." << std::endl;
      ++this->errors_;
    }
  }
  catch (const CORBA::BAD_PARAM&)
  {
    TAOX11_TEST_ERROR << "Foo::send_unionmessage - ERROR - "
      << "Caught unexpected BAD_PARAM exception whilst examining <d>." << std::endl;
    ++this->errors_;
  }

  return this->errors_ == 0;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
