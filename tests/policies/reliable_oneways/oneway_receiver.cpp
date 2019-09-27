/**
 * @file    oneway_receiver.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Policies test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "oneway_receiver.h"
#include "shutdown_helper.h"

Oneway_Receiver::Oneway_Receiver (
  IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_ (orb)
{
}

void
Oneway_Receiver::raise_no_permission ()
{
  throw CORBA::NO_PERMISSION ();
}

void
Oneway_Receiver::destroy ()
{
  IDL::traits<PortableServer::POA>::ref_type poa =
    this->_default_POA ();

  PortableServer::ObjectId id =
    poa->servant_to_id (this->_lock ());

  poa->deactivate_object (id);
}

IDL::traits<Test::Shutdown_Helper>::ref_type
Oneway_Receiver::get_shutdown_helper ()
{
  CORBA::servant_traits<Test::Shutdown_Helper>::ref_type shutdown_helper =
    CORBA::make_reference<Shutdown_Helper> (this->orb_);

  return shutdown_helper->_this ();
}
