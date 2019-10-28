/**
 * @file    hello.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "testlib/taox11_testlog.h"
#include "hello.h"

Hello::Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa) :
    orb_(orb), poa_(poa)
{
}

Hello::~Hello()
{
}

IDL::traits<PortableServer::POA>::ref_type
Hello::_default_POA()
{
  return this->poa_;
}

void
Hello::shutdown()
{
  this->orb_->shutdown(false);
}
