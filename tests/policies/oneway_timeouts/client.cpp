/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "testS.h"
#include "tao/x11/messaging/messaging.h"
#include "tao/x11/TAOC.h"
#include "tao/x11/Policy_CurrentC.h"
#include "ace/High_Res_Timer.h"
#include "ace/Arg_Shifter.h"
#include <thread>

#include "testlib/taox11_testlog.h"

#define TIME_THRESHOLD    100
#define NON_EXISTENT_IOR  "corbaloc:iiop:1.2@63.246.9.65:12345/test"

namespace
{
  TimeBase::TimeT       request_timeout = 0;
  Messaging::SyncScope  sync_scope;
  bool                  use_buf_constraints = false;
  bool                  use_sync_scope = false;

  uint32_t              bc_count = 0;
  uint32_t              bc_bytes = 0;
  TimeBase::TimeT       bc_timeout = 0;

  uint16_t num_requests = 10;
  uint16_t request_interval = 50;

  TimeBase::TimeT connect_timeout = 0;
  uint16_t        run_orb_delay = 0;
  uint16_t        run_orb_time = 0;
  bool            force_timeout = false;

  // This will force a blocking connection before starting the test
  // by sending the num_requests as a twoway.
  uint16_t max_request_time = 0;
  bool force_connect        = false;
  bool use_sleep            = false;
  bool use_twoway           = false;
  bool retry_transients     = false;
  bool retry_timeouts       = false;
  bool make_request_queued  = false;
  std::string server_ior ("file://test.ior");

  void print_usage ()
  {
    TAOX11_TEST_ERROR <<
      "usage: [-request_timeout ms=0]\n"
      "[-connect_timeout ms=0]\n"
      "[-request_interval ms=100]\n"
      "[-run_orb_delay ms=0]\n"
      "[-run_orb_time ms=0]\n"
      "[-max_request_time ms=0]\n"
      "[-num_requests n=10]\n"
      "[-use_twoway]\n"
      "[-retry_transients]\n"
      "[-retry_timeouts]\n"
      "[-use_sleep]\n"
      "[-force_timeout]\n"
      "[-force_connect]\n"
      "[-buffer_count n=0]\n"
      "[-buffer_bytes n=0]\n"
      "[-buffer_timeout ms=0]\n"
      "[-sync delayed|none]\n"
      "[-make_request_queued]\n"
      "[-server_ior <ior>]\n"
      << std::endl;
  }

  bool parse_command_line (int ac, ACE_TCHAR *av[])
  {
    ACE_Arg_Shifter args (ac, av);
    args.consume_arg ();

    while (args.is_anything_left ())
    {
      if (args.cur_arg_strncasecmp (ACE_TEXT("-request_timeout")) == 0)
      {
        args.consume_arg ();
        request_timeout = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-connect_timeout")) == 0)
      {
        args.consume_arg ();
        connect_timeout = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-request_interval")) == 0)
      {
        args.consume_arg ();
        request_interval = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-run_orb_delay")) == 0)
      {
        args.consume_arg ();
        run_orb_delay = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-run_orb_time")) == 0)
      {
        args.consume_arg ();
        run_orb_time = std::atoi(args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-max_request_time")) == 0)
      {
        args.consume_arg ();
        max_request_time = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-num_requests")) == 0)
      {
        args.consume_arg ();
        num_requests = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-use_twoway")) == 0)
      {
        use_twoway = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-retry_transients")) == 0)
      {
        retry_transients = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-retry_timeouts")) == 0)
      {
        retry_timeouts = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-use_sleep")) == 0)
      {
        use_sleep = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-force_timeout")) == 0)
      {
        force_timeout = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-force_connect")) == 0)
      {
        force_connect = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-buffer_count")) == 0)
      {
        args.consume_arg ();
        use_buf_constraints = true;
        bc_count =  std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-buffer_bytes")) == 0)
      {
        args.consume_arg ();
        use_buf_constraints = true;
        bc_bytes = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-buffer_timeout")) == 0)
      {
        args.consume_arg ();
        use_buf_constraints = true;
        bc_timeout = std::atoi (args.get_current ());
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-sync")) == 0)
      {
        args.consume_arg ();
        if (args.cur_arg_strncasecmp (ACE_TEXT("delayed")) == 0)
        {
          sync_scope = TAO::SYNC_DELAYED_BUFFERING;
          use_sync_scope = true;
        }
        else if (args.cur_arg_strncasecmp (ACE_TEXT("none")) == 0)
        {
          sync_scope = Messaging::SYNC_NONE;
          use_sync_scope = true;
        }
        else
        {
          print_usage ();
          return false;
        }
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-make_request_queued")) == 0)
      {
        make_request_queued = true;
        args.consume_arg ();
      }
      else if (args.cur_arg_strncasecmp (ACE_TEXT("-server_ior")) == 0)
      {
        args.consume_arg ();
        server_ior = args.get_current ();
        args.consume_arg ();
      }
      else
      {
        TAOX11_TEST_ERROR << "Error: Unknown argument \""
          << args.get_current () << "\""
          << std::endl;
        print_usage ();
        return false;
      }

    }
    return true;
  }

  IDL::traits<Tester>::ref_type
  set_request_timeout (
    IDL::traits<Tester>::ref_type tst,
    IDL::traits<CORBA::ORB>::ref_type orb)
  {
    if (request_timeout <= 0)
      return tst;

    CORBA::Any a;
    a <<= request_timeout * 10000;
    CORBA::PolicyList pols (1);
    pols[0] =
      orb->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE, a);
    IDL::traits<CORBA::Object>::ref_type obj =
      tst->_set_policy_overrides (pols, CORBA::SetOverrideType::ADD_OVERRIDE);
    pols[0]->destroy ();
    return IDL::traits<Tester>::narrow (obj);
  }


  int
  set_connect_timeout (IDL::traits<CORBA::ORB>::ref_type orb)
  {
    if (connect_timeout <= 0)
      return 0;
    IDL::traits<CORBA::Object>::ref_type obj =
      orb->resolve_initial_references ("PolicyCurrent");
    IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current =
      IDL::traits<CORBA::PolicyCurrent>::narrow (obj);
    CORBA::Any a;
    a <<= connect_timeout * 10000;
    CORBA::PolicyList pols (1);
    pols[0] = orb->create_policy (TAO::CONNECTION_TIMEOUT_POLICY_TYPE, a);

    // narrow from generic policy ref
    IDL::traits<TAO::ConnectionTimeoutPolicy>::ref_type bc_pol =
        IDL::traits<TAO::ConnectionTimeoutPolicy>::narrow (pols[0]);

    if (!bc_pol)
    {
      TAOX11_TEST_ERROR << "Unable to narrow TAO::ConnectionTimeoutPolicy"
          << std::endl;
      return 1;
    }

    if (bc_pol->relative_expiry () != connect_timeout * 10000)
    {
      TAOX11_TEST_ERROR << "Incorrect TAO::ConnectionTimeoutPolicy returned"
          << std::endl;
      return 1;
    }

    policy_current->set_policy_overrides (pols, CORBA::SetOverrideType::ADD_OVERRIDE);
    pols[0]->destroy ();

    return 0;
  }

  int
  set_buffering (IDL::traits<CORBA::ORB>::ref_type orb)
  {
    IDL::traits<CORBA::Object>::ref_type obj =
      orb->resolve_initial_references ("PolicyCurrent");
    IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current =
      IDL::traits<CORBA::PolicyCurrent>::narrow (obj);
    CORBA::PolicyList pols (1);

    if (use_sync_scope)
    {
      CORBA::Any a;
      if (make_request_queued)
        a <<= Messaging::SYNC_NONE;
      else
        a <<= sync_scope;
      pols[0] = orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE, a);
      policy_current->set_policy_overrides (pols, CORBA::SetOverrideType::ADD_OVERRIDE);
      pols[0]->destroy ();
    }

    if (use_buf_constraints)
    {
      TAO::BufferingConstraintMode bc_mode = TAO::BUFFER_FLUSH;
      TAO::BufferingConstraint bc;

      if (bc_count > 0)
      {
        bc_mode |= TAO::BUFFER_MESSAGE_COUNT;
      }

      if (bc_bytes > 0)
      {
        bc_mode |= TAO::BUFFER_MESSAGE_BYTES;
      }

      if (bc_timeout > 0)
      {
        bc_mode |= TAO::BUFFER_TIMEOUT;
      }

      bc.mode (bc_mode);
      bc.message_count (bc_count);
      bc.message_bytes (bc_bytes);
      bc.timeout (bc_timeout * 10000);
      CORBA::Any a;
      a <<= bc;
      pols[0] = orb->create_policy (TAO::BUFFERING_CONSTRAINT_POLICY_TYPE, a);

      // narrow from generic policy ref
      IDL::traits<TAO::BufferingConstraintPolicy>::ref_type bc_pol =
          IDL::traits<TAO::BufferingConstraintPolicy>::narrow (pols[0]);

      if (!bc_pol)
      {
        TAOX11_TEST_ERROR << "Unable to narrow TAO::BufferingConstraintPolicy"
            << std::endl;
        return 1;
      }

      if ((bc_pol->buffering_constraint().mode () != bc.mode ()) ||
          (bc_pol->buffering_constraint().timeout () != bc.timeout ()) ||
          (bc_pol->buffering_constraint().message_count () != bc.message_count ()) ||
          (bc_pol->buffering_constraint().message_bytes () != bc.message_bytes ()))
      {
        TAOX11_TEST_ERROR << "Incorrect TAO::BufferingConstraintPolicy returned"
            << std::endl;
        return 1;
      }

      policy_current->set_policy_overrides (pols, CORBA::SetOverrideType::ADD_OVERRIDE);

      pols[0]->destroy ();
    }

    return 0;
  }

  void
  reset_buffering (IDL::traits<CORBA::ORB>::ref_type orb)
  {
    IDL::traits<CORBA::Object>::ref_type obj =
      orb->resolve_initial_references ("PolicyCurrent");
    IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current =
      IDL::traits<CORBA::PolicyCurrent>::narrow (obj);
    CORBA::PolicyList pols (1);

    CORBA::Any a;
    a <<= sync_scope;
    pols[0] = orb->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE, a);
    policy_current->set_policy_overrides (pols, CORBA::SetOverrideType::ADD_OVERRIDE);
    pols[0]->destroy ();
  }
}

int main (int ac, char *av[])
{

  ACE_Time_Value before = ACE_High_Res_Timer::gettimeofday_hr ();

  uint16_t num_requests_sent = 0;

  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (ac, av);

    if (!parse_command_line (ac, av))
      return 1;

    if (set_connect_timeout (orb) != 0)
      return 1;
    if (set_buffering (orb) != 0)
      return 1;

    std::string ior (server_ior);
    if (force_timeout)
    {
      ior = NON_EXISTENT_IOR;
    }

    IDL::traits<CORBA::Object>::ref_type obj =
      orb->string_to_object (ior.c_str ());

    IDL::traits<Tester>::ref_type tmp_tester;
    if (force_connect)
    {
      tmp_tester = IDL::traits<Tester>::narrow (obj);
      tmp_tester->test2 (-2);
      TAOX11_TEST_DEBUG <<"Connected..." << std::endl;
    }
    else
      tmp_tester = IDL::traits<Tester>::narrow (obj);

    IDL::traits<Tester>::ref_type tester =
      set_request_timeout (tmp_tester, orb);

    if (!tester)
    {
      TAOX11_TEST_ERROR << "Error: tester seems nil" << std::endl;
      return 1;
    }
    int32_t i = 0;

    // Using make_request_queued option to work around test failure due to
    // different connection establishment behavior between OS.  Some system
    // can connect immediately and some may take longer time. With the flag on,
    // the test sets the SYNC_NONE scope and sends a request so the transport
    // queue is not empty for some SYNC_DELAYED_BUFFERING test case and hence
    // the requests are all queued and will be received by server continuously
    // during a short period.
    if (make_request_queued)
    {
      // Send this message while using SYNC_NONE.
      // This would leave the request in transport queue.
      tester->test (-3);
      // Set to SYNC_DELAYED_BUFFERING.
      // The requests will be queued since queue is not empty.
      reset_buffering (orb);
    }

    for (; i < num_requests; ++i)
    {
      before = ACE_High_Res_Timer::gettimeofday_hr ();
      try
      {
        if (use_twoway)
        {
          tester->test2 (i);
        }
        else
        {
          tester->test (i);
        }
      }
      catch (const CORBA::TRANSIENT&)
      {
        TAOX11_TEST_DEBUG << "Transient exception during test () "
          << "invocation " << i << std::endl;
        if (retry_transients)
          TAOX11_TEST_DEBUG << "retrying" << std::endl;
        else
          throw;
      }
      catch (const CORBA::TIMEOUT&)
      {
        TAOX11_TEST_DEBUG << "Timeout exception during test () "
          << "invocation " << i << std::endl;

        // This exception is expected with forced timeouts, since the
        // IOR is invalid. Unless we want to retry the invocation
        // go ahead and rethrow and let the outer catch deal with it.
        // Likewise if force_timeouts is not set, then throw it anyway
        // because the exception should not occur because these are
        // oneways.
        if (retry_timeouts)
          TAOX11_TEST_DEBUG << "retrying" << std::endl;
        else
          throw;
      }

      ++num_requests_sent;

      ACE_Time_Value after = ACE_High_Res_Timer::gettimeofday_hr ();
      if (max_request_time > 0 &&
          (after - before).msec () > max_request_time)
      {
        TAOX11_TEST_DEBUG << "note: test() took "
          << (after - before).msec () << "ms, "
          << "max is " << max_request_time
          << " ms" << std::endl;
      }

      TAOX11_TEST_DEBUG << "c" << i << std::endl;

      if (request_interval > 0)
      {
        ACE_Time_Value tv (0, request_interval * 1000);
        ACE_Time_Value done = tv +
          ACE_High_Res_Timer::gettimeofday_hr ();
        if (!use_sleep)
        {
          orb->run (tv);
        }
        else
        {
          std::this_thread::sleep_for (std::chrono::milliseconds (request_interval));
        }

        while (ACE_High_Res_Timer::gettimeofday_hr () < done)
        {
          std::this_thread::sleep_for (std::chrono::seconds (0));
        }
      }
    }

    TAOX11_TEST_DEBUG << "request loop complete" << std::endl;


    if (run_orb_delay > 0)
    {
      std::this_thread::sleep_for (std::chrono::milliseconds (run_orb_delay));
    }

    if (run_orb_time > 0)
    {
      ACE_Time_Value tv (0, run_orb_time * 1000);
      orb->run (tv);
    }

    TAOX11_TEST_DEBUG << "Sending synch request to shutdown server" << std::endl;
    use_twoway = true;
    use_sync_scope = false;

    if (force_timeout)
    {
      // we have one more invocation that may time out.
      before = ACE_High_Res_Timer::gettimeofday_hr ();
      tester->test2 (-1);
    }
    else
    {
      /// At this point, we aren't interested in the time it takes, we
      /// want the peer to shut down, so use the non-timeout reference.
      /// BUT IF THIS DOES raise a timeout, it will be reported as an
      /// error.
      tmp_tester->test2 (-1);
    }

    orb->shutdown (true);

    orb->destroy ();

    if (force_timeout)
    {
      TAOX11_TEST_ERROR << "Error: Connection did not time out."
            << std::endl;
      return 1;
    }

    return 0;
  }
  catch (const CORBA::TRANSIENT &)
  {
    TAOX11_TEST_DEBUG <<"caught transient exception" << std::endl;
    if (force_timeout)
    {
      ACE_Time_Value after = ACE_High_Res_Timer::gettimeofday_hr ();
      uint32_t ms = (after - before).msec ();
      if ((use_twoway || !use_sync_scope) &&
        request_timeout > 0 &&
        request_timeout < connect_timeout)
      {
        connect_timeout = request_timeout;
      }
      else if (use_sync_scope && !use_sleep)
      {
        if (ms > TIME_THRESHOLD)
        {
          TAOX11_TEST_DEBUG <<  "note: Buffered request took "
            << ms << " ms" << std::endl;
        }

        ms = num_requests_sent * request_interval;
      }

      if (ms - connect_timeout > TIME_THRESHOLD ||
          connect_timeout - ms > TIME_THRESHOLD)
        {
          TAOX11_TEST_DEBUG <<  "note: Timeout expected in "
            << connect_timeout << " ms, "
            << "but took " << ms << " ms"
            << std::endl;
        }

      return 0;
    }
    else
    {
      TAOX11_TEST_ERROR << "Error: Unexpected" << std::endl;
      return 1;
    }
  }
  catch (const CORBA::TIMEOUT &)
  {
    TAOX11_TEST_DEBUG <<"caught timeout exception" << std::endl;
    if (force_timeout)
    {
      ACE_Time_Value after = ACE_High_Res_Timer::gettimeofday_hr ();
      uint32_t ms = (after - before).msec ();
      if ((use_twoway || !use_sync_scope) &&
        request_timeout > 0 &&
        request_timeout < connect_timeout)
      {
        connect_timeout = request_timeout;
      }
      else if (use_sync_scope && !use_sleep)
      {
        if (ms > TIME_THRESHOLD)
        {
          TAOX11_TEST_DEBUG << "note: Buffered request took "
            << ms << " ms" << std::endl;
        }
        ms = num_requests_sent * request_interval;
      }

      if (ms - connect_timeout > TIME_THRESHOLD ||
        connect_timeout - ms > TIME_THRESHOLD)
      {
        TAOX11_TEST_DEBUG << "note: Timeout expected in "
          << connect_timeout << " ms, "
          << "but took " << ms << " ms"
          << std::endl;
      }
      return 0;
    }
    else
    {
      TAOX11_TEST_ERROR << "Error: Unexpected" << std::endl;
      return 1;
    }
  }
  catch (const CORBA::Exception &ex)
  {
    TAOX11_TEST_ERROR << "client: " << ex._name() << "\n\nLast operation took "
      << (ACE_High_Res_Timer::gettimeofday_hr () - before).msec () << "ms."
      << std::endl;
    return 1;
  }

  return 1;
}
