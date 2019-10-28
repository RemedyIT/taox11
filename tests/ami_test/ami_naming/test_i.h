/**
 * @file    test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_TEST_I_H
#define TAOX11_TEST_I_H

#include "testlib/taox11_testlog.h"

#include "testS.h"

class Test_i
: public virtual CORBA::servant_traits< A::Test>::base_type
{
public:
  // The Test methods.
  virtual int32_t foo (int32_t in_l) override;
  virtual int32_t sendc_foo (int16_t in_s) override;
  virtual int32_t sendc_ami_foo (bool in_b) override;

  virtual int32_t yadda () override;
  virtual void yadda (int32_t l) override;
  virtual int16_t sendc_get_yadda () override;
  virtual void sendc_get_yadda (int16_t s) override;
  virtual bool sendc_ami_get_yadda () override;
  virtual void sendc_ami_get_yadda (bool b) override;

  virtual int32_t _cxx_do () override;
  virtual void _cxx_do (int32_t l) override;


private:
  int32_t num_l_;
  int16_t num_s_;


};
class Two_i
: public virtual CORBA::servant_traits< A::Two>::base_type
{
public:
   // The Test methods.
  virtual int32_t foo_two (int32_t in_l) override;
  virtual int32_t _cxx_do (int32_t in_l) override;
};
/// Implement the A::Hello interface
class Hello :
    public virtual CORBA::servant_traits<A::Hello>::base_type
{
public:
  /// Constructor
  Hello(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  virtual IDL::traits<A::Test>::ref_type get_iTest() override;
  virtual IDL::traits<A::Two>::ref_type get_iTwo() override;

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* TAOX11_TEST_I_H */
