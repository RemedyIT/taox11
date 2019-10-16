/**
 * @file    foo.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"
#include "testlib/taox11_testlog.h"

void
Bar::a_bar (int32_t)
{
}


int32_t
Bar::a_bar ()
{
  return 1234;
}

void
Bar::do_something ()
{
  throw Test::StructEx (Test::MessageStruct (12345, "do_something"));
}

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<Test::Bar>::ref_type bar)
  : orb_ (orb)
  , bar_ (bar)
{
}

void Foo::do_it ()
{
  Test::Data dt;
  dt.stringData ("This is a union");
  dt.shortData (1234);
  dt.longData (98765);
  throw Test::FooEx ("Hello world!", L"Hello wide world", dt);
}

int32_t Foo::a_number()
{
  const Test::wcharArray wc_array
    {{
      {{L'A', L'B'}},
      {{L'C', L'D'}}
    }};
  const Test::charArray c_array
    {{
      {{'E', 'F'}},
      {{'G', 'H'}}
    }};
  throw Test::BooEx (999, 9.87f, wc_array, c_array);
}

int32_t Foo::a_number_and_message()
{
  CORBA::Any any;
  any <<= 4321;
  throw Test::FooBooEx (999, "Wrong number", any);
}

std::string Foo::a_string()
{
  return "This is a string";
}

void Foo::a_string(const std::string& /*_v*/)
{
  const Test::wcharArray wc_array
    {{
      {{L'I', L'J'}},
      {{L'K', L'L'}}
    }};
  const Test::charArray c_array
    {{
      {{'M', 'N'}},
      {{'O', 'P'}}
    }};
  throw Test::BooEx (777, 7.89f, wc_array, c_array);
}

void Foo::do_reason ()
{
  Test::Data dt;
  dt.longData (98765);
  dt.shortData (1234);
  dt.stringData ("This is a union");
  throw Test::ReasonEx (Test::Reason::two , false, dt);
}

void Foo::do_throw_system ()
{
  throw  CORBA::NO_IMPLEMENT ( 8, CORBA::COMPLETED_NO);
}

void Foo::do_throw_object ()
{
  const IDL::traits<Test::VT>::ref_type vt =
    CORBA::make_reference < IDL::traits<Test::VT>::obv_type > (3);
  throw Test::BarEx (this->bar_, 15, vt);
}

void Foo::do_throw_struct ()
{
  try
  {
    this->bar_->do_something ();
  }
  catch (const Test::StructEx &ex)
  {
    TAOX11_TEST_DEBUG << "Foo::do_throw_struct - "
      << "Correct exception caught : " << ex << std::endl;
    if (ex.struct_i ().id () != 12345)
      {
        TAOX11_TEST_ERROR << "Foo::do_throw_struct - ERROR : Correct exception caught "
          << "but 'id' seems to contain a wrong value. expected <12345> - "
          << "found <" << ex.struct_i ().id () << ">" << std::endl;
      }
    if (ex.struct_i ().msg () != "do_something")
      {
        TAOX11_TEST_ERROR << "Foo::do_throw_struct - ERROR : Correct exception caught "
          << "but 'id' seems to contain a wrong message. expected <do_something> - "
          << "found <" << ex.struct_i ().msg () << ">" << std::endl;
      }
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "Foo::do_throw_struct - ERROR : "
      << "Unexpected exception caught: " << ex.what() << std::endl;
    throw;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "Foo::do_throw_struct - ERROR : "
      << "Unexpected exception caught" << std::endl;
    throw;
  }

  throw Test::StructEx (Test::MessageStruct (888, "Almost right"));
}

void Foo::do_throw_structs ()
{
  Test::Messages messages(5);
  for (uint16_t i = 0; i < 5; ++i)
    {
      std::stringstream str ("");
      str << i;
      Test::MessageStruct msg(i, str.str ());
      messages[i] = msg;
    }
  throw Test::StructsEx (messages);
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
