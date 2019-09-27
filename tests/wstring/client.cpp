/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"
#include "testdata.h"

int main(int argc, char* argv[])
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

    IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object("file://test.ior");

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

    // Test
    {
      TAOX11_TEST_DEBUG << "Test wstring get and set." << std::endl;
      const std::wstring text = hello->getset_string (L"Hello there!");
      if (text != L"Hello there!")
      {
        TAOX11_TEST_ERROR_W << L"ERROR: hello->get_string() returned an unexpected value. "
          << L"expected <Hello there!>, received <" << text << L">" << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG_W << L"ostream test wstring: " << text << std::endl;
    }

    {
      TAOX11_TEST_DEBUG << "Test wstring type out parameter." << std::endl;
      std::wstring text (L"");
      hello->out_string (text);
      if (text != L"I hear you!")
      {
        TAOX11_TEST_ERROR_W << L"ERROR: hello->out_string() returned an unexpected value. "
          << L"expected <I hear you!>, received <" << text << L">" << std::endl;
        ++result;
      }
    }

    {
      TAOX11_TEST_DEBUG << "Test wstring type inout parameter." << std::endl;
      std::wstring text (L"Hello there!");
      hello->inout_string (text);
      if (text != L"I hear you!")
      {
        TAOX11_TEST_ERROR_W << L"ERROR: hello->inout_string returned an unexpected value. "
          << L"expected <I hear you!>, received <" << text << L">" << std::endl;
        ++result;
      }
    }

    {
      // Long text string
      std::wstring longText;
      longText.assign(66000, L'a');
      TAOX11_TEST_DEBUG << "Test wstring with large content." << std::endl;
      const std::wstring text = hello->getset_lstring (longText);
      if (text != longText)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->get_lstring returned an unexpected value. "
          << "expected 66000 times 'a'." << std::endl;
        ++result;
      }
    }

    {
      std::wstring text = L"";
      std::wstring longText;
      longText.assign(66000, L'b');
      TAOX11_TEST_DEBUG << "Test out wstring with large content." << std::endl;
      hello->out_lstring (text);
      if (text != longText)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_lstring() returned an unexpected value. "
          << "expected 66000 times 'b'" << std::endl;
        ++result;
      }
    }

    {
      std::wstring longText;
      longText.assign(66000, L'c');

      std::wstring expected;
      expected.assign(66000, L'd');

      TAOX11_TEST_DEBUG << "Test inout lstring with large content." << std::endl;
      hello->inout_lstring (longText);
      if (longText != expected)
      {
        TAOX11_TEST_ERROR << "ERROR: hello->inout_lstring returned an unexpected value. "
          << "expected 66000 times 'd'." << std::endl;
        ++result;
      }
      TAOX11_TEST_DEBUG_W << L"ostream longText wstring: " << longText << std::endl;
    }

    // string-sequence
    {
      TAOX11_TEST_DEBUG << "Test wstring-sequence type get and set." << std::endl;
      CORBA::WStringSeq outSeq;
      outSeq = stringVec;
      const CORBA::WStringSeq seq = hello->getset_stringSeq (outSeq);
      if (!eqv (seq, stringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->getset_stringSeq() returned an unexpected value."
          << std::endl;
        ++result;
      }

      TAOX11_TEST_DEBUG << "Test wstring-sequence type out parameter." << std::endl;
      hello->out_stringSeq (outSeq);
      if (!eqv (outSeq, stringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->out_stringSeq() returned an unexpected value."
          << std::endl;
        ++result;
      }

      TAOX11_TEST_DEBUG << "Test wstring-sequence type inout parameter." << std::endl;
      outSeq = stringVec;
      hello->inout_stringSeq (outSeq);
      if (!eqv (outSeq, stringOutVec))
      {
        TAOX11_TEST_ERROR << "ERROR: hello->inout_stringSeq (out) received an unexpected value."
          << std::endl;
        ++result;
      }
    }

    {
      // Test if bar is bounded and bounded correctly
      if (IDL::traits< Test::bounded_bar >::is_bounded ())
      {
        TAOX11_TEST_DEBUG << "main - OK, is_bounded trait for <bounded_bar> set correctly"
          << std::endl;
        if (IDL::traits< Test::bounded_bar >::bound () == 5)
        {
          TAOX11_TEST_DEBUG << "main - OK, Correct bound for "
            << "<bounded_bar> found." << std::endl;
        }
        else
        {
          TAOX11_TEST_ERROR << "main - ERROR, incorrect bound "
            << "found for <bounded_bar>. expected <5> - found <"
            << IDL::traits< Test::bounded_bar >::bound () << ">."
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
        hello->bounded_string (L"12345");
      }
      catch (const CORBA::Exception &e)
      {
        TAOX11_TEST_ERROR << "main - ERROR - Caught unexpected CORBA exception while "
          << "invoking 'bounded_string': " << e << std::endl;
      }

      try
      {
        TAOX11_TEST_DEBUG << "main - invoking bounded_string with \"123456\"" << std::endl;
        hello->bounded_string (L"123456");
        TAOX11_TEST_ERROR << "main - ERROR - No MARSHAL exception caught" << std::endl;
      }
      catch (const CORBA::BAD_PARAM &e)
      {
        TAOX11_TEST_DEBUG << "main - Caught CORBA::BAD_PARAM exception as expected. " << e
          << std::endl;
      }
    }

    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown();
    TAOX11_TEST_DEBUG << std::endl;

    orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
    ++result;
  }

  return result;
}
