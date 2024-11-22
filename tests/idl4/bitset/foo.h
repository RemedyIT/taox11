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
  MyBitset1 test_mybitset1 (const MyBitset1 & sin, MyBitset1 & sinout, MyBitset1 & sout) override;
  MyBitset2 test_mybitset2 (const MyBitset2 & sin, MyBitset2 & sinout, MyBitset2 & sout) override;
  MyBitset3 test_mybitset3 (const MyBitset3 & sin, MyBitset3 & sinout, MyBitset3 & sout) override;
  MyBitset4 test_mybitset4 (const MyBitset4 & sin, MyBitset4 & sinout, MyBitset4 & sout) override;

  void shutdown () override;

private:
  /// Use an ORB reference shutdown the server.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  /// Use a POA reference to activate the references to
  // the template module interface.
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* FOO_H */
