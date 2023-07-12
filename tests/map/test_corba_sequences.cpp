/**
 * @file    test_corba_sequences.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "test_corba_sequences.h"

#include "testlib/taox11_testlog.h"
#include <cmath>

template<class T>
bool
TestEqual(T exp, T a)
{
  return a == exp;
}

// perform special compare here to cover for the possible conversion losses
// where the native long double format (< 16 bytes) does not match the CORBA
// CDR format (16 bytes)
template<>
bool
TestEqual(long double _exp, long double a)
{
  return !(std::fabs (_exp - a) > std::numeric_limits<long double>::epsilon ());
}

void
check_length_inout (const Test::CorbaSequences &cs,
  int &error_count)
{
  if (cs.wchar_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout wchar sequence detected. expected <11> - found <"
        << cs.wchar_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.char_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout char sequence detected. expected <11> - found <"
        << cs.char_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.wstring_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR_W << "check_length_inout - ERROR : unexpected size of the "
        << "inout wstring sequence detected. expected <11> - found <"
        << cs.wstring_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.string_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout string sequence detected. expected <11> - found <"
        << cs.string_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.short_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout short sequence detected. expected <11> - found <"
        << cs.short_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.ushort_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout ushort sequence detected. expected <11> - found <"
        << cs.ushort_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.long_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout long sequence detected. expected <11> - found <"
        << cs.long_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.ulong_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout ulong sequence detected. expected <11> - found <"
        << cs.ulong_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.longlong_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout longlong sequence detected. expected <11> - found <"
        << cs.longlong_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.ulonglong_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout ulonglong sequence detected. expected <11> - found <"
        << cs.ulonglong_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.double_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout double sequence detected. expected <11> - found <"
        << cs.double_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.longdouble_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout longdouble sequence detected. expected <11> - found <"
        << cs.longdouble_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.float_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout float sequence detected. expected <11> - found <"
        << cs.float_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.octet_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout octet sequence detected. expected <11> - found <"
        << cs.octet_seq ().size () << ">" << std::endl;
      ++error_count;
    }
  if (cs.bool_seq ().size () != 11)
    {
      TAOX11_TEST_ERROR << "check_length_inout - ERROR : unexpected size of the "
        << "inout bool sequence detected. expected <11> - found <"
        << cs.bool_seq ().size () << ">" << std::endl;
      ++error_count;
    }
}

void
fill_sequences (Test::CorbaSequences &cs)
{
  static const wchar_t* wc_data = L"ABCDEFGHIJ";

  cs.short_seq().resize (10);
  for (int i = 0; i < 10; ++i)
    {
      cs.wchar_seq().push_back (wc_data[i]);
      cs.char_seq().push_back (i+32);

      cs.wstring_seq ().push_back (L"wstr-hi");
      cs.string_seq ().push_back ("str-hi");

      if ((i%2) == 0)
        {
          cs.short_seq ()[i] = -3*i;
        }
      cs.ushort_seq ().push_back (3*i);
      cs.long_seq ().push_back (-2147483647+i);
      cs.ulong_seq ().push_back (2147483647 - 1 - i);
      cs.longlong_seq ().push_back (-9223372036854775807+i);
      cs.ulonglong_seq ().push_back (9223372036854775807 - 1 - i);
      cs.double_seq ().push_back (std::pow(2, 127) - 1 - i);
      cs.longdouble_seq ().push_back (i*std::pow (2, 10));
      cs.float_seq ().push_back ((i/3.21f)*(i*100.44321f)/6.3f);
      cs.octet_seq ().push_back (i*1000);
      cs.bool_seq ().push_back (true);
    }

}

void
check_values (const Test::CorbaSequences &in,
  const Test::CorbaSequences& check,
  int increase,
  int &error_count)
{
  for (CORBA::WCharSeq::size_type i = 0; i < 10; ++i)
    {
      char c = in.wchar_seq ()[i] + increase;
      if (check.wchar_seq ()[i] != c)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the wchar out sequence found : "
            << "expected <" << c << "> - found <"
            << check.wchar_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::CharSeq::size_type i = 0; i < 10; ++i)
    {
      char c = in.char_seq ()[i] + increase;
      if (check.char_seq ()[i] != c)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the char out sequence found : "
            << "expected <" << c << "> - found <"
            << check.char_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::WStringSeq::size_type i = 0; i < 10; ++i)
    {
      std::wstring wstr = in.wstring_seq ()[i];
      wstr.append (L"-boo");
      if (check.wstring_seq ()[i] != wstr)
        {
          TAOX11_TEST_ERROR_W << "check_out_values - ERROR : unexpected value in "
            << "the wstring out sequence found : "
            << "expected <" << wstr << "> - found <"
            << check.wstring_seq()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::StringSeq::size_type i = 0; i < 10; ++i)
    {
      std::string str = in.string_seq ()[i];
      str.append ("-hi");
      if (check.string_seq ()[i] != str)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the string out sequence found : "
            << "expected <" << str << "> - found <"
            << check.string_seq()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::ShortSeq::size_type i = 0; i < 10; ++i)
    {
      int16_t expected  = in.short_seq ()[i];
      if ((i%2) == 0)
        {
          expected = std::abs (in.short_seq ()[i]);
        }
      if (expected != check.short_seq ()[i])
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the short out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.short_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::UShortSeq::size_type i = 0; i < 10; ++i)
    {
      uint16_t expected = in.ushort_seq ()[i] + increase;
      if (check.ushort_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the ushort out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.ushort_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::LongSeq::size_type i = 0; i < 10; ++i)
    {
      int32_t expected = in.long_seq ()[i] + increase;
      if (check.long_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the long out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.long_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::ULongSeq::size_type i = 0; i < 10; ++i)
    {
      uint32_t expected = in.ulong_seq ()[i] + increase;
      if (check.ulong_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the ulong out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.ulong_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::LongLongSeq::size_type i = 0; i < 10; ++i)
    {
      int64_t expected = in.longlong_seq ()[i] + increase;
      if (check.longlong_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the long long out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.longlong_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::ULongLongSeq::size_type i = 0; i < 10; ++i)
    {
      uint64_t expected = in.ulonglong_seq ()[i] + increase;
      if (check.ulonglong_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the unsigned long long out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.ulonglong_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::DoubleSeq::size_type i = 0; i < 10; ++i)
    {
      double expected = in.double_seq ()[i] + increase;
      if (!TestEqual<double> (check.double_seq ()[i], expected))
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the double out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.double_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::LongDoubleSeq::size_type i = 0; i < 10; ++i)
    {
      long double expected = in.longdouble_seq ()[i] + increase;
      if (!TestEqual (check.longdouble_seq ()[i], expected))
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the long double out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.longdouble_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::FloatSeq::size_type i = 0; i < 10; ++i)
    {
      float expected = in.float_seq ()[i] + increase;
      if (!TestEqual<float> (check.float_seq ()[i], expected))
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the float out sequence found : "
            << "expected <" << in.float_seq ()[i] + 1 << "> - found <"
            << check.float_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::OctetSeq::size_type i = 0; i < 10; ++i)
    {
      uint8_t expected = in.octet_seq ()[i] + increase;
      if (check.octet_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the octet out sequence found : "
            << "expected <" << expected << "> - found <"
            << check.octet_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::BooleanSeq::size_type i = 0; i < 10; ++i)
    {
      if (check.bool_seq ()[i] == in.bool_seq ()[i])
        {
          TAOX11_TEST_ERROR << "check_out_values - ERROR : unexpected value in "
            << "the bool out sequence found : "
            << "expected <" << in.bool_seq ()[i] << "> - found <"
            << check.bool_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
}

void
check_out_values (const Test::CorbaSequences &in,
  const Test::CorbaSequences& out, int &error_count)
{
  check_values (in, out, 1, error_count);
}

void
check_ret_values (const Test::CorbaSequences &in,
  const Test::CorbaSequences& ret,
  int &error_count)
{
  check_values (in, ret, 3, error_count);
}

void
check_inout_values (const Test::CorbaSequences& inout,
  int &error_count)
{
  for (wchar_t wc : inout.wchar_seq())
    {
      if (wc != L'A')
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the wchar inout sequence found : expected <A> - found <" << wc
            << ">" << std::endl;
          ++error_count;
        }
    }
  for (char c : inout.char_seq ())
    {
      if (c != 'B')
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the char inout sequence found : expected <B> - found <" << c
            << ">" << std::endl;
          ++error_count;
        }
    }
  for (std::wstring wstr : inout.wstring_seq ())
    {
      if (wstr != L"boo")
        {
          TAOX11_TEST_ERROR_W << "check_inout_values - ERROR : unexpected value in "
            << "the wstring inout sequence found : expected <boo> - found <"
            << wstr << ">" << std::endl;
          ++error_count;
        }
    }
  for (std::string str : inout.string_seq ())
    {
      if (str != "hi")
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the string inout sequence found : expected <hi> - found <"
            << str << ">" << std::endl;
          ++error_count;
        }
    }
  for (CORBA::ShortSeq::size_type i=0; i < inout.short_seq ().size (); ++i)
    {
      int16_t expected = 0;
      if ((i%2) == 0)
        {
          expected = 1;
        }
      if (inout.short_seq ()[i] != expected)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the short inout sequence found : expected <" << expected
            << "> - found <" << inout.short_seq ()[i] << ">" << std::endl;
          ++error_count;
        }
    }
  for (uint16_t ushrt : inout.ushort_seq ())
    {
      if (ushrt != 2)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the ushort inout sequence found : expected <2> - found <"
            << ushrt << ">" << std::endl;
          ++error_count;
        }
    }
  for (int32_t lng : inout.long_seq ())
    {
      if (lng != 3)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the long inout sequence found : expected <3> - found <"
            << lng << ">" << std::endl;
          ++error_count;
        }
    }
  for (uint32_t ulng : inout.ulong_seq ())
    {
      if (ulng != 4)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the ulong inout sequence found : expected <4> - found <"
            << ulng << ">" << std::endl;
          ++error_count;
        }
    }
  for (int64_t lnglng : inout.longlong_seq ())
    {
      if (lnglng != 5)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the longlong inout sequence found : expected <5> - found <"
            << lnglng << ">" << std::endl;
          ++error_count;
        }
    }
  for (uint64_t ulnglng : inout.ulonglong_seq ())
    {
      if (ulnglng != 6)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the ulonglong inout sequence found : expected <6> - found <"
            << ulnglng << ">" << std::endl;
          ++error_count;
        }
    }
  for (double dbl : inout.double_seq ())
    {
      double expected = 7;
      if (!TestEqual (dbl, expected))
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the double inout sequence found : expected <" << expected
            << "> - found <" << dbl << ">" << std::endl;
          ++error_count;
        }
    }
  for (long double ldbl : inout.longdouble_seq ())
    {
      long double expected = 8;
      if (!TestEqual (ldbl, expected))
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the long double inout sequence found : expected <" << expected
            << "> - found <" << ldbl << ">" << std::endl;
          ++error_count;
        }
    }
  for (float flt : inout.float_seq ())
    {
      float expected = 9.10f;
      if (!TestEqual (flt, expected))
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the float inout sequence found : expected <" << expected
            << "> - found <" << flt << ">" << std::endl;
          ++error_count;
        }
    }
  for (uint8_t ctt : inout.octet_seq ())
    {
      if (ctt != 11)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the octet inout sequence found : expected <11> - found <"
            << ctt << ">" << std::endl;
          ++error_count;
        }
    }
  for (bool bl : inout.bool_seq ())
    {
      if (bl)
        {
          TAOX11_TEST_ERROR << "check_inout_values - ERROR : unexpected value in "
            << "the bool inout sequence found : expected <false> - found <true>"
            << std::endl;
          ++error_count;
        }
    }
}

void
test_corba_sequences (IDL::traits<Test::Foo>::ref_type foo,
  int &error_count)
{
  TAOX11_TEST_DEBUG << std::endl << "Test CORBA defined sequences." << std::endl;
  TAOX11_TEST_DEBUG << "======================================" << std::endl;

  Test::CorbaSequences s_in;
  fill_sequences (s_in);

  Test::CorbaSequences s_inout = s_in;
  Test::CorbaSequences s_out;

  TAOX11_TEST_DEBUG << std::endl << "test_corba_sequences - calling "
    << "Foo::test_corba_sequences () with the following in- and "
    << "inout parameters : "
    << std::endl << "  wchar_seq      = " << IDL::traits<CORBA::WCharSeq>::write (s_in.wchar_seq ())
    << std::endl << "  char_seq       = " << s_in.char_seq ()
    << std::endl << "  wstring_seq    = " << s_in.wstring_seq ()
    << std::endl << "  string_seq     = " << s_in.string_seq ()
    << std::endl << "  short_seq      = " << s_in.short_seq ()
    << std::endl << "  ushort_seq     = " << s_in.ushort_seq ()
    << std::endl << "  long_seq       = " << s_in.long_seq ()
    << std::endl << "  ulong_seq      = " << s_in.ulong_seq ()
    << std::endl << "  longlong_seq   = " << s_in.longlong_seq ()
    << std::endl << "  ulonglong_seq  = " << s_in.ulonglong_seq ()
    << std::endl << "  double_seq     = " << s_in.double_seq ()
    << std::endl << "  longdouble_seq = " << s_in.longdouble_seq ()
    << std::endl << "  float_seq      = " << s_in.float_seq ()
    << std::endl << "  octet_seq      = " << IDL::traits<CORBA::OctetSeq>::write (s_in.octet_seq ())
    << std::endl << "  bool_seq       = " << s_in.bool_seq ()
    << std::endl;

  Test::CorbaSequences s_ret = foo->test_corba_sequences (s_in, s_inout, s_out);

  TAOX11_TEST_DEBUG << std::endl << "test_corba_sequences - returned from "
    << "Foo::test_corba_sequences () with the following return values : "
    << std::endl << "  wchar_seq      = " << s_ret.wchar_seq ()
    << std::endl << "  char_seq       = " << s_ret.char_seq ()
    << std::endl << "  wstring_seq    = " << s_ret.wstring_seq ()
    << std::endl << "  string_seq     = " << s_ret.string_seq ()
    << std::endl << "  short_seq      = " << s_ret.short_seq ()
    << std::endl << "  ushort_seq     = " << s_ret.ushort_seq ()
    << std::endl << "  long_seq       = " << s_ret.long_seq ()
    << std::endl << "  ulong_seq      = " << s_ret.ulong_seq ()
    << std::endl << "  longlong_seq   = " << s_ret.longlong_seq ()
    << std::endl << "  ulonglong_seq  = " << s_ret.ulonglong_seq ()
    << std::endl << "  double_seq     = " << s_ret.double_seq ()
    << std::endl << "  longdouble_seq = " << s_ret.longdouble_seq ()
    << std::endl << "  float_seq      = " << s_ret.float_seq ()
    << std::endl << "  octet_seq      = " << s_ret.octet_seq ()
    << std::endl << "  bool_seq       = " << s_ret.bool_seq ()
    << std::endl;
  TAOX11_TEST_DEBUG << std::endl << "test_corba_sequences - returned from "
    << "Foo::test_corba_sequences () with the following out values : "
    << std::endl << "  wchar_seq      = " << s_out.wchar_seq ()
    << std::endl << "  char_seq       = " << s_out.char_seq ()
    << std::endl << "  wstring_seq    = " << s_out.wstring_seq ()
    << std::endl << "  string_seq     = " << s_out.string_seq ()
    << std::endl << "  short_seq      = " << s_out.short_seq ()
    << std::endl << "  ushort_seq     = " << s_out.ushort_seq ()
    << std::endl << "  long_seq       = " << s_out.long_seq ()
    << std::endl << "  ulong_seq      = " << s_out.ulong_seq ()
    << std::endl << "  longlong_seq   = " << s_out.longlong_seq ()
    << std::endl << "  ulonglong_seq  = " << s_out.ulonglong_seq ()
    << std::endl << "  double_seq     = " << s_out.double_seq ()
    << std::endl << "  longdouble_seq = " << s_out.longdouble_seq ()
    << std::endl << "  float_seq      = " << s_out.float_seq ()
    << std::endl << "  octet_seq      = " << s_out.octet_seq ()
    << std::endl << "  bool_seq       = " << s_out.bool_seq ()
    << std::endl;

  TAOX11_TEST_DEBUG << std::endl << "test_corba_sequences - returned from "
    << "Foo::test_corba_sequences () with the following inout values : "
    << std::endl << "  wchar_seq      = " << s_inout.wchar_seq ()
    << std::endl << "  char_seq       = " << s_inout.char_seq ()
    << std::endl << "  wstring_seq    = " << s_inout.wstring_seq ()
    << std::endl << "  string_seq     = " << s_inout.string_seq ()
    << std::endl << "  short_seq      = " << s_inout.short_seq ()
    << std::endl << "  ushort_seq     = " << s_inout.ushort_seq ()
    << std::endl << "  long_seq       = " << s_inout.long_seq ()
    << std::endl << "  ulong_seq      = " << s_inout.ulong_seq ()
    << std::endl << "  longlong_seq   = " << s_inout.longlong_seq ()
    << std::endl << "  ulonglong_seq  = " << s_inout.ulonglong_seq ()
    << std::endl << "  double_seq     = " << s_inout.double_seq ()
    << std::endl << "  longdouble_seq = " << s_inout.longdouble_seq ()
    << std::endl << "  float_seq      = " << s_inout.float_seq ()
    << std::endl << "  octet_seq      = " << s_inout.octet_seq ()
    << std::endl << "  bool_seq       = " << s_inout.bool_seq ()
    << std::endl;

  check_length_inout (s_inout, error_count);

  check_out_values (s_in, s_out, error_count);
  check_inout_values (s_inout, error_count);
  check_ret_values (s_in, s_ret, error_count);
}
