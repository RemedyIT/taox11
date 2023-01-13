/**
 * @file    hello.h
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

/// Implement the Test::Hello interface
class Hello_Factory
  : public virtual CORBA::servant_traits<Test::Hello_Factory>::base_type
{
public:
  /// Constructor
  Hello_Factory (IDL::traits<CORBA::ORB>::ref_type orb,
                 IDL::traits<PortableServer::POA>::ref_type poa);
  ~Hello_Factory () override = default;

  // = The skeleton methods
  IDL::traits<Test::Hello>::ref_type get_hello () override;

  void get_hello_2 (IDL::traits<Test::Hello>::ref_type& hello_obj) override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

/// Implement the Test::Hello interface
class Hello
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello (IDL::traits<CORBA::ORB>::ref_type orb, IDL::traits<Test::Hello_Factory>::ref_type factory);
  ~Hello () override = default;

  // = The skeleton methods
  std::string get_string () override;
  IDL::traits<Test::Hello_Factory>::ref_type get_factory () override;
  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<Test::Hello_Factory>::ref_type factory_;
};

#endif /* HELLO_H */
