/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testC.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

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

int main(int argc, char* argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = _orb->string_to_object (ior);

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: string_to_object(<ior>) returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved object reference" << std::endl;

      IDL::traits<Test::Hello_Factory>::ref_type hello_factory = IDL::traits<Test::Hello_Factory>::narrow (obj);

      if (hello_factory == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello_Factory>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "narrowed Hello_Factory interface" << std::endl;

      IDL::traits<Test::Hello>::ref_type hello;
      Test::Hello2::_ref_type hello2;
      hello_factory->get_hello ("This is the client",
                                hello,
                                hello2);

      if (hello == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: Test::Hello_Factory::get_hello () returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "hello::get_string returned : " << hello->get_string() << std::endl;

      Test::StringInterface::_ref_type si;
      hello2->get_string (si);

      if (si)
        {
          TAOX11_TEST_ERROR << "ERROR: The StringInterface should be nil!" << std::endl;
          return 1;
        }
      else
        TAOX11_TEST_DEBUG << "StringInterface is seems nil" << std::endl;

      TAOX11_TEST_DEBUG << "shutting down...";

      hello->shutdown ();
      _orb->destroy ();
      TAOX11_TEST_DEBUG << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }
  return 0;
}
