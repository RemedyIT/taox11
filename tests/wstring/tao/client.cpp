/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++ client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

// FUZZ: disable check_for_ace_log_categories

#include "testC.h"
#include <iostream>
#include "testdata.h"
#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT ("file://test.ior");

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'k':
        ior = get_opts.opt_arg();
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                "usage:  %s "
                "-k <ior> "
                "\n",
                argv [0]),
            false);
    }
  // Indicates successful parsing of the command line
  return true;
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result_ = 0;
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args(argc, argv) == false)
      return 1;

    CORBA::Object_var tmp = orb->string_to_object(ior);

    Test::Hello_var hello = Test::Hello::_narrow (tmp.in());

    if (CORBA::is_nil(hello.in()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
              "Nil Test::Hello reference <%s>\n",
              ior),
          1);
    }
    std::cout << "narrowed Hello interface" << std::endl;

    // Test

    {
      std::cout << "Test wstring get and set." << std::endl;
      CORBA::WString_var getText = hello->getset_string(L"Hello there!");
      if (ACE_OS::strcmp(getText.in(), L"Hello there!") != 0)
      {
        std::wcout
            << L"ERROR: hello->getset_string() expected 'Hello there!', received "
            << getText.in() << std::endl;
        ++result_;
      }

      std::cout << "Test wstring type out parameter." << std::endl;
      CORBA::WString_var outText;
      hello->out_string(outText.out());
      if (ACE_OS::strcmp(outText.in(), L"I hear you!") != 0)
      {
        std::wcout
            << L"ERROR: hello->out_string() return value expected 'I hear you!', received "
            << outText.in() << std::endl;
        ++result_;
      }

      std::cout << "Test string type inout parameter." << std::endl;
      CORBA::WString_var inoutText = L"Hello there!";
      hello->inout_string(inoutText.inout());
      if (ACE_OS::strcmp(inoutText.in(), L"I hear you!") != 0)
      {
        std::wcout
            << L"ERROR: hello->inout_string parameter value expected 'I hear you!', received "
            << inoutText.in() << std::endl;
        ++result_;
      }

      // Long text string

      std::wstring longText;
      longText.assign(66000, L'a');
      CORBA::WString_var cwtext = CORBA::wstring_dup(longText.c_str());
      std::cout << "Test wstring with large content." << std::endl;
      CORBA::WString_var text = hello->getset_lstring(cwtext);
      if (ACE_OS::strcmp(text.in(), cwtext))
      {
        std::cout
            << "ERROR: hello->get_lstring parameter value expected 66000 times 'a', received different."
            << std::endl;
        ++result_;
      }
      text = L"";
      longText.assign(66000, L'b');
      cwtext = CORBA::wstring_dup(longText.c_str());
      std::cout << "Test out wstring with large content." << std::endl;
      hello->out_lstring(outText.out());
      if (ACE_OS::strcmp(cwtext.in(), outText.in()))
      {
        std::cout
            << "ERROR: hello->out_lstring() return value expected 66000 times 'b', received different."
            << std::endl;
        ++result_;
      }
      longText.assign(66000, L'c');
      cwtext = CORBA::wstring_dup(longText.c_str());

      std::wstring longText2;
      longText2.assign(66000, 'd');
      outText = CORBA::wstring_dup(longText2.c_str());
      std::cout << "Test inout lstring with large content." << std::endl;
      hello->inout_lstring(cwtext.inout());
      if (ACE_OS::strcmp(outText.in(), cwtext.in()))
      {
        std::cout
            << "ERROR: hello->inout_lstring parameter value expected 66000 times 'd', received different."
            << std::endl;
        ++result_;
      }

      // string-sequence

      std::cout << "Test wstring-sequence type get and set." << std::endl;
      CORBA::WStringSeq_var stringSeq = Array2Seq(stringArr);
      CORBA::WStringSeq_var stringOutSeq;
      stringOutSeq = Array2Seq(stringOutArr);
      CORBA::WStringSeq_var seq = hello->getset_stringSeq(stringSeq.in());
      if (!(eqv(seq, stringOutSeq)))
      {
        std::cout
            << "ERROR: hello->getset_stringSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      std::cout << "Test wstring-sequence type out parameter." << std::endl;
      hello->out_stringSeq(stringSeq.out());
      if (!(eqv(stringSeq, stringOutSeq)))
      {
        std::cout << "ERROR: hello->out_stringSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      std::cout << "Test wstring-sequence type inout parameter." << std::endl;
      stringSeq = Array2Seq(stringArr);
      hello->inout_stringSeq(stringSeq.inout());
      if (!(eqv(stringSeq, stringOutSeq)))
      {
        std::cout
            << "ERROR: hello->inout_stringSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    std::cout << "shutting down...";
    hello->shutdown();
    std::cout << std::endl;
    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception("Exception caught:");
    return 1;
  }

  return result_;
}
