/**
 * @file    server_task.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "server_task.h"

#include "ace/Manual_Event.h"

#include "testlib/taox11_testlog.h"

Server_Task::Server_Task (CORBA::servant_reference<Sender_i> sender,
                          ACE_Manual_Event &event,
                          const int no_iterations,
                          IDL::traits<CORBA::ORB>::ref_type orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , sender_ (sender)
  , event_ (event)
  , no_iterations_ (no_iterations)
  , orb_ (std::move(orb))
{
}

int
Server_Task::svc ()
{
  TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
        << "Wait for the Event" << std::endl;

  // Wait on the event
  // Yes, wait on event twice. One for making sure that we have got
  // all the clients  come and register with us.
  this->event_.wait ();

  TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
        << "Start doing some work" << std::endl;

  try
    {
      // Before start
      for (int i = 0; i != this->no_iterations_; ++i)
        {
          if (i % 25 == 0)
          {
            TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
                  << "Sending message " << i << "...." << std::endl;
          }

          this->sender_->send_message ();
        }
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception in <Server_Task::svc>: "
            << ex << std::endl;
      return 1;
    }

  TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
        << "Ready sending " << this->no_iterations_ << " messages" << std::endl;

  return 0;
}
