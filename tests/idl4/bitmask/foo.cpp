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

MyBitMask
Foo::test_mybitmask (const MyBitMask & sin,
  MyBitMask & sinout,
  MyBitMask & sout)
{
  sout = sin;
  sinout = sin;
  MyBitMask sret = sin;
  return sret;
}

MyBitMaskBound8
Foo::test_mybitmaskbound8 (const MyBitMaskBound8 & sin,
  MyBitMaskBound8 & sinout,
  MyBitMaskBound8 & sout)
{
  sout = sin;
  sinout = sin;
  MyBitMaskBound8 sret = sin;
  return sret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
