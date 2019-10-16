/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "testC.h"

#include "testlib/taox11_testlog.h"

std::string ior ("file://test.ior");
int niterations = 5;
int do_shutdown = 0;

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("xk:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'x':
        do_shutdown = 1;
        break;

      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        niterations = std::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
              << "-k <ior>\n"
              << "-i <niterations>\n"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int
main (int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type object =
        orb->string_to_object (ior);

      IDL::traits<Simple_Server>::ref_type server =
        IDL::traits<Simple_Server>::narrow (object);

      if (!server)
      {
        TAOX11_TEST_ERROR << "Object reference <"
              << ior << "> is nil" << std::endl;
        return 1;
      }

      for (int i = 0; i != niterations; ++i)
      {
        long r = server->test_method (i);

        if (r != i)
        {
          TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
                << "unexpected result = " << r
                << " for " << i << std::endl;
        }
      }

      if (do_shutdown)
      {
        server->shutdown ();
      }

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception <Client::svc> :" << ex << std::endl;
      return 1;
    }

  return 0;
}
