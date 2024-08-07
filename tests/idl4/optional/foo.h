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
  bar test_bar (const bar & sin, bar & sinout, bar & sout) override;

  void shutdown () override;

private:
  /// Use an ORB reference shutdown the server.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  /// Use a POA reference to activate the references to
  // the template module interface.
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* FOO_H */
