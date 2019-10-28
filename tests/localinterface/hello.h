/**
 * @file    hello.h
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef HELLO_H
#define HELLO_H

#include "testS.h"
#include "locala.h"

class iLocal_impl final :
    public virtual IDL::traits<Test::iLocal>::base_type
{
public:
  virtual std::string do_something() override;
};

class iA_impl final :
    public virtual CORBA::servant_traits<Test::iA>::base_type
{
  virtual std::string do_A(int32_t& result) override;
};


/// Implement the Test::Hello interface
class Hello final :
    public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb, IDL::traits<PortableServer::POA>::ref_type poa, int& result);

  virtual IDL::traits<Test::iA>::ref_type get_iA() override;
  virtual IDL::traits<Test::iA>::ref_type get_localnull() override;
  virtual IDL::traits<Test::iA>::ref_type get_local() override;
  virtual IDL::traits<Test::iA>::ref_type inout_local(
      IDL::traits<Test::iA>::ref_type in_v,
      IDL::traits<Test::iA>::ref_type& out_v,
      IDL::traits<Test::iA>::ref_type& inout_v) override;
  virtual IDL::traits<Test::iA>::ref_type attrIa() override;
  virtual void attrIa(IDL::traits<Test::iA>::ref_type _v) override;

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
  int &result_;
  IDL::traits<Test::iLocal_A>::ref_type m_lia;
};

#endif /* HELLO_H */
