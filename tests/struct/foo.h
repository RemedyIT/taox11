/**
 * @file    foo.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef FOO_H
#define FOO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Foo final
  : public virtual CORBA::servant_traits<Test::Foo>::base_type
{
public:
  /// Constructor
  Foo (IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods
  virtual bool pass_struct (const Test::Simple & s) override;

  virtual Test::Simple return_struct () override;

  virtual bool get_struct (Test::Simple & s) override;

  virtual bool update_struct (Test::Simple & s) override;

  virtual void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  Test::Simple s_;

  Foo (const Foo&) = delete;
  Foo (Foo&&) = delete;
  Foo& operator= (const Foo&) = delete;
  Foo& operator= (Foo&&) = delete;
};

#endif /* FOO_H */
