/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "client_task.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

std::string ior ("file://test.ior");
int iterations = 500;
int threads = 4;

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:i:n:") );
  int c;

  while ( (c = get_opts () ) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = atoi (get_opts.opt_arg () );
        break;

      case 'n':
        threads = atoi (get_opts.opt_arg () );
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
                     "-k <ior>\n"
                     "-i <iterations>\n"
                     "-n <threads>\n"
                     << std::endl;
        return false;
      }
  return true;
}

int
main (int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (parse_args (argc, argv) == false)
        return 1;

      IDL::traits<CORBA::Object>::ref_type tmp =
        orb->string_to_object (ior);

      IDL::traits<Test::Sleep_Service>::ref_type sleep_service =
        IDL::traits<Test::Sleep_Service>::narrow (tmp);

      if (!sleep_service)
        {
          TAOX11_TEST_ERROR << "Nil process reference <"
            << ior << std::endl;
          return 1;
        }

      Client_Task task0 (orb,
                         sleep_service,
                         iterations,
                         10 * 1000,
                         20 * 1000,
                         0);
      Client_Task task1 (orb,
                         sleep_service,
                         iterations,
                         50 * 1000,
                         20 * 1000,
                         1);

      if (task0.activate (THR_NEW_LWP | THR_JOINABLE, threads, 1) == -1)
        {
          TAOX11_TEST_ERROR << "Error activating client task 0" << std::endl;
          return 1;
        }
      if (task1.activate (THR_NEW_LWP | THR_JOINABLE, threads, 1) == -1)
        {
          TAOX11_TEST_ERROR << "Error activating client task 1" << std::endl;
          return 1;
        }
      ACE_Thread_Manager::instance ()->wait ();

      TAOX11_TEST_DEBUG << "Client threads are ready" << std::endl;

      // Get back in sync with the server..
      sleep_service->go_to_sleep (0);

      TAOX11_TEST_DEBUG << "Shutting down server" << std::endl;

      sleep_service->shutdown ();

      TAOX11_TEST_DEBUG << "Destroying ORB" << std::endl;

      orb->destroy ();

      TAOX11_TEST_DEBUG << "Task 0: Successful calls = "
        << task0.successful_calls () << ", timed out calls = "
        << task0.timed_out_calls () << std::endl;

      TAOX11_TEST_DEBUG << "Task 1: Successful calls = "
        << task1.successful_calls () << ", timed out calls = "
        << task1.timed_out_calls () << std::endl;

      // No more than 5 % of the calls are allowed to have a too big
      // difference
      if (task0.too_big_difference_calls () > iterations/20
          || task1.too_big_difference_calls () > iterations/20)
        {
          TAOX11_TEST_INFO << "Warning: Too many calls have a too big difference between "
            << "timeout and elapsed time (task0: " << task0.too_big_difference_calls ()
            << ", task1: " << task1.too_big_difference_calls () << ")" << std::endl;
        }
      else  if (task0.timed_out_calls () != 0 || task1.timed_out_calls () != 0)
        {
          TAOX11_TEST_INFO << "Warning: some calls (task0: " << task0.timed_out_calls ()
            << ", task1: " << task1.timed_out_calls ()
            << ") exceeded their expected elapsed times" << std::endl;
        }

      if (task0.timed_out_calls () > 0)
        {
          TAOX11_TEST_WARNING << "unexpected number of timeouts "
            << "for task 0 - expected <0> - found <" << task0.timed_out_calls () << ">" << std::endl;
        }

      if (task1.successful_calls () != 0)
        {
          TAOX11_TEST_ERROR << "ERROR: unexpected number of successful calls "
            << "for task 1 - expected <0> - found <" << task1.successful_calls ()
            << ">" << std::endl;
          return 1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Client - main - " << "Exception caught : " << ex << std::endl;
      return 1;
    }

  return 0;
}
