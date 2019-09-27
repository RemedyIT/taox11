/**
 * @file    client_task.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "client_task.h"

#include "testlib/taox11_testlog.h"

Client_Task::Client_Task (IDL::traits<CORBA::ORB>::ref_type orb,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr)
  , orb_ (std::move(orb))
 {
}

int
Client_Task::svc ()
{
  TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
        << "Starting client task" << std::endl;

  try
  {
    std::chrono::seconds tv (60);
    this->orb_->run (tv);
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << X11_LOG_THREAD_INFO
          << "Client_Task::svc - Exception caught - "
          << ex << std::endl;
    return 1;
  }
  TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
        << "Client task has finished." << std::endl;
  return 0;
}
