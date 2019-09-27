/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client IORTable application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testC.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

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
        TAOX11_TEST_ERROR << "ERROR: usage: -k <ior>" << std::endl;
        return false;
      }
  return true;
}

int
main (int argc, ACE_TCHAR *argv[])
{
  int16_t error = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type tmp =
        orb->string_to_object(ior);

      IDL::traits<Test::Hello>::ref_type hello =
        IDL::traits<Test::Hello>::narrow (tmp);

      if (hello == nullptr)
        {
          TAOX11_TEST_ERROR << "ERROR: hello reference seems nil"
            << std::endl;
          return 1;
        }

      std::string the_string = hello->get_string ();

      TAOX11_TEST_DEBUG << "string returned " << the_string << std::endl;

      if (the_string == "ERROR")
        {
          TAOX11_TEST_ERROR << "ERROR: Wrong servant has been invoked."
            << std::endl;
          error = 1;
        }

      hello->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "ERROR: Unexpected exception caught: "
        << ex << std::endl;
      return 1;
    }

  return error;
}
