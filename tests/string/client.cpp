/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

int
main(int argc, char* argv[])
{
  int result = 0;
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);

    if (orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
        << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj =
        orb->string_to_object("file://test.ior");

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
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
        << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    {
      TAOX11_TEST_DEBUG << "Test string type." << std::endl;
      std::string getText = hello->get_string ();
      if (getText != "Hello there!")
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_string() returned an unexpected value. "
          << "expected <Hello there!>, received <" << getText << ">" << std::endl;
        ++result;
      }
      hello->set_string ("Hello there!");
    }

    {
      TAOX11_TEST_DEBUG << "Test string type out parameter." << std::endl;
      std::string outText;
      hello->out_string (outText);
      if (outText != "I hear you!")
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_string() returned an unexpected value. "
          << "expected <I hear you!>, received <" << outText << ">" << std::endl;
        ++result;
      }
    }

    {
      TAOX11_TEST_DEBUG << "Test string type inout parameter." << std::endl;
      std::string inoutText = "Hello there!";
      hello->inout_string (inoutText);
      if (inoutText != "I hear you!")
      {
        TAOX11_TEST_ERROR << "ERROR: hello->inout_string() received unexpected value. "
          << "expected <I hear you!>, received <" << inoutText << ">" << std::endl;
        ++result;
      }
    }

    {
      std::string longText;
      longText.assign(66000, 'a');
      TAOX11_TEST_DEBUG << "Test string with large content." << std::endl;
      const std::string getText = hello->get_lstring ();
      if (getText != longText)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_lstring() returned an unexpected value "
          << "expected 66000 times 'a'";
        ++result;
      }
      hello->set_lstring (longText);
    }

    {
      std::string outText = "";
      std::string expected;
      expected.assign(66000, 'b');
      TAOX11_TEST_DEBUG << "Test out string with large content." << std::endl;
      hello->out_lstring (outText);
      if (outText != expected)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_lstring() returned an unexpected value. "
          << "expected 66000 times 'b'" << std::endl;
        ++result;
      }
    }

    {
      std::string longText;
      longText.assign(66000, 'c');

      std::string expected;
      expected.assign(66000, 'd');

      TAOX11_TEST_DEBUG << "Test inout string with large content." << std::endl;

      hello->inout_lstring (longText);
      if (longText != expected)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->inout_lstring() returned an unexpected value. "
          << "expected 66000 times 'd'" << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG << "ostream longText string :" << longText << std::endl;
    }

    // string-sequence
    {
      TAOX11_TEST_DEBUG << "Test string-sequence type get and set." << std::endl;
      const CORBA::StringSeq seq = hello->get_stringSeq ();
      if (!eqv (seq, stringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_stringSeq() returned an unexpected value."
          << std::endl;
        ++result;
      }
      hello->set_stringSeq (stringVec);

      TAOX11_TEST_DEBUG << "Test string-sequence type out parameter." << std::endl;
      CORBA::StringSeq outSeq;
      hello->out_stringSeq (outSeq);
      if (!eqv (outSeq, stringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_stringSeq() returned an unexpected value."
          << std::endl;
        ++result;
      }

      TAOX11_TEST_DEBUG << "Test string-sequence type inout parameter." << std::endl;
      outSeq = stringVec;
      hello->inout_stringSeq (outSeq);
      if (!eqv (outSeq, stringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: Hello::inout_stringSeq (out) returned an unexpected "
          << "value." << std::endl;
        ++result;
      }
    }
    TAOX11_TEST_DEBUG << std::endl;
    {
      // Test if bar is bounded and bounded correctly
      if (IDL::traits<Test::bounded_bar>::is_bounded ())
      {
        TAOX11_TEST_DEBUG << "main - OK, is_bounded trait for <bounded_bar> set correctly"
          << std::endl;
        if (IDL::traits<Test::bounded_bar >::bound () == 5)
        {
          TAOX11_TEST_DEBUG << "main - OK, Correct bound for "
            << "<bounded_bar> found." << std::endl;
        }
        else
        {
          TAOX11_TEST_ERROR << "main - ERROR, incorrect bound "
            << "found for <bounded_bar>. expected <5> - found <"
            << IDL::traits<Test::bounded_bar >::bound () << ">."
            << std::endl;
        }
      }
      else
      {
        TAOX11_TEST_ERROR << "main - ERROR bad is_bounded trait for bounded_bar found."
          << std::endl;
      }

      TAOX11_TEST_DEBUG << std::endl;

      try
      {
        TAOX11_TEST_DEBUG << "main - invoking bounded_string with \"12345\"" << std::endl;
        hello->bounded_string ("12345");
      }
      catch (const CORBA::Exception &e)
      {
        TAOX11_TEST_ERROR << "main - ERROR - Caught unexpected CORBA exception while "
          << "invoking 'bounded_string': " << e << std::endl;
      }

      try
      {
        TAOX11_TEST_DEBUG << "main - invoking bounded_string with \"123456\"" << std::endl;
        hello->bounded_string ("123456");
        TAOX11_TEST_ERROR << "main - ERROR - No MARSHAL exception caught" << std::endl;
      }
      catch (const CORBA::BAD_PARAM &e)
      {
        TAOX11_TEST_DEBUG << "main - Caught CORBA::BAD_PARAM exception as expected. " << e
          << std::endl;
      }
    }

    TAOX11_TEST_DEBUG << std::endl << "shutting down...";

    hello->shutdown();

      orb->destroy ();

    TAOX11_TEST_DEBUG << std::endl;
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    ++result;
  }
  return result;
}
