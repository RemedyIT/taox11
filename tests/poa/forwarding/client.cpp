/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA forwarding test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "ace/Get_Opt.h"
#include "testC.h"
#include "testlib/taox11_testlog.h"

static std::string ior_file_ ("");
static int servers_ = 2;
static int iterations_ = 3;

static bool
parse_args (int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("s:i:k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior_file_ = get_opts.opt_arg ();
        break;
      case 's':
        servers_ = atoi (get_opts.opt_arg ());
        break;
      case 'i':
        iterations_ = atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
          TAOX11_TEST_ERROR << "usage:  -k IOR" << std::endl << "-i iterations"
            << std::endl;
    }

  if (ior_file_.length() == 0)
    {
      TAOX11_TEST_ERROR << "Please specify the IOR for the servant"
                << std::endl;
      return false;
    }

  // Indicates successful parsing of command line.
  return true;
}

void
do_calls (IDL::traits<test>::ref_type test)
{
  for (int j = 1; j <= servers_; ++j)
    {
      for (int i = 1; i <= iterations_; ++i)
        {
          // Invoke the doit() method of the test reference.
          int32_t result = test->doit ();

          // Print the result of doit () method of the test reference.
          TAOX11_TEST_DEBUG << "doit() returned " << result << std::endl;
        }

      // Don't forward the last server
      if (j != servers_)
        {
          TAOX11_TEST_DEBUG << "Asking server to forward next time" << std::endl;
          test->forward ();
          TAOX11_TEST_DEBUG << "Server should now forward requests" << std::endl;
        }
    }
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize the ORB
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      // Initialize options based on command-line arguments.
      if (parse_args (argc, argv) == false)
        return 1;

      // Get an object reference from the argument string.
      IDL::traits<CORBA::Object>::ref_type object =
        orb->string_to_object (ior_file_);

      // Try to narrow the object reference to a test reference.
      IDL::traits<test>::ref_type test_ref = IDL::traits<test>::narrow (object);

      do_calls (test_ref);

      test_ref->shutdown ();

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Error <client>: Unexpected exception caught : ";
      TAOX11_TEST_ERROR << ex << std::endl;
      return 1;
    }

  return 0;
}
