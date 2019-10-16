/**
 * @file    client_task.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_MT_BIDIR__CLIENT_TASK_H
#define TAOX11_MT_BIDIR__CLIENT_TASK_H

#pragma once

#include "ace/Task.h"
#include "tao/x11/orb.h"

class Client_Task : public ACE_Task_Base
{
public:
  Client_Task (IDL::traits<CORBA::ORB>::ref_type orb,
               ACE_Thread_Manager *thr_mgr);

  int svc ();

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAOX11_MT_BIDIR__CLIENT_TASK_H */
