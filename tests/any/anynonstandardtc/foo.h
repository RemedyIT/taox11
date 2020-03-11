/**
 * @file    foo.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "tao/x11/anytypecode/any.h"
#include "testS.h"

/// Implement the Test::Hello interface
class Foo
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods
  bool pass_data (const CORBA::Any & s) override;

  CORBA::Any return_data (bool str) override;

  bool get_data (CORBA::Any & s, bool str) override;

  bool update_struct (CORBA::Any & s) override;

  bool update_union (CORBA::Any & s)override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  Test::Simple s_;
  Test::Data d_;
};

#endif /* FOO_H */
