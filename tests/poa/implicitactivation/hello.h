/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello: public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);
  virtual ~Hello();

  /// Returns the default POA for this servant.
  virtual IDL::traits<PortableServer::POA>::ref_type _default_POA()
      override;

  virtual void shutdown() override;

private:
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* HELLO_H */
