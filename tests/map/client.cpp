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
#include "ace/Get_Opt.h"

#include "test_sequences.h"
#include "test_corba_sequences.h"
#include "test_bounded_sequences.h"

bool run_cross_tao_test = false;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("t"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 't':
        run_cross_tao_test = true;
        break;
      default:
        TAOX11_TEST_ERROR << "Run with -t to indicate a cross "
              << "TAO test." << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

template<typename SEQ, typename REF_TYPE>
void test_bounded (IDL::traits<CORBA::Object>::ref_type obj,
  const std::string &type,
  int &error_count)
{
  typename IDL::traits<REF_TYPE>::ref_type ref =
    IDL::traits<REF_TYPE>::narrow (obj);
  if (ref == nullptr)
    {
      TAOX11_TEST_ERROR << "test_bounded - ERROR - Unable to narrow to the given "
        << "ref_type" << std::endl;
      ++error_count;
      return;
    }
  TAOX11_TEST_DEBUG << std::endl;
  // Test in parameter
  SEQ pin (11);
  try
    {
      ref->test_bounded_in (pin);
      TAOX11_TEST_ERROR << "test_bounded - ERROR : No BAD_PARAM exception caught while "
        << "testing the IN param for bounded <" << type << "> sequences" << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      TAOX11_TEST_DEBUG << "test_bounded - OK, BAD_PARAM exception caught while testing "
        << "the IN param for bounded <" << type << "> sequences." << std::endl;
    }
  SEQ pout;
  try
    {
      ref->test_bounded_out (pout);
      TAOX11_TEST_ERROR << "test_bounded - ERROR : No BAD_PARAM exception caught while "
        << "testing the OUT param for bounded <" << type << "> sequences" << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      TAOX11_TEST_DEBUG << "test_bounded - OK, BAD_PARAM exception caught while testing "
        << "the OUT param for bounded <" << type << "> sequences." << std::endl;
    }
  try
    {
      SEQ ret = ref->test_bounded_ret ();
      TAOX11_TEST_ERROR << "test_bounded - ERROR : No BAD_PARAM exception caught while "
        << "testing RETURN param for bounded <" << type << "> sequences" << std::endl;
      ++error_count;
    }
  catch (const CORBA::BAD_PARAM &)
    {
      TAOX11_TEST_DEBUG << "test_bounded - OK, BAD_PARAM exception caught while testing "
        << "the RETURN param for bounded <" << type << "> sequences." << std::endl;
    }
}

int main (int argc, char* argv[])
{
  int error_count = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
      {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
          return 1;
      }

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object ("file://test.ior");

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (obj);

      if (!foo)
        {
          TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (obj) returned null object." << std::endl;
          return 1;
        }
      TAOX11_TEST_DEBUG << "narrowed Foo interface" << std::endl;

      test_sequences (foo, error_count);

      if (!run_cross_tao_test)
        {
          test_bounded_sequences (foo, error_count);
          test_corba_sequences (foo, error_count);

          TAOX11_TEST_DEBUG << std::endl << "main - Testing BAD_PARAM exceptions" << std::endl;
          TAOX11_TEST_DEBUG << "======================================" << std::endl;

          test_bounded<Test::OctetBounded,
            Test::TestOctetBounded::Bounded>(foo->get_octet_bounded (), "octet", error_count);
          test_bounded<Test::ShortBounded,
            Test::TestShortBounded::Bounded>(foo->get_short_bounded (), "short", error_count);
          test_bounded<Test::LongBounded,
            Test::TestLongBounded::Bounded>(foo->get_long_bounded (), "long", error_count);
          test_bounded<Test::LongLongBounded,
            Test::TestLongLongBounded::Bounded>(foo->get_longlong_bounded (), "longlong", error_count);
          test_bounded<Test::DoubleBounded,
            Test::TestDoubleBounded::Bounded>(foo->get_double_bounded (), "double", error_count);
          test_bounded<Test::LongDoubleBounded,
            Test::TestLongDoubleBounded::Bounded>(foo->get_long_double_bounded (), "long double", error_count);
          test_bounded<Test::FloatBounded,
            Test::TestFloatBounded::Bounded>(foo->get_float_bounded (), "float", error_count);
          test_bounded<Test::BoolBounded,
            Test::TestBoolBounded::Bounded>(foo->get_bool_bounded (), "boolean", error_count);
          test_bounded<Test::UShortBounded,
            Test::TestUShortBounded::Bounded>(foo->get_ushort_bounded (), "unsigned short", error_count);
          test_bounded<Test::ULongBounded,
            Test::TestULongBounded::Bounded>(foo->get_ulong_bounded (), "unsigned long", error_count);
          test_bounded<Test::ULongLongBounded,
            Test::TestULongLongBounded::Bounded>(foo->get_ulonglong_bounded (), "unsigned longlong", error_count);
          test_bounded<Test::StringBounded,
            Test::TestStringBounded::Bounded>(foo->get_string_bounded (), "string", error_count);
          test_bounded<Test::CharBounded,
            Test::TestCharBounded::Bounded>(foo->get_char_bounded (), "char", error_count);
          test_bounded<Test::WCharBounded,
            Test::TestWCharBounded::Bounded>(foo->get_wchar_bounded (), "wchar", error_count);
        }

      TAOX11_TEST_DEBUG << std::endl << "shutting down...";
      foo->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const CORBA::BAD_PARAM& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected CORBA::BAD_PARAM exception caught"
        << e << std::endl;
      ++error_count;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "main - ERROR - Unexpected exception caught: " << e << std::endl;
      ++error_count;
    }
  return error_count;
}
