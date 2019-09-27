/**
 * @file    ami_test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAO_AMI_TEST_I_H
#define TAO_AMI_TEST_I_H

#include "testlib/taox11_testlog.h"

#include "ami_testS.h"

class AMI_TEST_SKEL_Export MyFoo_i
: public virtual CORBA::servant_traits< MyFoo>::base_type
{
public:
  /// ctor
  MyFoo_i();

  // The MyFoo methods.
  virtual int32_t foo (int32_t& out_l,
                       int32_t in_l,
                       const std::string& in_str) override;
  virtual int32_t yadda () override;
  virtual void yadda (int32_t yadda) override;
  virtual bool bool_attr() override;
  virtual void bool_attr(bool) override;
  virtual std::string foo_struct (structType &out_t) override;

private:
  int32_t yadda_;
  int32_t in_l_;
  int32_t in_l_last_;
  bool attr_bool_;
  const std::string in_str_;
  const std::string in_str_last_;
};

class AMI_TEST_SKEL_Export MyDerived_i
: public virtual CORBA::servant_traits< A::MyDerived>::base_type
{
public:
  virtual int32_t foo (int32_t& out_l,
                       int32_t in_l,
                       const std::string& in_str) override;
  virtual int32_t yadda () override;
  virtual void yadda (int32_t yadda) override;
  virtual bool bool_attr() override;
  virtual void bool_attr(bool) override;
  virtual std::string foo_struct (structType &out_t) override;
  virtual int32_t do_something (const std::string& ) override;
  virtual int32_t my_derived_attrib () override;
  virtual void my_derived_attrib (int32_t) override;
};
/// Implement the Hello interface
class AMI_TEST_SKEL_Export Hello_i:
    public virtual CORBA::servant_traits<Hello>::base_type
{
public:
  /// Constructor
  Hello_i(IDL::traits<CORBA::ORB>::ref_type orb,
      IDL::traits<PortableServer::POA>::ref_type poa);

  // = The skeleton methods
  virtual IDL::traits<MyFoo>::ref_type get_iMyFoo() override;
  virtual IDL::traits<A::MyDerived>::ref_type get_iMyDerived() override;

  virtual void shutdown() override;
private:
  /// Use an ORB reference to convert strings to objects and shutdown
  /// the application.
  IDL::traits<CORBA::ORB>::ref_type orb_;
  IDL::traits<PortableServer::POA>::ref_type poa_;
};

#endif /* TAO_AMI_TEST_I_H */
