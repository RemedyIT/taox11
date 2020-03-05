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

class iA_impl final
  : public virtual CORBA::servant_traits<Test::iA>::base_type
{
public:
  iA_impl () = default;
  ~iA_impl () = default;

  std::string do_something_A() override;

private:
  iA_impl (const iA_impl&) = delete;
  iA_impl (iA_impl&&) = delete;
  iA_impl& operator= (const iA_impl&) = delete;
  iA_impl& operator= (iA_impl&&) = delete;
};

class iB_impl final
  : public virtual CORBA::servant_traits<Test::iB>::base_type
{
public:
  iB_impl () = default;
  ~iB_impl () = default;

  std::string do_something_A() override;
  std::string do_something_B() override;
private:
  iB_impl (const iB_impl&) = delete;
  iB_impl (iB_impl&&) = delete;
  iB_impl& operator= (const iB_impl&) = delete;
  iB_impl& operator= (iB_impl&&) = delete;
};

class iC_impl final
  : public virtual CORBA::servant_traits<Test::iC>::base_type
{
public:
  iC_impl () = default;
  ~iC_impl () = default;

  std::string do_something_A() override;
  std::string do_something_B() override;
  std::string do_something_C() override;
private:
  iC_impl (const iC_impl&) = delete;
  iC_impl (iC_impl&&) = delete;
  iC_impl& operator= (const iC_impl&) = delete;
  iC_impl& operator= (iC_impl&&) = delete;
};

class iD_impl final
  : public virtual CORBA::servant_traits<Test::iD>::base_type
{
public:
  iD_impl () = default;
  ~iD_impl () = default;

  std::string do_something_D() override;
private:
  iD_impl (const iD_impl&) = delete;
  iD_impl (iD_impl&&) = delete;
  iD_impl& operator= (const iD_impl&) = delete;
  iD_impl& operator= (iD_impl&&) = delete;
};

class iE_impl final
  : public virtual CORBA::servant_traits<Test::iE>::base_type
{
public:
  iE_impl () = default;
  ~iE_impl () = default;

  std::string do_something_A() override;
  std::string do_something_B() override;
  std::string do_something_C() override;
  std::string do_something_D() override;
  std::string do_something_E() override;
private:
  iE_impl (const iE_impl&) = delete;
  iE_impl (iE_impl&&) = delete;
  iE_impl& operator= (const iE_impl&) = delete;
  iE_impl& operator= (iE_impl&&) = delete;
};

/// Implement the Test::Hello interface
class Hello final
  : public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
    IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  IDL::traits<Test::iA>::ref_type get_iA() override;
  IDL::traits<Test::iB>::ref_type get_iB() override;
  IDL::traits<Test::iC>::ref_type get_iC() override;
  IDL::traits<Test::iD>::ref_type get_iD() override;
  IDL::traits<Test::iE>::ref_type get_iE() override;

  void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;

  Hello (const Hello&) = delete;
  Hello (Hello&&) = delete;
  Hello& operator= (const Hello&) = delete;
  Hello& operator= (Hello&&) = delete;
};

#endif /* HELLO_H */
