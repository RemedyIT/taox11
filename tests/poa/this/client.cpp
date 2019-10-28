/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 client application for _this test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
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
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (obj) returned null object."
            << std::endl;
      return 1;
    }
    TAOX11_TEST_DEBUG << "narrowed Hello interface" << std::endl;

    IDL::traits<Test::iA>::ref_type ia = hello->some_function ();
    ia->do_something();

    TAOX11_TEST_DEBUG << "shutting down...";
    hello->shutdown ();
    TAOX11_TEST_DEBUG << std::endl;
    _orb->destroy ();
  }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }
  return 0;
}
