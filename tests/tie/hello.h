/**
 * @file    hello.h
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "tao/x11/orb.h"

/// Implement the Test::Hello interface
class Hello_impl final
{
public:
  /// Constructor
  Hello_impl (IDL::traits<CORBA::ORB>::ref_type orb);
  virtual ~Hello_impl ();

  std::string get_string ();
  std::string get_string2 ();

  void shutdown ();

  char readonly_char ();
  char at_char ();
  void at_char (char _v);

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
};

#endif /* HELLO_H */
