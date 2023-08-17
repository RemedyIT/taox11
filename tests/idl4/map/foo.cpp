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

Test::StringLongMap
Foo::test_string_long_map (const Test::StringLongMap & sin,
  Test::StringLongMap & sinout,
  Test::StringLongMap & sout)
{
  sout = sin;
  sinout = sin;
  Test::StringLongMap sret = sin;
  return sret;
}

Test::StringLongMap5
Foo::test_string_long_map5 (const Test::StringLongMap5 & sin,
  Test::StringLongMap5 & sinout,
  Test::StringLongMap5 & sout)
{
  sout = sin;
  sinout = sin;
  Test::StringLongMap5 sret = sin;
  return sret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
