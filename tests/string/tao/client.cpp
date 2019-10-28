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

    // Test

    std::cout << "Test string type get method." << std::endl;
    CORBA::String_var getText = hello->get_string();
    if (strcmp(getText.in(), "Hello there!") != 0)
    {
      std::cout
          << "ERROR: hello->get_string() expected 'Hello there!', received "
          << getText;
      ++result_;
    }

    //    ACE_DEBUG ((LM_DEBUG, "(%P|%t) - string returned <%C>\n",
    //            getText.in ()));

    hello->set_string("Hello there!");
    std::cout << "Test string type out parameter." << std::endl;
    CORBA::String_var outText;
    hello->out_string(outText.out());
    if (strcmp(outText.in(), "I hear you!") != 0)
    {
      std::cout
          << "ERROR: hello->out_string() return value expected 'I hear you!', received "
          << outText;
      ++result_;
    }

    std::cout << "Test string type inout parameter." << std::endl;
    CORBA::String_var inoutText = "Hello there!";
    hello->inout_string(inoutText);
    if (strcmp(inoutText.in(), "I hear you!") != 0)
    {
      std::cout
          << "ERROR: hello->inout_string parameter value expected 'I hear you!', received "
          << std::endl;
      ++result_;
    }

    std::string longText;
    longText.assign(66000, 'a');
    std::cout << "Test get string with large content." << std::endl;
    getText = hello->get_lstring();
    if (strcmp(getText.in(), longText.c_str()) != 0)
    {
      std::cout
          << "ERROR: Hello::get_lstring parameter value expected 66000 times 'a', received different.";
      ++result_;
    }

    CORBA::String_var longTaoText = CORBA::string_dup(longText.c_str());
    hello->set_lstring(longTaoText);
    outText = "";
    longText.assign(66000, 'b');
    std::cout << "Test out string with large content." << std::endl;
    hello->out_lstring(outText);
    if (strcmp(outText.in(), longText.c_str()) != 0)
    {
      std::cout
          << "ERROR: hello->out_lstring() return value expected 66000 times 'b', received, received different.";
      ++result_;
    }
    longText.assign(66000, 'c');
    std::string resultLongText;
    resultLongText.assign(66000, 'd');
    std::cout << "Test inout string with large content." << std::endl;
    longTaoText = CORBA::string_dup(longText.c_str());
    hello->inout_lstring(longTaoText);
    if (strcmp(longTaoText.in(), resultLongText.c_str()) != 0)
    {
      std::cout
          << "ERROR: Hello::inout_lstring parameter value expected 66000 times 'd', received different.";
      ++result_;
    }

    // string-sequence
    {
      std::cout << "Test string-sequence type get." << std::endl;
      CORBA::StringSeq_var seq = hello->get_stringSeq();
      CORBA::StringSeq_var stringOutSeq = Array2Seq(stringOutArr);
       if (!(eqv(seq, stringOutSeq)))
      {
        std::cout << "ERROR: hello->get_stringSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      std::cout << "Test string-sequence type out parameter." << std::endl;
      CORBA::StringSeq_var outSeq;
      hello->out_stringSeq(outSeq);
      if (!(eqv(outSeq, stringOutSeq)))
      {
        std::cout << "ERROR: hello->out_stringSeq() received unexpected value."
            << std::endl;
        ++result_;
      }
      std::cout << "Test string-sequence type inout parameter." << std::endl;
      CORBA::StringSeq_var stringSeq = Array2Seq(stringArr);
      outSeq = stringSeq;
      hello->inout_stringSeq(outSeq);
      if (!(eqv(outSeq, stringOutSeq)))
      {
        std::cout
            << "ERROR: Hello::inout_stringSeq (out) received unexpected value."
            << std::endl;
        ++result_;
      }
    }

    std::cout << "shutting down...";
    hello->shutdown();

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    ex._tao_print_exception("Exception caught:");
    return 1;
  }

  return result_;
}
