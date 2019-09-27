/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client CORBA sequences Any test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
// Test of ANY insertion/extraction for all standard CORBA sequences

#include "testC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"
#include <cmath>

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");


bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:  -k <ior>"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

template <typename _Seq>
bool run_test (IDL::traits<Test::Hello>::ref_type tester, const _Seq& inseq)
{
  TAOX11_TEST_DEBUG << "runtest start - inseq = " << inseq << std::endl;

  _Seq check_;
  CORBA::Any input;
  input <<= inseq;

  input >>= check_;
  if (check_ != inseq)
  {
    TAOX11_TEST_ERROR << "ERROR: failed to verify input sequence" << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG << "calling Test::Hello::test()" << std::endl;

  CORBA::Any output = tester->test (input);

  check_.clear ();
  output >>= check_;
  if (check_ != inseq)
  {
    TAOX11_TEST_ERROR << "ERROR: failed to verify output sequence" << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG << "runtest end" << std::endl;
  return true;
}

template <>
bool run_test<CORBA::WCharSeq> (IDL::traits<Test::Hello>::ref_type tester, const CORBA::WCharSeq& inseq)
{
  TAOX11_TEST_DEBUG << "runtest start - inseq = " << inseq << std::endl;

  CORBA::WCharSeq check_;
  CORBA::Any input;
  input <<= inseq;

  input >>= check_;
  if (check_ != inseq)
  {
    TAOX11_TEST_ERROR << "ERROR: failed to verify input sequence" << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG << "calling Test::Hello::test_wcharseq()" << std::endl;

  // MCO@20130319 - temporary till TAO WharSeq problem fixed (see also below)
  // the regulat test() simply returns an unmarshaled Any which results in
  // a MARSHAL exception at marshaling time which is more difficult to debug
  // test_wcharseq () attempts to demarshal the Any before returning in another Any
  // CORBA::Any output = tester->test (input);
  CORBA::Any output = tester->test_wcharseq (input);

  check_.clear ();
  output >>= check_;
  if (check_ != inseq)
  {
    TAOX11_TEST_ERROR << "ERROR: failed to verify output sequence" << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG << "runtest end" << std::endl;
  return true;
}

// perform special compare here to cover for the possible conversion losses
// where the native long double format (< 16 bytes) does not match the CORBA
// CDR format (16 bytes)
bool is_equal(const CORBA::LongDoubleSeq& _seq1, const CORBA::LongDoubleSeq& _seq2)
{
  if (_seq1.size () == _seq2.size ())
  {
    CORBA::LongDoubleSeq::const_iterator _i1;
    CORBA::LongDoubleSeq::const_iterator _i2;
    for (_i1 = _seq1.begin (), _i2 = _seq2.begin () ;
         _seq1.cend () != _i1 ;
         ++_i1, ++_i2)
    {
      if (std::fabs (*_i1 - *_i2) > std::numeric_limits<long double>::epsilon ())
      {
        return false;
      }
    }
    return true;
  }
  return false;
}

template <>
bool run_test<CORBA::LongDoubleSeq> (IDL::traits<Test::Hello>::ref_type tester, const CORBA::LongDoubleSeq& inseq)
{
  TAOX11_TEST_DEBUG << "runtest - inseq = " << inseq << std::endl;

  CORBA::LongDoubleSeq check_;
  CORBA::Any input;
  input <<= inseq;

  input >>= check_;
  if (!is_equal (check_, inseq))
  {
    TAOX11_TEST_ERROR << "ERROR: failed to verify input sequence" << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG << "calling Test::Hello::test ()" << std::endl;

  CORBA::Any output = tester->test (input);

  check_.clear ();
  output >>= check_;
  if (!is_equal (check_, inseq))
  {
    TAOX11_TEST_ERROR << "ERROR: failed to verify output sequence" << std::endl;
    return false;
  }

  TAOX11_TEST_DEBUG << "runtest end" << std::endl;
  return true;
}

int
main(int argc, char* argv[])
{
  bool testSucceeded = true;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
        {
          TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
                    << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference."
                    << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

      if (!hello)
        {
          TAOX11_TEST_ERROR << "ERROR: Test::Hello::narrow (obj) returned null object."
                    << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

      // testing

      CORBA::OctetSeq octets;

      // empty sequence
      testSucceeded &= run_test (hello, octets);

      // filled sequence
      octets = {11,12,13,14,15,16,17,18,19,20};
      testSucceeded &= run_test (hello, octets);

      CORBA::CharSeq chars;

      // empty sequence
      testSucceeded &= run_test (hello, chars);

      // filled sequence
      chars = {'a','b','c','d','e'};
      testSucceeded &= run_test (hello, chars);

      CORBA::WCharSeq wchars;

      // empty sequence
      testSucceeded &= run_test (hello, wchars);

// MCO@2013-03-19 - WCharSeq demarshaling in TAO currently has a flaw causing (de)marshaling problems
//      // filled sequence
//      wchars = {L'a',L'b',L'c',L'd',L'e'};
//      testSucceeded &= run_test (hello, wchars);

      CORBA::ShortSeq shorts;

      // empty sequence
      testSucceeded &= run_test (hello, shorts);

      // filled sequence
      shorts = {-11,-12,-13,-14,-15,16,17,18,19,20};
      testSucceeded &= run_test (hello, shorts);

      CORBA::UShortSeq ushorts;

      // empty sequence
      testSucceeded &= run_test (hello, ushorts);

      // filled sequence
      ushorts = {11,12,13,14,15,16,17,18,19,20};
      testSucceeded &= run_test (hello, ushorts);

      CORBA::LongSeq longs;

      // empty sequence
      testSucceeded &= run_test (hello, longs);

      // filled sequence
      longs = {-11,-12,-13,-14,-15,16,17,18,19,20};
      testSucceeded &= run_test (hello, longs);

      CORBA::ULongSeq ulongs;

      // empty sequence
      testSucceeded &= run_test (hello, ulongs);

      // filled sequence
      ulongs = {11,12,13,14,15,16,17,18,19,20};
      testSucceeded &= run_test (hello, ulongs);

      CORBA::LongLongSeq llongs;

      // empty sequence
      testSucceeded &= run_test (hello, llongs);

      // filled sequence
      llongs = {-11,-12,-13,-14,-15,16,17,18,19,20};
      testSucceeded &= run_test (hello, llongs);

      CORBA::ULongLongSeq ullongs;

      // empty sequence
      testSucceeded &= run_test (hello, ullongs);

      // filled sequence
      ullongs = {11,12,13,14,15,16,17,18,19,20};
      testSucceeded &= run_test (hello, ullongs);

      CORBA::FloatSeq floats;

      // empty sequence
      testSucceeded &= run_test (hello, floats);

      // filled sequence
      floats = {-11.1F,-12.1F,-13.1F,-14.1F,-15.1F,16.1F,17.1F,18.1F,19.1F,20.1F};
      testSucceeded &= run_test (hello, floats);

      CORBA::DoubleSeq doubles;

      // empty sequence
      testSucceeded &= run_test (hello, doubles);

      // filled sequence
      doubles = {-11.1,-12.1,-13.1,-14.1,-15.1,16.1,17.1,18.1,19.1,20.1};
      testSucceeded &= run_test (hello, doubles);

      CORBA::LongDoubleSeq ldoubles;

      // empty sequence
      testSucceeded &= run_test (hello, ldoubles);

      // filled sequence
      ldoubles = {-11.1,-12.1,-13.1,-14.1,-15.1,16.1,17.1,18.1,19.1,20.1};
      testSucceeded &= run_test (hello, ldoubles);

      CORBA::StringSeq strings;

      // empty sequence
      testSucceeded &= run_test (hello, strings);

      // filled sequence
      strings = {"abc", "def", "hij", "klm", "nop"};
      testSucceeded &= run_test (hello, strings);

      CORBA::WStringSeq wstrings;

      // empty sequence
      testSucceeded &= run_test (hello, wstrings);

      // filled sequence
      wstrings = {L"abc", L"def", L"hij", L"klm", L"nop"};
      testSucceeded &= run_test (hello, wstrings);

      CORBA::BooleanSeq booleans;

      // empty sequence
      testSucceeded &= run_test (hello, booleans);

      // filled sequence
      booleans = {true, true, true, true, true};
      testSucceeded &= run_test (hello, booleans);

      // end testing

      TAOX11_TEST_DEBUG << "shutting down...";

      hello->shutdown ();

      TAOX11_TEST_DEBUG << std::endl;

      _orb->destroy ();

      if (testSucceeded)
      {
        TAOX11_TEST_DEBUG << " Any CORBA Sequences Test OK" << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << " ERROR : Any CORBA Sequences test failed" << std::endl;
      }
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      testSucceeded = false;
    }

  return testSucceeded ? 0 : 1;
}

