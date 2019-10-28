/**
 * @file    client_task.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "client_task.h"
#include "tao/x11/TimeBaseC.h"
#include "tao/x11/messaging/messaging.h"
#include "tao/x11/anytypecode/any.h"

#include "tao/x11/Policy_ForwardC.h"
#include "tao/x11/Policy_CurrentC.h"
#include <thread>

#include "testlib/taox11_testlog.h"

#define LOG_PREFIX "(" << x11_logger::threadid << ") "

Client_Task::Client_Task (IDL::traits<CORBA::ORB>::ref_type orb,
                          IDL::traits<Test::Sleep_Service>::ref_type sleep_service,
                          int iterations,
                          uint32_t sleep_time,
                          uint32_t timeout,
                          int task)
  : orb_ (std::move(orb))
  , sleep_service_ (std::move(sleep_service))
  , iterations_ (iterations)
  , sleep_time_ (sleep_time)
  , timeout_ (timeout)
  , successful_calls_ (0)
  , timed_out_calls_ (0)
  , too_big_difference_ (0)
  , task_ (task)
{
}

Client_Task::~Client_Task ()
{
  TAOX11_TEST_DEBUG << LOG_PREFIX << "Client_Task::~Client_Task";
}

int
Client_Task::successful_calls () const
{
  return this->successful_calls_;
}

int
Client_Task::timed_out_calls () const
{
  return this->timed_out_calls_;
}

int
Client_Task::too_big_difference_calls () const
{
  return this->too_big_difference_;
}

int
Client_Task::svc ()
{

  int successful_calls = 0;
  int timed_out_calls = 0;

  TAOX11_TEST_DEBUG << LOG_PREFIX << "Client_Task::svc - starting" << std::endl;

  try
    {
      this->validate_connection ();

      IDL::traits<CORBA::Object>::ref_type object =
          this->orb_->resolve_initial_references ("PolicyCurrent");

      if (!object)
      {
        TAOX11_TEST_ERROR << "Client_Task::svc - ERROR: Unable to retrieve PolicyCurrent." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::PolicyCurrent>::ref_type policy_current =
        IDL::traits<CORBA::PolicyCurrent>::narrow (object);

      TimeBase::TimeT timeout_period = 10 * this->timeout_; // ms -> 100nanusec

      CORBA::Any timeout_as_any;
      timeout_as_any <<= timeout_period;

      CORBA::PolicyList policy_list (1);
      policy_list[0] =
        this->orb_->create_policy (Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE,
                                  timeout_as_any);

      // narrow from generic policy ref
      IDL::traits<Messaging::RelativeRoundtripTimeoutPolicy>::ref_type rrt_pol =
          IDL::traits<Messaging::RelativeRoundtripTimeoutPolicy>::narrow (policy_list[0]);

      if (!rrt_pol)
      {
        TAOX11_TEST_ERROR << "Unable to narrow Messaging::RelativeRoundtripTimeoutPolicy"
            << std::endl;
        return 1;
      }

      if (rrt_pol->relative_expiry () != timeout_period)
      {
        TAOX11_TEST_ERROR << "Incorrect Messaging::RelativeRoundtripTimeoutPolicy relative_expiry returned"
            << std::endl;
        return 1;
      }

      // implicitly widen to object ref
      IDL::traits<CORBA::Object>::ref_type polobj = rrt_pol;

      // (re-)narrow
      rrt_pol = IDL::traits<Messaging::RelativeRoundtripTimeoutPolicy>::narrow (polobj);

      if (!rrt_pol)
      {
        TAOX11_TEST_ERROR << "Unable to narrow Messaging::RelativeRoundtripTimeoutPolicy from CORBA::Object reference"
            << std::endl;
        return 1;
      }

      if (policy_current)
      {
        policy_current->set_policy_overrides (policy_list,
                                              CORBA::SetOverrideType::ADD_OVERRIDE);
      }
      else
      {
        TAOX11_TEST_ERROR << "Client_Task::svc - ERROR: Current policy seems nil." << std::endl;
        return 1;
      }

      // Check whether the policy has been set
      // policy_current exists
      CORBA::PolicyTypeSeq ts;
      CORBA::PolicyList pl = policy_current->get_policy_overrides (ts);
      // Should return all
      if (pl.size () != 1)
      {
        TAOX11_TEST_ERROR << "Client_Task::svc - ERROR: Unexpected number of policies found:"
          << "expected <1> - found <" << pl.size () << ">" << std::endl;
      }
      else if (pl[0]->policy_type () != Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE)
      {
        TAOX11_TEST_ERROR << "Client_Task::svc - ERROR: Unexpected policy type found:"
          << "expected <" << Messaging::RELATIVE_RT_TIMEOUT_POLICY_TYPE
          << "> - found <" << pl[0]->policy_type () << ">" << std::endl;
      }

      // Add a policy type which is not available in the set policies
      ts.push_back (Messaging::QUEUE_ORDER_POLICY_TYPE);
      pl = policy_current->get_policy_overrides (ts);
      // Should return 0
      if (pl.size () != 0)
      {
        TAOX11_TEST_ERROR << "Client_Task::svc - ERROR: Unexpected number of policies found:"
          << "expected <0> - found <" << pl.size () << ">" << std::endl;
      }

      for (int i = 0; i != this->iterations_; ++i)
        {
          int retval = this->one_iteration ();

          if (retval == 1)
            ++successful_calls;
          else if (retval == 0)
            ++timed_out_calls;
        }

      {
        std::lock_guard<std::mutex> __guard (this->mutex_);
        this->successful_calls_ += successful_calls;
        this->timed_out_calls_  += timed_out_calls;
      }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << LOG_PREFIX << "Client_Task::svc - "
            << "exception caught in task <" << this->task_
            << "> : " << ex << std::endl;
      return 1;
    }

  TAOX11_TEST_DEBUG << LOG_PREFIX << "Client_Task::svc - finished" << std::endl;

  return 0;
}

void
Client_Task::validate_connection ()
{
  try
    {
      for (int i = 0; i != 100; ++i)
        {
          this->sleep_service_->go_to_sleep (0);
        }
    }
  catch (const CORBA::TRANSIENT& )
    {
      // Ignore transient exceptions
      TAOX11_TEST_DEBUG << "Transient caught in task <"
            << this->task_ << ">" << std::endl;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Client_Task::validate_connection - "
            << "exception caught in task <" << this->task_
            << ">: " << ex << std::endl;
    }
}

int
Client_Task::one_iteration ()
{
  try
    {
      ACE_Time_Value start = ACE_OS::gettimeofday ();

      this->sleep_service_->go_to_sleep (this->sleep_time_);

      ACE_Time_Value end = ACE_OS::gettimeofday ();

      if (this->timeout_ > 0)
        {
          unsigned long max_milliseconds = this->timeout_ / 1000;

          ACE_Time_Value elapsed = end - start;
          const unsigned long elapsed_milliseconds = elapsed.msec ();

          if (elapsed_milliseconds > max_milliseconds)
            {
              const unsigned long difference =
                elapsed_milliseconds - max_milliseconds;

              // If the different is more than 10 milliseconds we are
              // *way* off, this is an error.
              if (difference > 10)
                {
                  this->too_big_difference_++;
                  TAOX11_TEST_WARNING << "("
                        << x11_logger::processid << "|"
                        << x11_logger::threadid << ") task <"
                        << this->task_ << ">: Elapsed time <"
                        << elapsed_milliseconds << "> expected <"
                        << max_milliseconds << ">"
                        << std::endl;
                }
            }
        }

      return 1;
    }
  catch (const CORBA::TIMEOUT& )
    {
      if (this->sleep_time_ > this->timeout_)
        {
          // If we don't sleep then the next request will also
          // timeout:
          // The server is already sleeping, the client sends a -new-
          // request, but the server cannot process it until the
          // previous sleep completes. The sleep times for multiple
          // requests accumulate, and the client always fails.
          std::this_thread::sleep_for (std::chrono::microseconds (this->sleep_time_ - this->timeout_));
        }

      // Ignore this exception, it is usually caused by a transient
      // condition
      return 0;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Client_Task::one_iteration - "
            << "exception caught in task <" << this->task_
            << ">: " << ex << std::endl;
    }

  return 1;
}
