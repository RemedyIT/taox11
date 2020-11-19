// FUZZ: disable check_for_ace_log_categories

#include "foo.h"

Foo::Foo (CORBA::ORB_ptr orb)
  : orb_ (CORBA::ORB::_duplicate (orb))
{
}

bool
Foo::pass_union (const Test::Data & s)
{
  this->s_ = s;

  const Test::Point &pt = this->s_.pointData();
  if (pt.x == 12 && pt.y == 34)
    {
      ACE_DEBUG ((LM_DEBUG, "Foo::pass_union -  correct pointData received.\n"));
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "Foo::pass_union -  ERROR - incorrect pointData received - "
        "expected <Test::Point {x=12,y=34}> - received <Test::Point {x=%d,y=%d}>\n",
        pt.x, pt.y));
    }

  this->s_.longData (123456);
  return true;
}

bool
Foo::pass_default_union (const Test::DefaultData & dd)
{
  if (dd._d () != 2)
  {
    ACE_ERROR ((LM_ERROR, "Foo::pass_default_union - ERROR - Unexpected "
      "discriminant found: expected <2> - found <%d>\n", dd._d ()));
  }
  if (dd.shortData () != 12)
  {
    ACE_ERROR ((LM_ERROR, "Foo::pass_default_union - ERROR - Unexpected "
      "shortData found: expected <12> - found <%d>\n", dd.shortData ()));
  }
  this->dd_ = dd;
  return true;
}

Test::Data *
Foo::return_union ()
{
  return new Test::Data (this->s_);
}

Test::DefaultData
Foo::return_default_union ()
{
  return Test::DefaultData (this->dd_);
}

Test::X_Union *
Foo::return_x_union ()
{
  Test::X_Union_var dx = new Test::X_Union ();
  dx->x_long (10);
  return dx._retn ();
}

Test::Y_Union *
Foo::return_y_union ()
{
  Test::Y_Union_var dy = new Test::Y_Union ();
  dy->y_test ("hello");
  return dy._retn ();
}

Test::Z_Union *
Foo::return_z_union (const Test::Z_Union & z)
{
  // Check incoming union
  if (z._d () != 1)
  {
    ACE_ERROR ((LM_ERROR, "Foo::return_z_union -  ERROR - incorrect discriminant received - "
      "expected <2> - got <%d>\n", z._d ()));
  }
  if (ACE_OS::strcmp(z.z_string (), "Hi") != 0)
  {
    ACE_ERROR ((LM_ERROR, "Foo::return_z_union -  ERROR - incorrect value for z_string received - "
      "expected <\"Hi\"> - got <\"%C\">\n", z.z_string ()));
  }
  Test::Z_Union_var ret = new Test::Z_Union ();
  CORBA::String_var tmp = CORBA::string_dup ("Bye");
  ret->z_string (tmp);
  ret->_d (3);
  return ret._retn ();
}

bool
Foo::get_union (Test::Data_out s)
{
  s = new Test::Data (this->s_);
  Test::Point p = {10, 56};
  s->pointData (p);
  return true;
}

bool
Foo::get_default_union (Test::DefaultData_out dd)
{
  this->dd_.longData (10);
  dd = Test::DefaultData (this->dd_);
  return true;
}

bool
Foo::update_union (Test::Data & s)
{
  s = this->s_;
  Test::Track t = { 99, {23, 77} };
  s.trackData (t);
  return true;
}

bool
Foo::update_default_union (Test::DefaultData & dd)
{
  if (dd.defData () != 123456)
  {
    ACE_ERROR ((LM_ERROR, "Foo::update_union -  ERROR - incorrect defData received - "
      "expected <123456> - received <%d>\n", dd.defData ()));
  }
  dd.longData (99);
  return true;
}

bool
Foo::send_unionmessage (const Test::UnionMessage & /*msg*/)
{
  return true;
}


void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
