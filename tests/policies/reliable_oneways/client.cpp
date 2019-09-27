/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testC.h"
#include "tao/x11/messaging/messaging.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/Policy_CurrentC.h"
#include "testlib/taox11_testlog.h"

#include "ace/Get_Opt.h"

std::string ior = "file://test.ior";
bool test_sync_with_target    = false;
bool test_sync_with_server    = false;
bool test_sync_with_transport = false;
bool test_sync_none           = false;
int iterations = 500;

const int ALIVE     = 0;
const int DESTROYED = 1;
const int SHUTDOWN  = 2;
int server_status = ALIVE;

int successful_calls = 0;
int failed_calls = 0;
int test_runtime = 10; // sec

int object_ne = 0;
int no_permission = 0;
int transient = 0;
int comm_failure = 0;

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:i:ntsrd:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case 'i':
        iterations = std::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        test_sync_none = true;
        break;

      case 't':
        test_sync_with_transport = true;
        break;

      case 's':
        test_sync_with_server = true;
        break;

      case 'r':
        test_sync_with_target = true;
        break;

      case 'd':
        test_runtime = std::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
              << "-k <ior>\n"
              << "-n -t -s -r"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

void
set_sync_scope_policy (Messaging::SyncScope sync_scope)
{
  int argc = 0;
  ACE_TCHAR **argv = 0;
  IDL::traits<CORBA::ORB>::ref_type orb =
    CORBA::ORB_init (argc, argv);

  IDL::traits<CORBA::Object>::ref_type object =
    orb->resolve_initial_references ("PolicyCurrent");

  IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current =
    IDL::traits<CORBA::PolicyCurrent>::narrow (object);

  if (!policy_current)
    {
      TAOX11_TEST_ERROR << "set_sync_scope_policy - ERROR: Nil policy current"
            << std::endl;
      return;
    }
  CORBA::Any scope_as_any;
  scope_as_any <<= sync_scope;

  CORBA::PolicyList policies(1);
  policies[0] =
    orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        scope_as_any);

  // narrow from generic policy ref
  IDL::traits<Messaging::SyncScopePolicy>::ref_type ss_pol =
      IDL::traits<Messaging::SyncScopePolicy>::narrow (policies[0]);

  if (!ss_pol)
  {
    TAOX11_TEST_ERROR << "Unable to narrow Messaging::SyncScopePolicy"
        << std::endl;
    return;
  }

  if (ss_pol->synchronization () != sync_scope)
  {
    TAOX11_TEST_ERROR << "Incorrect Messaging::SyncScopePolicy returned"
        << std::endl;
    return;
  }

  // implicitly widen to object ref
  IDL::traits<CORBA::Object>::ref_type polobj = ss_pol;

  // (re-)narrow
  ss_pol = IDL::traits<Messaging::SyncScopePolicy>::narrow (polobj);

  if (!ss_pol)
  {
    TAOX11_TEST_ERROR << "Unable to narrow Messaging::SyncScopePolicy from CORBA::Object reference"
        << std::endl;
    return;
  }

  policy_current->set_policy_overrides (policies,
                                        CORBA::SetOverrideType::ADD_OVERRIDE);

  policies[0]->destroy ();
}

void
run_one_iteration (int i,
                   IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver,
                   IDL::traits<Test::Shutdown_Helper>::ref_type shutdown_helper)
{
  try
  {
    if (i == iterations / 3)
    {
      TAOX11_TEST_DEBUG << "Destroying object" << std::endl;
      server_status = DESTROYED;
      oneway_receiver->destroy ();
    }
    if (i == 2 * iterations / 3)
    {
      TAOX11_TEST_DEBUG << "Shutting down server process" << std::endl;
      server_status = SHUTDOWN;
      shutdown_helper->shutdown ();
    }
  }
  catch (const CORBA::Exception&)
  {
  }

  oneway_receiver->raise_no_permission ();
}

void
call_failed (const CORBA::SystemException &ex)
{
  failed_calls++;
  TAOX11_TEST_ERROR << "Exception raised: " << ex << std::endl;
}

void
run_test_sync_with_target (IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_TARGET);

  IDL::traits<Test::Shutdown_Helper>::ref_type shutdown_helper =
    oneway_receiver->get_shutdown_helper ();

  for (int i = 0; i != iterations; ++i)
  {
    try
    {
      run_one_iteration (i, oneway_receiver, shutdown_helper);
    }
    catch (const CORBA::NO_PERMISSION& ex)
    {
      no_permission ++;
      if (server_status == ALIVE)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::OBJECT_NOT_EXIST& ex)
    {
      object_ne ++;
      if (server_status == DESTROYED)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::COMM_FAILURE& ex)
    {
      comm_failure ++;
      if (server_status == SHUTDOWN && transient == 0)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::TRANSIENT& ex)
    {
      transient ++;
      if (server_status == SHUTDOWN)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::SystemException& ex)
    {
      call_failed (ex);
    }
  }
}

void
run_test_sync_with_server (IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_SERVER);

  IDL::traits<Test::Shutdown_Helper>::ref_type shutdown_helper =
    oneway_receiver->get_shutdown_helper ();

  for (int i = 0; i != iterations; ++i)
  {
    try
    {
      run_one_iteration (i, oneway_receiver, shutdown_helper);

      if (server_status == DESTROYED
          || server_status == SHUTDOWN)
        failed_calls++;
      else
        successful_calls++;
    }
    catch (const CORBA::NO_PERMISSION& ex)
    {
      no_permission ++;
      call_failed (ex);
    }
    catch (const CORBA::OBJECT_NOT_EXIST& ex)
    {
      object_ne ++;
      if (server_status == DESTROYED)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::COMM_FAILURE& ex)
    {
      comm_failure ++;
      if (server_status == SHUTDOWN && transient == 0)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::TRANSIENT& ex)
    {
      transient ++;
      if (server_status == SHUTDOWN)
        successful_calls++;
      else
        call_failed (ex);
    }
    catch (const CORBA::SystemException& ex)
    {
      call_failed (ex);
    }
  }
}

void
run_test_unreliable (IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver);

void
run_test_sync_with_transport (IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_WITH_TRANSPORT);

  run_test_unreliable (oneway_receiver);
}

void
run_test_sync_none (IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver)
{
  set_sync_scope_policy (Messaging::SYNC_NONE);

  run_test_unreliable (oneway_receiver);
}


void
run_test_unreliable (IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver)
{
  IDL::traits<Test::Shutdown_Helper>::ref_type shutdown_helper =
    oneway_receiver->get_shutdown_helper ();

  int undetected = 0;
  for (int i = 0; i != iterations; ++i)
    {
      try
      {
        run_one_iteration (i, oneway_receiver, shutdown_helper);

        // A few failures can go undetected...
        if (server_status == SHUTDOWN)
          undetected++;

        // Once a TRANSIENT is raised all future calls should fail.
        if (transient > 0)
          failed_calls++;
        else
          successful_calls++;
      }
      catch (const CORBA::NO_PERMISSION& ex)
      {
        no_permission ++;
        call_failed (ex);
      }
      catch (const CORBA::OBJECT_NOT_EXIST& ex)
      {
        object_ne ++;
        call_failed (ex);
      }
      catch (const CORBA::COMM_FAILURE& ex)
      {
        comm_failure ++;
        call_failed (ex);
      }
      catch (const CORBA::TRANSIENT& ex)
      {
        transient ++;
        if (server_status == SHUTDOWN)
          successful_calls++;
        else
          call_failed (ex);
      }
      catch (const CORBA::SystemException& ex)
      {
        call_failed (ex);
      }
    }
  TAOX11_TEST_DEBUG << "Undetected TRANSIENT count = " << undetected << std::endl;
}

int
main (int argc, char *argv[])
{
  int errorCount = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type tmp =
        orb->string_to_object(ior);

      IDL::traits<Test::Oneway_Receiver>::ref_type oneway_receiver =
        IDL::traits<Test::Oneway_Receiver>::narrow (tmp);

      if (!oneway_receiver)
        {
          TAOX11_TEST_ERROR << "Nil oneway receiver reference <"
                << ior << ">" << std::endl;
          return 1;
        }

      if (test_sync_with_target)
        {
          TAOX11_TEST_DEBUG << "========================" << std::endl;
          TAOX11_TEST_DEBUG << "Running SYNC_WITH_TARGET" << std::endl;
          run_test_sync_with_target (oneway_receiver);
        }
      else if (test_sync_with_server)
        {
          TAOX11_TEST_DEBUG << "========================" << std::endl;
          TAOX11_TEST_DEBUG << "Running SYNC_WITH_SERVER" << std::endl;
          run_test_sync_with_server (oneway_receiver);
        }
      else if (test_sync_with_transport)
        {
          TAOX11_TEST_DEBUG << "===========================" << std::endl;
          TAOX11_TEST_DEBUG << "Running SYNC_WITH_TRANSPORT" << std::endl;
          run_test_sync_with_transport (oneway_receiver);
        }
      else
        {
          TAOX11_TEST_DEBUG << "Running SYNC_NONE" << std::endl;
          run_test_sync_none (oneway_receiver);
        }

      std::chrono::seconds time (test_runtime);

      orb->run (time);
      orb->destroy ();

      TAOX11_TEST_INFO << "Detected " << no_permission << " NO_PERMISSION, "
                                      << object_ne << " OBJECT_NOT_EXIST, "
                                      << comm_failure << " COMM_FAILURE, "
                                      << transient << " TRANSIENT exceptions"
                                      << std::endl;

      if (successful_calls == 0)
        {
          ++errorCount;
          TAOX11_TEST_ERROR << "ERROR: No requests were successful" << std::endl;
        }
      else
        TAOX11_TEST_DEBUG << "<" << successful_calls
              << "> successful calls detected" << std::endl;

      if (failed_calls != 0)
        {
          ++errorCount;
          TAOX11_TEST_ERROR << "ERROR: At least <" << failed_calls << "> requests failed."
            << std::endl;
        }
      else
        TAOX11_TEST_DEBUG << "No failing calls detected!" << std::endl;

      if (failed_calls + successful_calls != iterations)
        {
          ++errorCount;
          TAOX11_TEST_ERROR << "ERROR: Mismatched number of calls (" << successful_calls
          << " + " << failed_calls << " != " << iterations << std::endl;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Exception caught <client> : " << ex << std::endl;
      return 1;
    }

    return errorCount;
}
