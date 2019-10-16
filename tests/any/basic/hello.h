/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef HELLO_H
#define HELLO_H

#include "tao/x11/anytypecode/any.h"
#include "testS.h"

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello (IDL::traits<CORBA::ORB>::ref_type orb, int & result);
  virtual ~Hello ();

  // = The skeleton methods
  virtual CORBA::Any
  TestAny(
     const CORBA::Any &a,
     CORBA::Any &b,
     CORBA::Any &c) override;

  // = The skeleton methods
  virtual CORBA::Any
  TestAny2(
     const CORBA::Any &a,
     CORBA::Any &b,
     CORBA::Any &c,
     const CORBA::Any &d) override;

  virtual CORBA::Any
  TestAny3(
    const CORBA::Any &a,
    CORBA::Any &b,
    CORBA::Any &c) override;

  // = The skeleton methods
  virtual CORBA::Any
  TestAny4(
     const CORBA::Any &a,
     CORBA::Any &b,
     CORBA::Any &c) override;

  virtual CORBA::Any
  TestAnyTypeCodes (
    const CORBA::Any &a,
    CORBA::Any &b) override;

  virtual CORBA::TCKind
  TestAnyTypeCodes2 (CORBA::TCKind a, CORBA::TCKind& b) override;

  virtual IDL::traits< CORBA::TypeCode>::ref_type
  TestAnyTypeCodes3 (IDL::traits< CORBA::TypeCode>::ref_type a,
    IDL::traits< CORBA::TypeCode>::ref_type& b) override;

  virtual Test::Hello::Tc_seq
  TestTypeCodeSeq (const Test::Hello::Tc_seq& a, Test::Hello::Tc_seq& b) override;

  virtual void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;

  int &result_;
};

#endif /* HELLO_H */
