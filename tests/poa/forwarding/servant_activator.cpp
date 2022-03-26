/**
 * @file    servant_activator.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA forwarding test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "servant_activator.h"
#include "tao/x11/portable_server/ForwardRequestC.h"
#include "test_i.h"

#include "testlib/taox11_testlog.h"

ServantActivator::ServantActivator (
  IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<CORBA::Object>::ref_type forward_to)
    : orb_ (std::move(orb))
    , forward_to_ (forward_to)
{
}

ServantActivator::~ServantActivator()
{
  TAOX11_TEST_DEBUG << "ServantActivator::~ServantActivator" << std::endl;
}

CORBA::servant_reference<PortableServer::Servant>
ServantActivator::incarnate (const PortableServer::ObjectId& /*oid*/,
  IDL::traits<PortableServer::POA>::ref_type /*adapter*/)
{
  this->orb_->shutdown (false);

  if (!this->forward_to_)
  {
    TAOX11_TEST_ERROR << "Unable to forward requests" << std::endl;
    throw test::Cannot_Forward ();
  }

  TAOX11_TEST_DEBUG << "ServantActivator::incarnate : "
               << "Throwing ForwardRequest exception." << std::endl;

  // Throw forward exception
  throw PortableServer::ForwardRequest (this->forward_to_);
}

void
ServantActivator::etherealize (const PortableServer::ObjectId& ,
  IDL::traits<PortableServer::POA>::ref_type ,
  CORBA::servant_reference<PortableServer::Servant> ,
  bool ,
  bool )
{
  TAOX11_TEST_DEBUG << "ServantActivator::etherealize" << std::endl;
}
