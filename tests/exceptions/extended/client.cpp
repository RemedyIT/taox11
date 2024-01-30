/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"

const std::string STRING_TO_RECEIVE ("This is a string");
const std::string ALMOST_RIGHT ("Almost right");
const std::string UNION_STRING ("This is a union");
const std::string WRONG_NUMBER ("Wrong number");

uint16_t
test_bar_object (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t result = 0;
  try
    {
      foo->do_throw_object();
      TAOX11_TEST_ERROR << "test_bar_object - ERROR : Test::Foo::do_throw_object() "
        << "returned without throwing exception!" << std::endl;
      ++result;
    }
  catch (const Test::BarEx &x)
    {
      uint16_t const prev_result = result;
      IDL::traits<Test::Bar>::ref_type bar = x.bar_i();
      if (!bar)
        {
          TAOX11_TEST_ERROR << "test_bar_object - ERROR : catching a Test::BarEx but "
            << "the reference to 'bar' seems nil" << std::endl;
          ++result;
        }
      else
        {
          try
            {
              bar->do_something ();
              TAOX11_TEST_ERROR << "test_bar_object - ERROR : no StructEx exception thrown"
                << std::endl;
              ++result;
            }
          catch (const Test::StructEx &ex)
            {
              TAOX11_TEST_DEBUG << "test_bar_object - bar->do_something "
                << "Correct exception caught: " << ex << std::endl;
              if (ex.struct_i ().id () != 12345)
                {
                  TAOX11_TEST_ERROR << "test_bar_object - ERROR : Correct exception caught "
                    << "but 'id' seems to contain a wrong value. expected <12345> - "
                    << "found <" << ex.struct_i ().id () << ">" << std::endl;
                  ++result;
                }
              if (ex.struct_i ().msg () != "do_something")
                {
                  TAOX11_TEST_ERROR << "test_bar_object - ERROR : Correct exception caught "
                    << "but 'id' seems to contain a wrong message. expected <do_something> - "
                    << "found <" << ex.struct_i ().msg () << ">" << std::endl;
                  ++result;
                }
              if (std::strcmp(ex._name (), "StructEx") != 0)
                {
                  TAOX11_TEST_ERROR << "test_bar_object - ERROR : Correct exception caught "
                    << "but '_name' seems to contain a wrong value. expected <StructEx> - "
                    << "found <" << ex._name () << ">" << std::endl;
                  ++result;
                }
            }
          catch (...)
            {
              TAOX11_TEST_ERROR << "test_bar_object - ERROR : Unexpected exception "
                << "caught while running do_something" << std::endl;
              ++result;
            }

          int32_t const value = bar->a_bar ();
          if (value != 1234)
            {
              TAOX11_TEST_ERROR << "ERROR: incorrect 'bar' found. found <"
                            << value << "> - expected <1234>"
                            << std::endl;
              ++result;
            }
        }
      if (x.bar_octet () != 15)
        {
          TAOX11_TEST_ERROR << "ERROR: incorrect 'bar_octet' found. found <"
                        << x.bar_octet () << "> - expected <15>"
                        << std::endl;
          ++result;
        }
      const IDL::traits<Test::VT>::ref_type vt = x.bar_valuetype ();
      if (vt->id () != 3)
        {
          TAOX11_TEST_ERROR << "ERROR: incorrect 'id' in valuetype found. found <"
                        << vt->id () << "> - expected <3>"
                        << std::endl;
          ++result;
        }
      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::BarEx: " << x << std::endl;
        }
      else
        {
          // This is a summary: no need to increase result.
          TAOX11_TEST_ERROR << "ERROR catching Test::BarEx: " << x << std::endl;
        }
    }
  return result;
}

uint16_t
test_exceptions (IDL::traits<Test::Foo>::ref_type foo)
{
  uint16_t result = 0;

  // catching UserException Test::FooEx at different levels in the exception hierarchy:
  // as CORBA::UserException, CORBA::Exception, Test::FooEx and std::exception.
  // The info details of the first 3 must be equal and must contain
  // the name Test::FooEx and message 'Hello World'.
  std::stringstream excep_user;
  try
    {
      foo->do_it ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned without throwing exception!"
                    << std::endl;
      ++result;
    }
  catch (const CORBA::UserException &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooEx as "
                    << "CORBA::UserException exception:" << x << std::endl;
      excep_user << x;
    }

  std::stringstream excep_corba;
  try
    {
      foo->do_it ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned without throwing exception!"
                    << std::endl;
      ++result;
    }
  catch (const CORBA::Exception &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooEx as CORBA::Exception "
                    << "exception: " << x << std::endl;
      excep_corba << x;
    }

  std::stringstream excep_fooex;
  try
    {
      foo->do_it ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned without throwing exception!"
                    << std::endl;
      ++result;
    }
  catch (const Test::FooEx &x)
    {
      uint16_t const prev_result = result;

      if (x.union_data()._d() == Test::DataType::dtLong)
        {
          if (x.union_data().longData() != 98765)
            {
              TAOX11_TEST_ERROR << "ERROR do_it : Test::FooEx returned with an invalid "
                            << "longData in union! received <\""
                            << x.union_data().longData()
                            << "\"> - expected <98765>"
                            << std::endl;
              ++result;
            }
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR do_it : Test::FooEx returned with an invalid "
                        << "type in union! received <"
                        << x.union_data()._d()
                        << "> - expected <Test::DataType::dtLong>"
                        << std::endl;
          ++result;
        }
      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooEx: " << x << std::endl;
        }
      else
        {
          // This is a summary: no need to increase result.
          TAOX11_TEST_ERROR << "ERROR catching Test::FooEx: " << x << std::endl;
        }
      excep_fooex << x;
    }

  try
    {
      foo->do_it ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned without throwing exception!"
                    << std::endl;
      ++result;
    }
  catch (const std::exception &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooEx as std::exception: "
                    << x << std::endl;
      try
        {
          throw;
        }
      catch (const Test::FooEx &x_)
        {
          std::stringstream excep_std;
          excep_std << x_;
          std::string excep_str = excep_std.str();
          if (excep_str.find ("FooEx") == std::string::npos)
            {
              TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned wrong std::exception info!"
                            << excep_str << std::endl;
              ++result;
            }
        }
    }

  if ((excep_fooex.str().find ("FooEx") == std::string::npos) ||
      (excep_fooex.str().find ("message") == std::string::npos) ||
      (excep_fooex.str().find ("Hello world!") == std::string::npos))
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() returned wrong exception info!"
                    << std::endl;
      ++result;
    }
  if (excep_fooex.str().find ("\"\"Hello wide") != std::string::npos)
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it() wstring has wrong representation, "
                    << "double quotes!" << std::endl;
      ++result;
    }
  if ((excep_fooex.str() == excep_user.str()) && (excep_fooex.str() == excep_corba.str()))
    {
      TAOX11_TEST_DEBUG << "Successfully received the same information for Test::FooEx, "
                    << "CORBA::UserException, and CORBA::Exception." << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_it(), Test::FooEx exception, CORBA::UserException "
                    << " and CORBA::Exception didn't receive the same information!"
                    << std::endl
                    << " - CORBA::UserException: " << excep_user.str() << std::endl
                    << " - Test::FooEx: " << excep_fooex.str() << std::endl
                    << " - CORBA::Exception: " << excep_corba.str() << std::endl;
      ++result;
    }

  try
    {
      uint32_t num = foo->a_number ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::a_number() returned without throwing "
                    << "exception! " << num << std::endl;
      ++result;
    }
  catch (const Test::BooEx &x)
    {
      uint16_t const prev_result = result;
      if (x.id () != 999)
        {
          TAOX11_TEST_ERROR << "ERROR: a_number : Test::BooEx returned with an invalid id! "
                        << "received <" << x.id() << "> - expected <999>"
                        << std::endl;
          ++result;
        }
      const float expected_value = 9.87f;
      if (x.value () != expected_value)
        {
          TAOX11_TEST_ERROR << "ERROR: a_number : Test::BooEx returned with an invalid value! "
                        << "received <" << x.value() << "> - expected <9.87>"
                        << std::endl;
          ++result;
        }
      const Test::wcharArray wc_array = x.wc_array ();
      uint8_t wch = L'A';
      for (const std::array<wchar_t, 2> &ar1 : wc_array)
        {
          for (const wchar_t &C : ar1)
            {
              if (C != wch)
                {
                  TAOX11_TEST_ERROR << "ERROR: a_number : Test::BooEx returned with an invalid "
                                << "wchar array value! received <'" << C
                                << "'> - expected <'" << wch << "'>"
                                << std::endl;
                  ++result;
                }
              wch++;
            }
        }
      const Test::charArray c_array = x.c_array ();
      uint8_t ch = 'E';
      for (const std::array<char, 2> &ar1 : c_array)
        {
          for (const char &C : ar1)
            {
              if (C != ch)
                {
                  TAOX11_TEST_ERROR << "ERROR: a_number : Test::BooEx returned with an invalid "
                                << "char array value! received <'" << C
                                << "'> - expected <'" << ch << "'>"
                                << std::endl;
                  ++result;
                }
              ch++;
            }
        }

      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::BarEx: " << x << std::endl;
        }
      else
        {
          // This is a summary: no need to increase result.
          TAOX11_TEST_ERROR << "ERROR catching Test::BarEx: " << x << std::endl;
        }
    }

  try
    {
      uint32_t const num = foo->a_number_and_message ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::a_number_and_message() returned without "
                    << "throwing exception! " << num << std::endl;
      ++result;
    }
  catch (const CORBA::UserException &x)
    {
      uint16_t const prev_result = result;
      try
        {
          throw;
        }
      catch (const Test::FooBooEx &x_)
        {
          if (x_.id() != 999)
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "a_number_and_message : Test::FooBooEx: "
                            << "invalid id received <" << x_.id()
                            << "> : expected <999>"
                            << std::endl;
              ++result;
            }
          if (x_.message().find (WRONG_NUMBER) == std::string::npos)
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "a_number_and_message : Test::FooBooEx: "
                            << "invalid message received <\""
                            << x_.message() << "\"> : expected <\""
                            << WRONG_NUMBER << "\">" << std::endl;
              ++result;
            }
          const CORBA::Any any = x_.x();
          IDL::traits<CORBA::TypeCode>::ref_type type_any = any.type();
          if (type_any->kind() != CORBA::TCKind::tk_long)
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "a_number_and_message : Test::FooBooEx: "
                            << "invalid any type received <"
                            << type_any->kind() << "> : expected <"
                            << "CORBA::TCKind::tk_long>" << std::endl;
              ++result;
            }
          int32_t value;
          if (any >>= value)
            {
              if (value != 4321)
                {
                  TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                                << "a_number_and_message : Test::FooBooEx: "
                                << "invalid any value received <" << value
                                << "> : expected <4321>"
                                << std::endl;
                  ++result;
                }
            }
          else
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "a_number_and_message : Test::FooBooEx: "
                            << "Unable to retrieve long from any"
                            << std::endl;
              ++result;
            }
        }
      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooBooEx as "
                        << "CORBA::UserException exception: " << x << std::endl;
        }
      else
        {
          // This is a summary: No need to increase the result.
          TAOX11_TEST_ERROR << "ERROR: a_number_and_message : catching Test::FooBooEx "
                        << "as CORBA::UserException: " << x << std::endl;
        }
    }

  try
    {
      uint32_t const num = foo->a_number_and_message ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::a_number_and_message() returned without "
                    << "throwing exception! " << num << std::endl;
      ++result;
    }
  catch (const Test::FooBooEx &x)
    {
      uint16_t const prev_result = result;
      if (x.id() != 999)
        {
          TAOX11_TEST_ERROR << "ERROR a_number_and_message : Test::FooBooEx: "
                        << "invalid id received <" << x.id()
                        << "> : expected <999>" << std::endl;
          ++result;
        }
      if (x.message().find (WRONG_NUMBER) == std::string::npos)
        {
          TAOX11_TEST_ERROR << "ERROR a_number_and_message : catching Test::FooBooEx: "
                        << "invalid message received <"
                        << x.message() << "> : expected <\"" << WRONG_NUMBER
                        << "\">" << std::endl;
          ++result;
        }
      const CORBA::Any any = x.x();
      IDL::traits<CORBA::TypeCode>::ref_type type_any = any.type();
      if (type_any->kind() != CORBA::TCKind::tk_long)
        {
          TAOX11_TEST_ERROR << "ERROR a_number_and_message : catching Test::FooBooEx: "
                        << "invalid any type received <" << type_any->kind()
                        << "> : expected <CORBA::TCKind::tk_long>"
                        << std::endl;
          ++result;
        }
      int32_t value;
      if (any >>= value)
        {
          if (value != 4321)
            {
              TAOX11_TEST_ERROR << "ERROR a_number_and_message : catching Test::FooBooEx: "
                            << "invalid any value received <" << value
                            << "> : expected <4321>"
                            << std::endl;
              ++result;
            }
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR a_number_and_message : catching Test::FooBooEx: "
                        << "Unable to retrieve long from any"
                        << std::endl;
          ++result;
        }

      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::FooBooEx as "
                        << "Test::FooBooEx exception: " << x << std::endl;
        }
      else
        {
          // This is a summary. No need to increase result.
          TAOX11_TEST_ERROR << "ERROR catching Test::FooBooEx: " << x << std::endl;
        }
    }

  try
    {
      std::string const str = foo->a_string ();
      if (str.find (STRING_TO_RECEIVE) == std::string::npos)
        {
          TAOX11_TEST_ERROR << "ERROR a_string : received an unexpected string: "
                        << "received <\"" << str << "\"> - expected <\""
                        << STRING_TO_RECEIVE << "\">"
                        << std::endl;
          ++result;
        }
      else
        {
          TAOX11_TEST_DEBUG << "Successfully called Test::Foo::a_string (): "
                        << "{\"" << str << "\"}" << std::endl;
        }
    }
  catch (const CORBA::Exception &x)
    {
      TAOX11_TEST_ERROR << "ERROR: caught unexpected exception after calling "
                    << "Test::Foo::a_string (): " << x << std::endl;
      ++result;
    }

  try
    {
      foo->a_string ("bye bye");
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::a_string(\"bye bye\") returned without "
                    << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const Test::BooEx &x)
    {
      uint16_t const prev_result = result;
      if (x.id () != 777)
        {
          TAOX11_TEST_ERROR << "ERROR: a_string : catching Test::BooEx: "
                        << "invalid id received <"
                        << x.id() << "> : expected <999>"
                        << std::endl;
          ++result;
        }
      float const expected_value = 7.89f;
      if (x.value () != expected_value)
        {
          TAOX11_TEST_ERROR << "ERROR: a_string : catching Test::BooEx: "
                        << "invalid value received <" << x.value()
                        << "> : expected <7.89>"
                        << std::endl;
          ++result;
        }
      Test::wcharArray const wc_array = x.wc_array ();
      uint8_t wch = L'I';
      for (const std::array<wchar_t, 2> &ar1 : wc_array)
        {
          for (const wchar_t &C : ar1)
            {
              if (C != wch)
                {
                  TAOX11_TEST_ERROR << "ERROR: a_string : catching Test::BooEx: "
                                << "invalid wchar array value received <'" << C
                                << "'> : expected <'" << wch << "'>"
                                << std::endl;
                  ++result;
                }
              ++wch;
            }
        }
      Test::charArray const c_array = x.c_array ();
      uint8_t ch = 'M';
      for (const std::array<char, 2> &ar1 : c_array)
        {
          for (const char &C : ar1)
            {
              if (C != ch)
                {
                  TAOX11_TEST_ERROR << "ERROR: a_string : catching Test::BooEx: "
                                << "invalid char array value received <'" << C
                                << "'> : expected <'" << ch << "'>"
                                << std::endl;
                  ++result;
                }
              ++ch;
            }
        }

      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::BooEx: " << x << std::endl;
        }
      else
        {
          // This is a summary. No need to increase result
          TAOX11_TEST_ERROR << "ERROR catching Test::BooEx: " << x << std::endl;
        }
    }

  try
    {
      foo->do_reason ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_reason() returned without throwing "
                    << "an exception!" << std::endl;
      ++result;
    }
  catch (const CORBA::UserException &)
    {
      try
        {
          throw;
        }
      catch (const Test::ReasonEx &x)
        {
          uint16_t const prev_result = result;

          if (x.why() != Test::Reason::two)
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "do_reason : Test::ReasonEx: "
                            << "invalid value for why received <" << x.why()
                            << "> : expected <Test::Reason::two>"
                            << std::endl;
              ++result;
            }
          if (x.reason_bool() != false)
            {
              TAOX11_TEST_ERROR << "ERROR Test::ReasonEx returned with an invalid "
                            << "reason_bool! received <" << x.reason_bool()
                            << "> - expected <false>" << std::endl;
              ++result;
            }
          if (x.union_data()._d() == Test::DataType::dtString)
            {
              if (x.union_data().stringData().compare (UNION_STRING) != 0)
                {
                  TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                                << "do_reason : Test::ReasonEx: "
                                << "invalid stingData in union received <\""
                                << x.union_data().stringData()
                                << "\"> : expected <\"" << UNION_STRING << "\">"
                                << std::endl;
                  ++result;
                }
            }
          else
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "do_reason : Test::ReasonEx: "
                            << "invalid type in union received <"
                            << x.union_data()._d()
                            << "> : expected <Test::DataType::dtString>"
                            << std::endl;
              ++result;
            }

          if (prev_result == result)
            {
              TAOX11_TEST_DEBUG << "Successfully caught expected Test::ReasonEx as "
                            << "CORBA::UserException exception: " << x << std::endl;
            }
          else
            {
              // This is a summary. No need to increase result
              TAOX11_TEST_ERROR << "ERROR catching Test::ReasonEx as CORBA::UserException: " << x << std::endl;
            }
        }
    }

  // catching SystemException CORBA::NO_IMPLEMENT at different levels in the exception hierarchy:
  // as CORBA::SystemException, CORBA::Exception, CORBA::NO_IMPLEMENT and std::exception.
  // The info details of the first 3 must be equal and must contain
  // the name NO_IMPLEMENT, "Minor code : 8" and "Completed : NO" .
  std::stringstream excep_corba_s;
  try
    {
      foo->do_throw_system ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system() returned without "
                    << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const CORBA::Exception &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected CORBA::NO_IMPLEMENT as "
                    << "CORBA::Exception " << std::endl << "  " <<  x
                    << std::endl;
      excep_corba_s << x;
    }

  std::stringstream excep_system;
  try
    {
      foo->do_throw_system ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system() returned without "
                    << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const CORBA::SystemException &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected CORBA::NO_IMPLEMENT as "
                    << "CORBA::SystemException: " << x
                    << std::endl;
      excep_system << x;
    }

  std::stringstream excep_no_impl;
  try
    {
      foo->do_throw_system ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system() returned without "
                    << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const CORBA::NO_IMPLEMENT &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected CORBA::NO_IMPLEMENT as "
                    << "CORBA::NO_IMPLEMENT ()" << std::endl << "  " << x
                    << std::endl;
      excep_no_impl << x;
    }

  try
    {
      foo->do_throw_system ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system() returned without "
                    << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const CORBA::Exception &x)
    {
      TAOX11_TEST_DEBUG << "Successfully caught expected CORBA::NO_IMPLEMENT as "
                    << "CORBA::Exception: " << std::endl << "  " << x
                    << std::endl;
      std::stringstream excep_std;
      excep_std << x;
      std::string excep_str = excep_std.str();
      if (excep_str.find ("NO_IMPLEMENT") == std::string::npos)
        {
          TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system() returned wrong "
                        << "std::exception info! -" << excep_str << std::endl;
          ++result;
        }
    }
  if ((excep_no_impl.str().find ("NO_IMPLEMENT") == std::string::npos) ||
      (excep_no_impl.str().find ("Minor code : 8") == std::string::npos) ||
      (excep_no_impl.str().find ("Completed : NO") == std::string::npos))
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system() returned wrong exception info!"
                  << std::endl;
      ++result;
    }

  if ((excep_system.str() == excep_corba_s.str()) && (excep_system.str() == excep_no_impl.str()))
    {
      TAOX11_TEST_DEBUG << "Successfully received the same information for NO_IMPLEMENT, "
                    << "CORBA::SystemException, and CORBA::Exception."
                    << std::endl;
    }
  else
    {
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_throw_system, Test::FooEx NO_IMPLEMENT, "
                    << "CORBA::SystemException, and CORBA::Exception. didn't receive "
                    << "the same information!"
                    << std::endl
                    << " - CORBA::SystemException: " << excep_system.str() << std::endl
                    << " - CORBA::NO_IMPLEMENT: " << excep_no_impl.str() << std::endl
                    << " - CORBA::Exception: " << excep_corba_s.str() << std::endl;
      ++result;
    }

  try
    {
      foo->do_throw_struct ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_thow_struct() returned without "
                    << "throwing exception!" << std::endl;
      ++result;
    }
  catch (const CORBA::UserException &)
    {
      try
        {
          throw;
        }
      catch (const Test::StructEx &x_)
        {
          uint16_t const prev_result = result;

          if (x_.struct_i().id() != 888)
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "do_throw_struct : Test::StructEx: "
                            << "invalid id received <" << x_.struct_i().id()
                            << "> : expected <888>" << std::endl;
              ++result;
            }
          if (x_.struct_i().msg().find (ALMOST_RIGHT) == std::string::npos)
            {
              TAOX11_TEST_ERROR << "ERROR catching a CORBA::UserException: "
                            << "do_throw_struct : Test::StructEx: "
                            << "invalid message received <\"" << x_.struct_i().msg()
                            << "\"> : expected <\"" << ALMOST_RIGHT << "\">"
                            << std::endl;
              ++result;
            }
          if (prev_result == result)
            {
              TAOX11_TEST_DEBUG << "Successfully caught expected Test::StructEx as "
                            << "CORBA::UserException exception: " << x_ << std::endl;
            }
          else
            {
              // This is a summary. No need to increase result.
              TAOX11_TEST_ERROR << "ERROR catching Test::StructEx as CORBA::UserException: " << x_ << std::endl;
            }
        }
    }

  try
    {
      foo->do_throw_struct ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_thow_struct() returned without throwing exception!"
                    << std::endl;
      ++result;
    }
  catch (const Test::StructEx &x)
    {
      uint16_t const prev_result = result;

      if (x.struct_i().id() != 888)
        {
          TAOX11_TEST_ERROR << "ERROR do_throw_struct : catching a Test::StructEx exception: "
                        << "invalid id received <" << x.struct_i().id()
                        << "> : expected <888>" << std::endl;
          ++result;
        }
      if (x.struct_i().msg().find (ALMOST_RIGHT) == std::string::npos)
        {
          TAOX11_TEST_ERROR << "ERROR do_throw_struct : catching a Test::StructEx exception: "
                        << "invalid message received <\"" << x.struct_i().msg()
                        << "\"> : expected <\"" << ALMOST_RIGHT << "\">"
                        << std::endl;
          ++result;
        }
      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::StructEx exception: " << x << std::endl;
        }
      else
        {
          // This is a summary. No need to increase result
          TAOX11_TEST_ERROR << "ERROR catching Test::StructEx: " << x << std::endl;
        }
    }

  try
    {
      foo->do_throw_structs ();
      TAOX11_TEST_ERROR << "ERROR: Test::Foo::do_thow_structs() returned without "
                    << "throwing an exception!" << std::endl;
      ++result;
    }
  catch (const Test::StructsEx &x)
    {
      uint16_t const prev_result = result;

      if (x.structs().size() != 5)
        {
          TAOX11_TEST_ERROR << "ERROR do_throw_structs : catching Test::StructsEx: "
                        << "invalid length received <" << x.structs().size()
                        << "> : expected <5>" << std::endl;
          ++result;
        }
      else
        {
          for (uint16_t i = 0; i < 5; ++i)
            {
              Test::MessageStruct msg = x.structs()[i];
              std::stringstream nr ("");
              nr << i;
              if (msg.id() != i)
                {
                  TAOX11_TEST_ERROR << "ERROR do_throw_structs : catching Test::StructEx: "
                                << "invalid id received <"
                                << msg.id() << "> : expected <" << i << ">"
                                << std::endl;
                  ++result;
                }
              if (msg.msg().compare (nr.str ()) != 0)
                {
                  TAOX11_TEST_ERROR << "ERROR do_throw_structs : catching Test::StructEx :"
                                << "invalid message received <\"" << msg.msg()
                                << "\"> : expected <\"" << i << "\">"
                                << std::endl;
                  ++result;
                }
            }
        }
      if (prev_result == result)
        {
          TAOX11_TEST_DEBUG << "Successfully caught expected Test::StructsEx exception: " << x << std::endl;
        }
      else
        {
          // This is a summary. No need to increase result
          TAOX11_TEST_ERROR << "ERROR catching Test::StructsEx: " << x << std::endl;
        }
    }

  TAOX11_TEST_DEBUG << "Testing zero initialization" << std::endl;
  Test::ArrayEx aex;
  uint16_t default_uint16_t {};

  for (const auto& l_array_member : aex.l_array()) {
    if (l_array_member != default_uint16_t) {
      TAOX11_TEST_ERROR << "Exception member l_array not value initialized, " << l_array_member << " instead of " << default_uint16_t << std::endl;
      ++result;
    }
  }

  return result;
}

int
main (int argc, char* argv[])
{
  uint16_t errors = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!orb)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                       << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj =
        orb->string_to_object ("file://test.ior");

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object."
                       << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "narrowed Foo interface. Starting tests" << std::endl;

      IDL::traits<Test::VT>::factory_ref_type vt_factory =
        CORBA::make_reference<Test::VT_init>();
      orb->register_value_factory (vt_factory->_obv_repository_id(),
                                   vt_factory);
      TAOX11_TEST_DEBUG << "Registered factory with id: "
        << vt_factory->_obv_repository_id() << std::endl;


      errors = test_exceptions (foo);
      errors += test_bar_object (foo);

      TAOX11_TEST_DEBUG << "shutting down..." << std::endl;

      foo->shutdown ();
      orb->destroy ();

      TAOX11_TEST_DEBUG << "<" << errors
                    << "> problem(s) were found." << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }
  return errors;
}
