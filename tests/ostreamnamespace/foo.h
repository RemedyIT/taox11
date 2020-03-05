/**
 * @file    foo.h
 * @author  Marijke Hengstmengel
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef FOO_H
#define FOO_H

#include "tao/x11/anytypecode/any.h"
#include "testS.h"

/// Implement the Test::Hello interface
class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods
  bool pass_data (const CORBA::Any & s) override;

  void do_it () override;

  void do_reason () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;

  Foo (const Foo&) = delete;
  Foo (Foo&&) = delete;
  Foo& operator= (const Foo&) = delete;
  Foo& operator= (Foo&&) = delete;
};

#endif /* FOO_H */
