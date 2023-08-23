/**
 * @file    foo.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"

#include "testlib/taox11_testlog.h"

FooImpl::FooImpl (IDL::traits<CORBA::ORB>::ref_type orb,
  IDL::traits<PortableServer::POA>::ref_type poa)
  : orb_ (std::move(orb))
  , poa_ (std::move(poa))
{
}

Point2D
FooImpl::test_Point2D (const Point2D & sin,
  Point2D & sinout,
  Point2D & sout)
{
  sout = sin;
  sinout = sin;
  Point2D sret = sin;
  return sret;
}

Point3D
FooImpl::test_Point3D (const Point3D & sin,
  Point3D & sinout,
  Point3D & sout)
{
  sout = sin;
  sinout = sin;
  Point3D sret = sin;
  return sret;
}

void
FooImpl::shutdown ()
{
  this->orb_->shutdown (false);
}
