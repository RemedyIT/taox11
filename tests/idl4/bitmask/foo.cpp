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
  if (!(sin & MyBitMask::flag0))
    {
      TAOX11_TEST_ERROR << "ERROR: test_mybitmask: Received incorrect sin<" << sin << ">" << std::endl;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_mybitmask: Received sin<" << sin << ">" << std::endl;
    }
  if (!(sinout & MyBitMask::flag1))
    {
      TAOX11_TEST_ERROR << "ERROR: test_mybitmask: Received incorrect sinout<" << sinout << ">" << std::endl;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_mybitmask: Received sinout<" << sinout << ">" << std::endl;
    }
  sout = sin;
  if (!(sout & MyBitMask::flag0))
    {
      TAOX11_TEST_ERROR << "ERROR: test_mybitmask: Setting sout failed <" << sout << ">" << std::endl;
    }
  sinout = sin;
  return MyBitMask::flag2;
}

MyBitMaskBound8
Foo::test_mybitmaskbound8 (const MyBitMaskBound8 & sin,
  MyBitMaskBound8 & sinout,
  MyBitMaskBound8 & sout)
{
  if (!(sin & MyBitMaskBound8::flag8_2))
    {
      TAOX11_TEST_ERROR << "ERROR: MyBitMaskBound8: Received incorrect sin<" << sin << ">" << std::endl;
    }
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
