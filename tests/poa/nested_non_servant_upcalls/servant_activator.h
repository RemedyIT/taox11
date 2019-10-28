/**
 * @file    servant_activator.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 POA forwarding test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef SERVANT_ACTIVATOR_H
#define SERVANT_ACTIVATOR_H

#include "tao/x11/portable_server/ServantActivatorC.h"
#include "ace/Task.h"
#include "ace/Auto_Event.h"
#include "testS.h"

class Object_Activator : public ACE_Task_Base
{
public:
  Object_Activator(ACE_Thread_Manager &thread_manager,
      IDL::traits<PortableServer::POA>::ref_type poa);

  int
  svc();

  ACE_Auto_Event object_activated_;

  IDL::traits<PortableServer::POA>::ref_type poa_;
};

class Servant_Activator : public IDL::traits<PortableServer::ServantActivator>::base_type
{
public:
  Servant_Activator(IDL::traits<PortableServer::POA>::ref_type poa);

  CORBA::servant_traits<PortableServer::Servant>::ref_type
  incarnate(const PortableServer::ObjectId &oid,
      IDL::traits<PortableServer::POA>::ref_type poa);

  void
  etherealize(const PortableServer::ObjectId &oid,
      IDL::traits<PortableServer::POA>::ref_type adapter,
      CORBA::servant_traits<PortableServer::Servant>::ref_type servant,
      bool cleanup_in_progress, bool remaining_activations);

  IDL::traits<PortableServer::POA>::ref_type poa_;

  PortableServer::ObjectId id_;
};

class test_i : public CORBA::servant_traits<test>::base_type
{
public:
  test_i(IDL::traits<PortableServer::POA>::ref_type poa);
  ~test_i();

  void
  method();

protected:
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* SERVANT_ACTIVATOR_H */
