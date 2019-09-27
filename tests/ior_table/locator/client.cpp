/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "simple_testC.h"

#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

std::string ior ("corbaloc:iiop:localhost:19123/SIMPLE_TEST_KEY");
bool test_not_found = false;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("a:t:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'a':
        ior = get_opts.opt_arg ();
        break;
      case 't':
        test_not_found = std::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -a <corbaloc>" << std::endl;
        return false;
      }
  return true;
}

int
main (int argc, char *argv[])
{
  IDL::traits<CORBA::ORB>::ref_type orb;
  try
    {
      orb =
        CORBA::ORB_init (argc, argv, "MY_TEST_ORB");

      if (orb == nullptr)
        {
          TAOX11_TEST_ERROR
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type obj = orb->string_to_object (ior);

      if (obj == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: string_to_object returned a "
          << "nullptr!" << std::endl;
          return 1;
        }
      IDL::traits<Simple::SimpleTest>::ref_type simpleTestClient =
        IDL::traits<Simple::SimpleTest>::narrow (obj);

      if (simpleTestClient == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: Simple client is nil!" << std::endl;
          return 1;
        }

      simpleTestClient->shutdown ();

    }
  catch (const CORBA::OBJECT_NOT_EXIST &)
    {
      if (test_not_found)
        {
          TAOX11_TEST_DEBUG << "OK: caught the correct exception." << std::endl;
        }
      else
        {
          TAOX11_TEST_ERROR << "ERROR: Unexpected CORBA::OBJECT_NOT_EXIST "
            "exception caught" << std::endl;
        }
    }
  catch (const CORBA::Exception &ex)
    {
      TAOX11_TEST_ERROR << "ERROR (client): caught a CORBA exception : "
        << ex << std::endl;
      return 1;
    }

  orb->destroy ();

  return 0;
}
