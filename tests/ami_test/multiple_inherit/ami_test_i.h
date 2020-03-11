/**
 * @file    ami_test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_AMI_TEST_I_H
#define TAOX11_AMI_TEST_I_H

#include "testlib/taox11_testlog.h"

#include "ami_testS.h"

class A_i
: public virtual CORBA::servant_traits< Test::A>::base_type
{
public:
  // The AMI_Test methods.
  int32_t op_A (int32_t) override;
};

class B_i
: public virtual CORBA::servant_traits< Test::B>::base_type
{
public:
  // The AMI_Test methods.
  int32_t op_A (int32_t) override;
  int32_t op_B (int32_t) override;
};

class C_i
: public virtual CORBA::servant_traits< Test::C>::base_type
{
public:
  // The AMI_Test methods.
  int32_t op_A (int32_t) override;
  int32_t op_C (int32_t) override;
};

class D_i
: public virtual CORBA::servant_traits< Test::D>::base_type
{
public:
  // The AMI_Test methods.
  int32_t op_A (int32_t) override;
  int32_t op_B (int32_t) override;
  int32_t op_C (int32_t) override;
  int32_t op_D (int32_t) override;
};

/// Implement the Test::Hello interface
class Hello:
    public virtual CORBA::servant_traits<Test::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  IDL::traits<Test::A>::ref_type get_iA() override;
  IDL::traits<Test::B>::ref_type get_iB() override;
  IDL::traits<Test::C>::ref_type get_iC() override;
  IDL::traits<Test::D>::ref_type get_iD() override;

  void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* TAOX11_AMI_TEST_I_H */
