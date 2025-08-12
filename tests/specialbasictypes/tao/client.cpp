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
#include <limits>
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

int result_ = 0;

template<class T>
void TestEqual(T exp, T a)
{
  if (a != exp)
  {
    std::cout << "ERROR: Received unexpected value expected " << exp << " , received " << a
        << std::endl;
    ++result_;
  }
}

template<>
void TestEqual(wchar_t exp, wchar_t a)
{
  if (a != exp)
  {
    std::cout << "ERROR: Received unexpected wchar_t value" << std::endl;
    ++result_;
  }
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    CORBA::ORB_var orb = CORBA::ORB_init(argc, argv);

    if (parse_args(argc, argv) == false)
      return 1;

    CORBA::Object_var tmp = orb->string_to_object(ior);

    Test::Hello_var hello = Test::Hello::_narrow (tmp.in());
    if (CORBA::is_nil(hello.in()))
    {
      ACE_ERROR_RETURN ((LM_DEBUG, "Nil Test::Hello reference <%s>\n", ior), 1);
    }

    std::cout << "narrowed Hello interface" << std::endl;

    // Test

    // Char ~ CORBA::Char
    {
      CORBA::Char charmin = std::numeric_limits<CORBA::Char>::min();
      CORBA::Char charmax = std::numeric_limits<CORBA::Char>::max();
      std::cout << "Test Char type get and set." << std::endl;
      CORBA::Char num = hello->getset_char(charmin, charmax);
      TestEqual(num, charmin);
      std::cout << "Test Char type out parameter." << std::endl;
      CORBA::Char cinout;
      hello->out_char(charmin, cinout);
      TestEqual(cinout, charmin);
      hello->out_char(charmax, cinout);
      TestEqual(cinout, charmax);
      std::cout << "Test Char type inout parameter." << std::endl;
      num =charmin;
      cinout = charmax;
      hello->inout_char(num, cinout);
      TestEqual(cinout, charmin);
      TestEqual(num, charmax);
    }

    // WChar ~ CORBA::WChar
    {
      std::cout << "Test WChar type get and set." << std::endl;
      CORBA::WChar num = hello->getset_wchar(L'0', L'z');
      TestEqual(num, L'0');
      // NOTE: No result test using <CORBA::WChar>::min and <CORBA::WChar>::max because wchar in Corba is different from wchar in std-library.
      std::cout << "Test WChar type out parameter." << std::endl;
      CORBA::WChar cinout;
      hello->out_wchar(L'A', cinout);
      TestEqual(cinout, L'A');
      hello->out_wchar(L'z', cinout);
      TestEqual(cinout, L'z');
      CORBA::WChar wchar_tmin = std::numeric_limits<CORBA::WChar>::min();
      CORBA::WChar wchar_tmax = std::numeric_limits<CORBA::WChar>::max();
      hello->out_wchar(wchar_tmin, cinout);
      hello->out_wchar(wchar_tmax, cinout);
      // NOTE: No result test using <CORBA::WChar>::min and <CORBA::WChar>::max because wchar in Corba is different from wchar in std-library.
      std::cout << "Test WChar type inout parameter." << std::endl;
      cinout = wchar_tmax;
      hello->inout_wchar(L'A', cinout);
      TestEqual(cinout, L'A');
      hello->inout_wchar(L'z', cinout);
      TestEqual(cinout, L'z');
    }

    // Octet ~ CORBA::Octet
    {
      CORBA::Octet uint8_tmin = std::numeric_limits<CORBA::Octet>::min();
      CORBA::Octet uint8_tmax = std::numeric_limits<CORBA::Octet>::max();
      std::cout << "Test Octet type get and set." << std::endl;
      CORBA::Octet num = hello->getset_octet(uint8_tmin, uint8_tmax);
      TestEqual(num, uint8_tmin);
      std::cout << "Test Octet type out parameter." << std::endl;
      CORBA::Octet outNumber;
      hello->out_octet(uint8_tmin, outNumber);
      TestEqual(outNumber, uint8_tmin);
      hello->out_octet(uint8_tmax, outNumber);
      TestEqual(outNumber, uint8_tmax);
      std::cout << "Test Octet type inout parameter." << std::endl;
      outNumber = uint8_tmax;
      hello->inout_octet(outNumber);
      TestEqual(outNumber, uint8_tmin);
      outNumber = uint8_tmin;
      hello->inout_octet(outNumber);
      TestEqual(outNumber, uint8_tmax);
    }

    // Boolean ~ CORBA::Boolean
    {
      std::cout << "Test Boolean type get and set." << std::endl;
      CORBA::Boolean boolmin = std::numeric_limits<CORBA::Boolean>::min();
      CORBA::Boolean boolmax = std::numeric_limits<CORBA::Boolean>::max();
      CORBA::Boolean num = hello->getset_boolean(boolmin, boolmax);
      TestEqual(num, boolmin);
      std::cout << "Test Boolean type out parameter." << std::endl;
      hello->out_boolean(false, num);
      TestEqual(num, true);
      hello->out_boolean(true, num);
      TestEqual(num, false);
      std::cout << "Test Boolean type inout parameter." << std::endl;
      num = true;
      hello->inout_boolean(num);
      TestEqual(num, false);
      num = false;
      hello->inout_boolean(num);
      TestEqual(num, true);
    }

    // End

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
