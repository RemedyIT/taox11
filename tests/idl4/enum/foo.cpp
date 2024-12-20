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

MyEnum
Foo::test_myenum (MyEnum sin,
  MyEnum & sinout,
  MyEnum & sout)
{
  if (sin != MyEnum::flag0)
    {
      TAOX11_TEST_ERROR << "ERROR: test_myenum: Received incorrect sin<" << sin << ">" << std::endl;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_myenum: Received sin<" << sin << ">" << std::endl;
    }
  if (sinout != MyEnum::flag1)
    {
      TAOX11_TEST_ERROR << "ERROR: test_myenum: Received incorrect sinout<" << sinout << ">" << std::endl;
    }
  else
    {
      TAOX11_TEST_DEBUG << "test_myenum: Received sinout<" << sinout << ">" << std::endl;
    }
  sout = sin;
  if (sout != MyEnum::flag0)
    {
      TAOX11_TEST_ERROR << "ERROR: test_myenum: Setting sout failed <" << sout << ">" << std::endl;
    }
  sinout = sin;
  return MyEnum::flag2;
}

MyEnumBound8
Foo::test_myenumbound8 (MyEnumBound8 sin,
  MyEnumBound8 & sinout,
  MyEnumBound8 & sout)
{
  if (sin != MyEnumBound8::flag8_2)
    {
      TAOX11_TEST_ERROR << "ERROR: MyEnumBound8: Received incorrect sin<" << sin << ">" << std::endl;
    }
  sout = sin;
  sinout = sin;
  MyEnumBound8 sret = sin;
  return sret;
}

MyEnumBound16
Foo::test_myenumbound16 (MyEnumBound16 sin,
  MyEnumBound16 & sinout,
  MyEnumBound16 & sout)
{
  if (sin != MyEnumBound16::flag16_2)
    {
      TAOX11_TEST_ERROR << "ERROR: MyEnumBound16: Received incorrect sin<" << sin << ">" << std::endl;
    }
  sout = sin;
  sinout = sin;
  MyEnumBound16 sret = sin;
  return sret;
}

MyEnumBound32
Foo::test_myenumbound32 (MyEnumBound32 sin,
  MyEnumBound32 & sinout,
  MyEnumBound32 & sout)
{
  if (sin != MyEnumBound32::flag32_2)
    {
      TAOX11_TEST_ERROR << "ERROR: MyEnumBound32: Received incorrect sin<" << sin << ">" << std::endl;
    }
  sout = sin;
  sinout = sin;
  MyEnumBound32 sret = sin;
  return sret;
}

MyValueEnum
Foo::test_myvalueenum (MyValueEnum sin,
  MyValueEnum & sinout,
  MyValueEnum & sout)
{
  if (sin != MyValueEnum::myv4)
    {
      TAOX11_TEST_ERROR << "ERROR: MyValueEnum: Received incorrect sin<" << sin << ">" << std::endl;
    }
  sout = sin;
  sinout = sin;
  MyValueEnum sret = sin;
  return sret;
}

void
Foo::shutdown ()
{
  this->orb_->shutdown (false);
}
