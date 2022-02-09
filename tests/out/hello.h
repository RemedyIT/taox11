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
class Hello_Factory final
  : public virtual CORBA::servant_traits<Test::Hello_Factory>::base_type
{
public:
  /// Constructor
  Hello_Factory (IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);
  ~Hello_Factory () override = default;

  // = The skeleton methods
  void get_hello (const std::string& log_string,
      IDL::traits<Test::Hello>::ref_type& hello_obj,
      IDL::traits<Test::Hello2>::ref_type& hello2_obj) override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;

  Hello_Factory (const Hello_Factory&) = delete;
  Hello_Factory (Hello_Factory&&) = delete;
  Hello_Factory& operator= (const Hello_Factory&) = delete;
  Hello_Factory& operator= (Hello_Factory&&) = delete;
};

class StringInterface final
  : public virtual CORBA::servant_traits<Test::StringInterface>::base_type
{
public:
  StringInterface () = default;
  ~StringInterface () override = default;
private:
  StringInterface (const StringInterface&) = delete;
  StringInterface (StringInterface&&) = delete;
  StringInterface& operator= (const StringInterface&) = delete;
  StringInterface& operator= (StringInterface&&) = delete;
};

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  explicit Hello (IDL::traits<CORBA::ORB>::ref_type orb);
  ~Hello () override = default;

  // = The skeleton methods
  std::string get_string () override;

  void shutdown () override;

private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

/// Implement the Test::Hello2 interface
class Hello2 final
  : public virtual CORBA::servant_traits<Test::Hello2>::base_type
{
public:
  /// Constructor
  Hello2 (IDL::traits<PortableServer::POA>::ref_type poa);
  virtual ~Hello2 ();

  // = The skeleton methods
  void get_string (IDL::traits<Test::StringInterface>::ref_type& si) override;

private:
  IDL::traits<PortableServer::POA>::ref_type poa_;

  Hello2 (const Hello2&) = delete;
  Hello2 (Hello2&&) = delete;
  Hello2& operator= (const Hello2&) = delete;
  Hello2& operator= (Hello2&&) = delete;
};

#endif /* HELLO_H */
