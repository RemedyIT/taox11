/**
 * @file    foo.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"

#include "testlib/taox11_testlog.h"

Foo::Foo (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<PortableServer::POA>::ref_type poa)
  : orb_ (std::move(orb))
  , poa_ (std::move(poa))
{
}

bar
Foo::test_bar (const bar & sin, bar & sinout, bar & sout)
{
  sout = sin;
  sinout = sin;
  bar sret = sin;
  sret.z(6);
  return sret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
