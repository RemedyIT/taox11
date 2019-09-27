/**
 * @file    server_task.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "server_task.h"

#include "testlib/taox11_testlog.h"

#include "ace/High_Res_Timer.h"

Server_Task::Server_Task (ACE_Thread_Manager *thr_mgr,
                          IDL::traits<CORBA::ORB>::ref_type orb,
                          const ACE_Time_Value &running_time)
  : ACE_Task_Base (thr_mgr)
  , orb_ (std::move(orb))
  , running_time_ (running_time)
{
}

int
Server_Task::svc ()
{
  try
    {
      ACE_hrtime_t start = ACE_OS::gethrtime();
      this->orb_->run (this->running_time_);

      ACE_hrtime_t elapsed_time = ACE_OS::gethrtime() - start;
#if !defined ACE_LACKS_LONGLONG_T
      ACE_UINT32 usecs = ACE_UINT32(elapsed_time /
        ACE_High_Res_Timer::global_scale_factor());
#else  /* ! ACE_LACKS_LONGLONG_T */
      ACE_UINT32 usecs = elapsed_time /
        ACE_High_Res_Timer::global_scale_factor();
#endif /* ! ACE_LACKS_LONGLONG_T */
      TAOX11_TEST_DEBUG << x11_logger::threadid << " Done. Ran for <"
            << usecs << "> usecs" << std::endl;
    }
  catch (const CORBA::Exception&)
    {
      return 1;
    }
  return 0;
}
