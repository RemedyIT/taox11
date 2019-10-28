/**
 * @file    ami_test_i.h
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11 interface ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAO_AMI_TEST_I_H
#define TAO_AMI_TEST_I_H

#include "testlib/taox11_testlog.h"

#include "ami_testS.h"

class AMI_TEST_SKEL_Export AMI_Test_i
: public virtual CORBA::servant_traits< A::AMI_Test>::base_type
{
public:
  /// ctor
  AMI_Test_i (IDL::traits<CORBA::ORB>::ref_type orb);

  // The AMI_Test methods.
  virtual int32_t foo (int32_t& out_l,
               int32_t in_l,
               const std::string& in_str) override;

  virtual void bar (A::enumType &out_e) override;

  virtual void shutdown () override;

  virtual int32_t yadda () override;

  virtual void yadda (int32_t yadda) override;

  virtual bool bool_attr() override;

  virtual void bool_attr(bool) override;

  virtual std::string foo_struct (A::structType &out_t) override;

private:
  IDL::traits<CORBA::ORB>::ref_type  orb_;

  int32_t yadda_;

  int32_t in_l_;

  int32_t in_l_last_;

  bool attr_bool_;

  const std::string in_str_;

  const std::string in_str_last_;

};

#endif /* TAO_AMI_TEST_I_H */
