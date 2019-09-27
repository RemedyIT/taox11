/**
 * @file    test_i.cpp
 * @author  Marcel Smit
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "test_i.h"
#include "servant_activator.h"
#include "testlib/taox11_testlog.h"

test_i::test_i (IDL::traits<CORBA::ORB>::ref_type orb,
                IDL::traits<PortableServer::POA>::ref_type poa,
                IDL::traits<PortableServer::ServantActivator>::weak_ref_type activator,
                int32_t value)
  : orb_ (orb),
    poa_ (poa),
    activator_ (activator),
    value_ (value)
{
}

test_i::~test_i()
{
  TAOX11_TEST_DEBUG << "test_i::~test_i"
        << std::endl;
}

int32_t
test_i::doit ()
{
  return this->value_++;
}

void
test_i::forward ()
{
  IDL::traits<PortableServer::ServantActivator>::ref_type act =
      this->activator_.lock ();

  PortableServer::ObjectId id = this->poa_->servant_to_id (this->_lock ());
  IDL::traits<CORBA::Object>::ref_type obj_ref = poa_->servant_to_reference (
      this->_lock ());

  this->poa_->deactivate_object(id);
}

void
test_i::shutdown ()
{
  this->orb_->shutdown (false);
}

IDL::traits<PortableServer::POA>::ref_type
test_i::_default_POA ()
{
  return this->poa_;
}
