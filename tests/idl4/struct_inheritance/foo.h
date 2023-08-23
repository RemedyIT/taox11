/**
 * @file    foo.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

class FooImpl final
  : public virtual CORBA::servant_traits<Foo>::base_type
{
public:
  /// Constructor
  FooImpl (IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  Point2D
  test_Point2D (const Point2D & sin, Point2D & sinout,
    Point2D & sout) override;

  Point3D
  test_Point3D (const Point3D & sin, Point3D & sinout,
    Point3D & sout) override;

  void shutdown () override;

private:
  /// Use an ORB reference shutdown the server.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  /// Use a POA reference to activate the references to
  // the template module interface.
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* FOO_H */
