/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"

class iA_impl final
  : public virtual CORBA::servant_traits<Test::iA>::base_type
{
public:
  iA_impl () = default;
  ~iA_impl () = default;

  virtual std::string do_something_A();
private:
  iA_impl (const iA_impl&) = delete;
  iA_impl (iA_impl&&) = delete;
  iA_impl& operator= (const iA_impl&) = delete;
  iA_impl& operator= (iA_impl&&) = delete;
};

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa,
      int& result);

  virtual IDL::traits<Test::iA>::ref_type get_iA() override;
  virtual IDL::traits<Test::iA>::ref_type get_null() override;
  virtual IDL::traits<Test::iA>::ref_type inout_null(
      IDL::traits<Test::iA>::ref_type in_v,
      IDL::traits<Test::iA>::ref_type& out_v,
      IDL::traits<Test::iA>::ref_type& inout_v) override;

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
  int &result_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
