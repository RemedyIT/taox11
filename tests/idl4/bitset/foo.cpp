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

MyBitset1
Foo::test_mybitset1 (const MyBitset1 & sin, MyBitset1 & sinout, MyBitset1 & sout)
{
  sout = sin;
  sinout = sin;
  MyBitset1 sret = sin;
  return sret;
}

MyBitset2
Foo::test_mybitset2 (const MyBitset2 & sin, MyBitset2 & sinout, MyBitset2 & sout)
{
  sout = sin;
  sinout = sin;
  MyBitset2 sret = sin;
  return sret;
}

MyBitset3
Foo::test_mybitset3 (const MyBitset3 & sin, MyBitset3 & sinout, MyBitset3 & sout)
{
  sout = sin;
  sinout = sin;
  MyBitset3 sret = sin;
  return sret;
}

MyBitset4
Foo::test_mybitset4 (const MyBitset4 & sin, MyBitset4 & sinout, MyBitset4 & sout)
{
  sout = sin;
  sinout = sin;
  MyBitset4 sret = sin;
  return sret;
}

SmallBitset
Foo::test_smallbitset (const SmallBitset & sin, SmallBitset & sinout, SmallBitset & sout)
{
  sout = sin;
  sinout = sin;
  SmallBitset sret = sin;
  return sret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
