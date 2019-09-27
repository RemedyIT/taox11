/**
 * @file    server_task.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef SERVER_TASK_H
#define SERVER_TASK_H

#pragma once

#include "tao/x11/orb.h"
#include "ace/Task.h"

class Server_Task
  : public ACE_Task_Base
{
public:
  Server_Task (ACE_Thread_Manager *thr_mgr,
               IDL::traits<CORBA::ORB>::ref_type orb,
               const ACE_Time_Value &running_time);

  int svc ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;

  /// Total running time for each thread
  ACE_Time_Value running_time_;
};

#endif /* SERVER_TASK_H */
