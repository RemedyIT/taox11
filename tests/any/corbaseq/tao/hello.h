/**
 * @file    hello.h
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef HELLO_H
#define HELLO_H
#include /**/ "ace/pre.h"

#include "testS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual POA_Test::Hello
{
public:
  /// Constructor
  Hello (CORBA::ORB_ptr orb);
  virtual ~Hello ();

  // = The skeleton methods
  ::CORBA::Any *
  test (const CORBA::Any& input) override;

  ::CORBA::Any *
  test_wcharseq (const ::CORBA::Any & input) override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::ORB_var orb_;
};

#include /**/ "ace/post.h"
#endif /* HELLO_H */
