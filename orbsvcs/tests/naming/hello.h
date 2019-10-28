/* -------------------------------------------------------------------
 * @file    hello.h
 * @author  Marcel Smit
 *
 * @brief   TAOX11 Naming service test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * -------------------------------------------------------------------*/
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello (IDL::traits<CORBA::ORB>::ref_type orb);

  // = The skeleton methods
  virtual std::string get_string ();

  virtual void shutdown ();

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  CORBA::object_reference<CORBA::ORB> orb_;
};

#endif /* HELLO_H */
