/**
 * @file    server_task.h
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_MT_BIDIR_SERVER_TASK_H
#define TAOX11_MT_BIDIR_SERVER_TASK_H

#pragma once

#include "ace/Task.h"
#include "tao/x11/orb.h"
#include "sender_i.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Manual_Event;
ACE_END_VERSIONED_NAMESPACE_DECL

class Server_Task : public ACE_Task_Base
{
public:
  Server_Task (CORBA::servant_reference<Sender_i> s,
               ACE_Manual_Event &event,
               const int no_iterations,
               IDL::traits<CORBA::ORB>::ref_type orb,
               ACE_Thread_Manager *thr_mgr);

  int svc ();

private:
  CORBA::servant_reference<Sender_i> sender_;

  ACE_Manual_Event &event_;

  const int no_iterations_;

  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* TAOX11_MT_BIDIR_SERVER_TASK_H */
