/**
 * @file    foo.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
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
  virtual bool pass_data (const CORBA::Any & s);

  virtual CORBA::Any return_data (bool str);

  virtual bool get_data (CORBA::Any & s, bool str);

  virtual bool update_struct (CORBA::Any & s);

  virtual bool update_union (CORBA::Any & s);

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  Test::Simple s_;
  Test::Data d_;
};

#endif /* FOO_H */
