/**
 * @file    foo.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  MyEnum test_myenum (MyEnum sin, MyEnum & sinout, MyEnum & sout) override;
  MyEnumBound8 test_myenumbound8 (MyEnumBound8 sin, MyEnumBound8 & sinout, MyEnumBound8 & sout) override;
  MyEnumBound16 test_myenumbound16 (MyEnumBound16 sin, MyEnumBound16 & sinout, MyEnumBound16 & sout) override;
  MyEnumBound32 test_myenumbound32 (MyEnumBound32 sin, MyEnumBound32 & sinout, MyEnumBound32 & sout) override;
  MyValueEnum test_myvalueenum (const MyValueEnum sin, MyValueEnum & sinout, MyValueEnum & sout) override;

  void shutdown () override;

private:
  /// Use an ORB reference shutdown the server.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  /// Use a POA reference to activate the references to
  // the template module interface.
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* FOO_H */
