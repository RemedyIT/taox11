/**
 * @file    client_task.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef MT_TIMEOUT_CLIENT_TASK_H
#define MT_TIMEOUT_CLIENT_TASK_H

#pragma once

#include "testC.h"
#include "ace/Task.h"
#include <mutex>

class Client_Task : public ACE_Task_Base
{
public:
  Client_Task (IDL::traits<CORBA::ORB>::ref_type orb,
               IDL::traits<Test::Sleep_Service>::ref_type sleep_service,
               int iterations,
               uint32_t sleep_time,
               uint32_t timeout,
               int task);
  ~Client_Task ();

  int successful_calls () const;

  int timed_out_calls () const;

  int too_big_difference_calls () const;

  int svc ();

private:
  int one_iteration ();

  void validate_connection ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;

  IDL::traits<Test::Sleep_Service>::ref_type sleep_service_;

  int iterations_;

  uint32_t sleep_time_;

  uint32_t timeout_;

  int successful_calls_;

  int timed_out_calls_;

  int too_big_difference_;

  std::mutex mutex_;

  int task_;
};

#endif /* MT_TIMEOUT_CLIENT_TASK_H */
