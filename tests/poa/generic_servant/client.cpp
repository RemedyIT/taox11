/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "ace/Profile_Timer.h"
#include "ace/Read_Buffer.h"
#include "testC.h"

#include "testlib/taox11_testlog.h"

static std::string IOR("");
static int iterations = 1;
static int oneway = 0;
static bool shutdown_server = false;
static unsigned long timeout = 5;
static bool timed_method = false;

static bool
parse_args(int argc, char **argv)
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("f:k:i:T:otx"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
    case 'k':
      IOR = get_opts.opt_arg();
      break;

    case 'o':
      oneway = 1;
      break;

    case 't':
      timed_method = true;
      break;

    case 'i':
      iterations = atoi(get_opts.opt_arg());
      break;

    case 'T':
      timeout = static_cast<unsigned long>(atoi(get_opts.opt_arg()));
      break;

    case 'x':
      shutdown_server = true;
      break;

    case '?':
    default:
      TAOX11_TEST_ERROR << "usage:\n"
          "-k IOR\n"
          "-o oneway\n"
          "-t timed operations\n"
          "-T timeout for timed operations\n"
          "-i iterations\n"
          "-x shutdown server\n" << std::endl;
      return false;
    }

  if (IOR.empty())
  {
    TAOX11_TEST_ERROR << "Please specify the IOR for the servant" << std::endl;
    return false;
  }

  // Indicates successful parsing of command line.
  return true;
}

bool
print_stats(ACE_Profile_Timer::ACE_Elapsed_Time &elapsed_time,
    int iterations)
{
  if (iterations > 0)
  {
    elapsed_time.real_time *= ACE_ONE_SECOND_IN_MSECS;
    elapsed_time.user_time *= ACE_ONE_SECOND_IN_MSECS;
    elapsed_time.system_time *= ACE_ONE_SECOND_IN_MSECS;

    elapsed_time.real_time /= iterations;
    elapsed_time.user_time /= iterations;
    elapsed_time.system_time /= iterations;

    double tmp = 1000 / elapsed_time.real_time;

    TAOX11_TEST_INFO << "\titerations\t = " << iterations << std::endl;
    TAOX11_TEST_INFO << "\treal_time\t = "
        << (elapsed_time.real_time < 0.0 ? 0.0 : elapsed_time.real_time)
        << " ms" << std::endl;
    TAOX11_TEST_INFO << "\tuser_time\t = "
        << (elapsed_time.user_time < 0.0 ? 0.0 : elapsed_time.user_time)
        << " ms" << std::endl;
    TAOX11_TEST_INFO << "\tsystem_time\t = "
        << (elapsed_time.system_time < 0.0 ? 0.0 : elapsed_time.system_time)
        << " ms" << std::endl;
    TAOX11_TEST_INFO << "\t" << (tmp < 0.0 ? 0.0 : tmp) << " calls/second"
        << std::endl;
    return true;
  } else
  {
    TAOX11_TEST_ERROR << "\tNo time stats printed. Zero iterations or error occurred."
        << std::endl;
    return false;
  }
}

int
main(int argc, char *argv[])
{

  try
  {
    // Initialize the ORB
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);

    // Initialize options based on command-line arguments.
    if (!parse_args(argc, argv))
      return 1;

    // Get an object reference from the argument string.
    IDL::traits<CORBA::Object>::ref_type object = orb->string_to_object(IOR);

    // Try to narrow the object reference to a test reference.
    IDL::traits<test>::ref_type test_ref = IDL::traits<test>::narrow (object);

    std::string ior = orb->object_to_string(test_ref);

    TAOX11_TEST_INFO << "Connecting to: " << ior << std::endl;

    ACE_Profile_Timer timer;
    ACE_Profile_Timer::ACE_Elapsed_Time elapsed_time;

    // We start an ACE_Profile_Timer here...
    timer.start();

    int i = 0;

    for (i = 0; i < iterations; i++)
    {
      if (oneway && timed_method)
      {
        test_ref->timed_oneway_method(timeout);
      } else if (oneway)
      {
        test_ref->oneway_method();
      } else if (!oneway && timed_method)
      {
        test_ref->timed_method(timeout);
      } else
      {
        test_ref->method();
      }
    }

    // stop the timer.
    timer.stop();
    timer.elapsed_time(elapsed_time);

    // compute average time.
    if (!print_stats(elapsed_time, i))
      return 1;

    if (shutdown_server)
    {
      test_ref->shutdown();
    }

    orb->destroy();
  } catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "CLIENT EXCEPTION : " << ex << std::endl;
    return 1;
  }

  return 0;
}
