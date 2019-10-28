/**
 * @file    servant_activator.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 POA forwarding test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "servant_activator.h"
#include "testlib/taox11_testlog.h"
#include "tao/x11/portable_server/ServantActivatorC.h"

extern Object_Activator *global_object_activator;


test_i::test_i(IDL::traits<PortableServer::POA>::ref_type poa) :
    poa_ (poa)
{
  TAOX11_TEST_DEBUG << "test_i created: instance " << std::endl;
}

test_i::~test_i()
{
  TAOX11_TEST_DEBUG << "test_i destroyed: instance " << std::endl;
}

void
test_i::method()
{
}

Object_Activator::Object_Activator(ACE_Thread_Manager &thread_manager,
    IDL::traits<PortableServer::POA>::ref_type poa) :
    ACE_Task_Base (std::addressof(thread_manager)), poa_ (poa)
{
}

int
Object_Activator::svc()
{
  try
  {
    CORBA::servant_reference<test_i> servant = CORBA::make_reference<test_i> (poa_);
    PortableServer::ObjectId id = poa_->activate_object (servant);
    this->object_activated_.signal ();
    poa_->deactivate_object (id);
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught in activator thread: " << ex
        << std::endl;
    return 1;
  }
  return 0;
}

Servant_Activator::Servant_Activator(
    IDL::traits<PortableServer::POA>::ref_type poa) :
    poa_ (std::move(poa))
{
}

CORBA::servant_traits<PortableServer::Servant>::ref_type
Servant_Activator::incarnate(const PortableServer::ObjectId &,
    IDL::traits<PortableServer::POA>::ref_type)
{
  CORBA::servant_reference<test_i> servant = CORBA::make_reference<test_i> (
      poa_);
  this->id_ = poa_->activate_object (servant);

  poa_->deactivate_object (this->id_);

  int result = global_object_activator->activate ();
  if (result == -1)
  {
    TAOX11_TEST_ERROR << "global_object_activator->activate ()" << std::endl;
    throw CORBA::OBJECT_NOT_EXIST();
  }
  ACE_Time_Value timeout (5, 0);

  result = global_object_activator->object_activated_.wait (std::addressof(timeout), 0);
  if (result == 0)
  {
    TAOX11_TEST_ERROR << "Object Activator was able to make progress during "
        "recursive non-servant upcall: test failed" << std::endl;
    throw CORBA::OBJECT_NOT_EXIST();
  }
  else if (result == -1 && errno == ETIME)
  {
    TAOX11_TEST_DEBUG << "Object Activator was not able to make progress during "
        "recursive non-servant upcall: test succeeded\n" << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Unexpected error during event.wait (): %d\n" << result
        << std::endl;
    throw CORBA::OBJECT_NOT_EXIST();
  }
  return CORBA::make_reference<test_i> (poa_);
}

void
Servant_Activator::etherealize(const PortableServer::ObjectId &,
    IDL::traits<PortableServer::POA>::ref_type,
    CORBA::servant_traits<PortableServer::Servant>::ref_type, bool, bool)
{
}
