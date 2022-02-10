/**
 * @file    test_impl.h
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAO_TEST_IMPL_H
#define TAO_TEST_IMPL_H

#include "valueboxS.h"

class Test_impl final
  : public virtual CORBA::servant_traits<Test>::base_type
{
public:
  /// ctor
  Test_impl(IDL::traits<CORBA::ORB>::ref_type orb);
  virtual ~Test_impl() = default;

  IDL::traits<VBlong>::ref_type basic_op1(
      IDL::traits<VBlong>::ref_type p1,
      IDL::traits<VBlong>::ref_type& p2,
      IDL::traits<VBlong>::ref_type& p3) override;

  IDL::traits<vb_basic::M_VBlong>::ref_type basic_op2(
      IDL::traits<vb_basic::M_VBlong>::ref_type p1,
      IDL::traits<vb_basic::M_VBlong>::ref_type& p2,
      IDL::traits<vb_basic::M_VBlong>::ref_type& p3) override;

  int32_t basic_op3(int32_t p1, int32_t& p2, int32_t& p3) override;

  IDL::traits<VBstring>::ref_type string_op1(
      IDL::traits<VBstring>::ref_type p1,
      IDL::traits<VBstring>::ref_type& p2,
      IDL::traits<VBstring>::ref_type& p3) override;

  std::string string_op2(const std::string& p1, std::string& p2,
      std::string& p3) override;

  IDL::traits<VBseqlong>::ref_type seq_op1(
      IDL::traits<VBseqlong>::ref_type p1,
      IDL::traits<VBseqlong>::ref_type& p2,
      IDL::traits<VBseqlong>::ref_type& p3) override;

  void seq_op2(const TDseqlong& p1, TDseqlong& p2,
      TDseqlong& p3) override;

  IDL::traits<VBfixed_struct1>::ref_type struct_op1(
      IDL::traits<VBfixed_struct1>::ref_type p1,
      IDL::traits<VBfixed_struct1>::ref_type& p2,
      IDL::traits<VBfixed_struct1>::ref_type& p3) override;

  void struct_op2(const Fixed_Struct1& p1, Fixed_Struct1& p2,
      Fixed_Struct1& p3) override;

  IDL::traits<VBvariable_struct1>::ref_type struct_op3(
      IDL::traits<VBvariable_struct1>::ref_type p1,
      IDL::traits<VBvariable_struct1>::ref_type& p2,
      IDL::traits<VBvariable_struct1>::ref_type& p3) override;

  void struct_op4(const Variable_Struct1& p1,
      Variable_Struct1& p2, Variable_Struct1& p3) override;

  IDL::traits<VBlongarray>::ref_type array_op1(
      IDL::traits<VBlongarray>::ref_type p1,
      IDL::traits<VBlongarray>::ref_type& p2,
      IDL::traits<VBlongarray>::ref_type& p3) override;

  void array_op2(const LongArray& p1, LongArray& p2,
      LongArray& p3) override;

  IDL::traits<VBstringarray>::ref_type array_op3(
      IDL::traits<VBstringarray>::ref_type p1,
      IDL::traits<VBstringarray>::ref_type& p2,
      IDL::traits<VBstringarray>::ref_type& p3) override;

  void array_op4(const StringArray& p1, StringArray& p2,
      StringArray& p3) override;

  IDL::traits<VBfixed_union1>::ref_type union_op1(
      IDL::traits<VBfixed_union1>::ref_type p1,
      IDL::traits<VBfixed_union1>::ref_type& p2,
      IDL::traits<VBfixed_union1>::ref_type& p3) override;

  void union_op2(const Fixed_Union1& p1, Fixed_Union1& p2,
      Fixed_Union1& p3) override;

  IDL::traits<VBvariable_union1>::ref_type union_op3(
      IDL::traits<VBvariable_union1>::ref_type p1,
      IDL::traits<VBvariable_union1>::ref_type& p2,
      IDL::traits<VBvariable_union1>::ref_type& p3) override;

  void union_op4(const Variable_Union1& p1, Variable_Union1& p2,
      Variable_Union1& p3) override;

  void shutdown() override;

private:
  /// The ORB
  IDL::traits<CORBA::ORB>::ref_type orb_;

  Test_impl (const Test_impl&) = delete;
  Test_impl (Test_impl&&) = delete;
  Test_impl& operator= (const Test_impl&) = delete;
  Test_impl& operator= (Test_impl&&) = delete;
};

#endif /* TAO_TEST_IMPL_H */
