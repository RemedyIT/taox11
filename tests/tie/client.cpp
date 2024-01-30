/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

// GCC 4.x and 6 have a bug which prevents us to use the tie
// specialization without specifying the namespace explicitly
#if defined __GNUC__ && (__GNUC__ < 7)
#define TAOX11_LACKS_TIE_SPECIALIZATION
#endif

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
        TAOX11_TEST_ERROR << "usage: -k <ior>"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
        {
          TAOX11_TEST_ERROR
            << "ERROR: string_to_object(<ior>) returned null reference."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (obj);

      if (!hello)
        {
          TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      TAOX11_TEST_INFO << "narrowed Hello interface" << std::endl;

      std::string const hello_string = hello->get_string ();
      TAOX11_TEST_INFO << "hello->get_string () returned <" << hello_string
        << ">" << std::endl;

#if !defined (TAOX11_LACKS_TIE_SPECIALIZATION)
      if (hello_string != Test::foo)
      {
        TAOX11_TEST_ERROR << "ERROR: get_string returned <" << hello_string
                          << "> but should have returned <" << Test::foo << ">"
                          << std::endl;
      }
#else
      if (hello_string != Test::regular_foo)
      {
        TAOX11_TEST_ERROR << "ERROR: get_string returned <" << hello_string
                          << "> but should have returned <" << Test::regular_foo << ">"
                          << std::endl;
      }
#endif /* !TAOX11_LACKS_TIE_SPECIALIZATION */

      char const at_char_ = hello->at_char();

      if (at_char_ != 'b')
      {
        TAOX11_TEST_ERROR << "ERROR: at_char returned <" << at_char_ << "> but should "
                          << "have returned <b>"
                          << std::endl;
      }

      TAOX11_TEST_INFO << "shutting down...";

      hello->shutdown ();

      TAOX11_TEST_INFO << std::endl;

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }
  return 0;
}
