/**
 * @file    receiver_factory.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "receiver_factory.h"
#include "receiver.h"

Receiver_Factory::Receiver_Factory(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
  orb_(orb), poa_(poa)
{
}

IDL::traits<Test::Receiver>::ref_type Receiver_Factory::create_receiver()
{
  CORBA::servant_traits<Test::Receiver>::ref_type impl = CORBA::make_reference<Receiver_impl> ();
  PortableServer::ObjectId id = this->poa_->activate_object(impl);
  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference(id);
  Test::Receiver::_ref_type ia = IDL::traits<Test::Receiver>::narrow (object);
  return ia;
}

void Receiver_Factory::shutdown()
{
  this->orb_->shutdown(false);
}
