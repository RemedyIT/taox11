/**
 * @file    test_impl.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_impl.h"
#include "testlib/taox11_testlog.h"

#include <array>

Test_impl::Test_impl(IDL::traits<CORBA::ORB>::ref_type orb) :
    orb_(std::move(orb))
{
}

//***************************************************************************
// Rotate characters in string, last character becomes the first and
// all others are shifted to the right.
//***************************************************************************
void rotate_string(std::string& s)
{
  if (s.size () > 1)
  {
    s.insert (0, 1, s.back ());
    s.resize (s.size () - 1);
  }
}

std::string rotate_string2(const std::string& s_in)
{
  std::string s(s_in);
  rotate_string (s);
  return s;
}

IDL::traits<VBlong>::ref_type Test_impl::basic_op1(
    IDL::traits<VBlong>::ref_type p1,
    IDL::traits<VBlong>::ref_type& p2,
    IDL::traits<VBlong>::ref_type& p3)
{
  p3 = CORBA::make_reference < VBlong > (p2->_value() * 5);

  p2->_value(p2->_value() * 3);

  IDL::traits<VBlong>::ref_type retval = 0;
  retval = CORBA::make_reference < VBlong > (p1->_value() * 3);

  // Compile test for ostringstream write
  std::ostringstream ss;
  ss << IDL::traits<VBlong>::write (p3) << std::endl;

  return retval;
}

IDL::traits<vb_basic::M_VBlong>::ref_type Test_impl::basic_op2(
    IDL::traits<vb_basic::M_VBlong>::ref_type p1,
    IDL::traits<vb_basic::M_VBlong>::ref_type& p2,
    IDL::traits<vb_basic::M_VBlong>::ref_type& p3)
{
  p3 = CORBA::make_reference < vb_basic::M_VBlong > (p2->_value() * 5);

  p2->_value(p2->_value() * 3);

  IDL::traits<vb_basic::M_VBlong>::ref_type retval = 0;
  retval = CORBA::make_reference < vb_basic::M_VBlong > (p1->_value() * 3);

  return retval;
}

int32_t Test_impl::basic_op3(int32_t p1, int32_t& p2, int32_t& p3)
{
  p3 = p2 * 5;
  p2 = p2 * 3;
  return p1 * 3;
}

IDL::traits<VBstring>::ref_type Test_impl::string_op1(
    IDL::traits<VBstring>::ref_type p1,
    IDL::traits<VBstring>::ref_type& p2,
    IDL::traits<VBstring>::ref_type& p3)
{
  rotate_string(p2->_value());

  p3 = CORBA::make_reference < VBstring > (p2->_value());

  rotate_string(p1->_value());

  IDL::traits<VBstring>::ref_type retval = 0;
  retval = CORBA::make_reference < VBstring > (p1->_value());

  return retval;
}

std::string Test_impl::string_op2(const std::string& p1, std::string& p2,
    std::string& p3)
{
  rotate_string(p2);
  p3 = std::string(p2);

  std::string return_string (p1);
  rotate_string(return_string);

  return return_string;
}

IDL::traits<VBseqlong>::ref_type Test_impl::seq_op1(
    IDL::traits<VBseqlong>::ref_type p1,
    IDL::traits<VBseqlong>::ref_type& p2,
    IDL::traits<VBseqlong>::ref_type& p3)
{
  // Copy the value of p2
  p3 = CORBA::make_reference < VBseqlong > (p2->_value());
  for (int32_t& item : p3->_value())
  {
    item *= 5;
  }
  for (int32_t& item : p2->_value())
  {
    item *= 3;
  }

  IDL::traits<VBseqlong>::ref_type retval = 0;
  retval = p1;
  return retval;
}

void Test_impl::seq_op2(const TDseqlong& p1, TDseqlong& p2, TDseqlong& p3)
{
  p3 = TDseqlong(p1);
  for (int32_t& item : p3)
  {
    item *= 5;
  }
  for (int32_t& item : p2)
  {
    item *= 3;
  }
}

IDL::traits<VBfixed_struct1>::ref_type Test_impl::struct_op1(
    IDL::traits<VBfixed_struct1>::ref_type p1,
    IDL::traits<VBfixed_struct1>::ref_type& p2,
    IDL::traits<VBfixed_struct1>::ref_type& p3)
{
  p3 = CORBA::make_reference < VBfixed_struct1 > (p2->_value());
// transform p3 values
  p3->_value().l(p3->_value().l() * 5);
  p3->_value().abstruct().s1() = (p3->_value().abstruct()).s1() * 5;
  p3->_value().abstruct().s2() = (p3->_value().abstruct()).s2() * 5;

// transform p2 values
  p2->_value().l(p2->_value().l() * 3);
  p2->_value().abstruct().s1() = (p2->_value().abstruct()).s1() * 3;
  p2->_value().abstruct().s2() = (p2->_value().abstruct()).s2() * 3;

  IDL::traits<VBfixed_struct1>::ref_type retval = 0;
  retval = CORBA::make_reference < VBfixed_struct1 > (p1->_value());

  return retval;
}

void Test_impl::struct_op2(const Fixed_Struct1& p1, Fixed_Struct1& p2,
    Fixed_Struct1& p3)
{
  p3 = p1;
  p2.l() *= 3;
  p2.abstruct().s1() *= 3;
  p2.abstruct().s2() *= 3;
}

IDL::traits<VBvariable_struct1>::ref_type Test_impl::struct_op3(
    IDL::traits<VBvariable_struct1>::ref_type p1,
    IDL::traits<VBvariable_struct1>::ref_type& p2,
    IDL::traits<VBvariable_struct1>::ref_type& p3)
{
// transform p2 values
  p2->_value().l(p2->_value().l() * 3);
  rotate_string(p2->_value().str());
  p3 = CORBA::make_reference < VBvariable_struct1 > (p2->_value());
  IDL::traits<VBvariable_struct1>::ref_type retval = 0;
  retval = CORBA::make_reference < VBvariable_struct1 > (p1->_value());
  return retval;
}

void Test_impl::struct_op4(const Variable_Struct1& p1, Variable_Struct1& p2,
    Variable_Struct1& p3)
{
  p3 = Variable_Struct1();
  p3.l(p1.l());
  p3.str(p1.str());
  p2.l(p2.l() * 3);
  rotate_string(p2.str());
}

IDL::traits<VBlongarray>::ref_type Test_impl::array_op1(
    IDL::traits<VBlongarray>::ref_type p1,
    IDL::traits<VBlongarray>::ref_type& p2,
    IDL::traits<VBlongarray>::ref_type& p3)
{
  for (int32_t& pitem : p2->_value())
    pitem *= 3;
  p3 = CORBA::make_reference < VBlongarray > (p2->_value());
  IDL::traits<VBlongarray>::ref_type retval = 0;
  retval = CORBA::make_reference < VBlongarray > (p1->_value());
  return retval;
}

void Test_impl::array_op2(const LongArray& p1, LongArray& p2,
    LongArray& p3)
{
  for (int32_t& pitem : p2)
    pitem *= 3;
  for (LongArray::size_type i = 0; i < p1.size(); i++)
  {
    p3[i] = p1[i];
  }
}

IDL::traits<VBstringarray>::ref_type Test_impl::array_op3(
    IDL::traits<VBstringarray>::ref_type p1,
    IDL::traits<VBstringarray>::ref_type& p2,
    IDL::traits<VBstringarray>::ref_type& p3)
{
  for (std::string& p : p2->_value())
  {
    rotate_string(p);
  }
  p3 = CORBA::make_reference < VBstringarray > (p2->_value());
  IDL::traits<VBstringarray>::ref_type retval = CORBA::make_reference
      < VBstringarray > (p1->_value());
  return retval;
}

void Test_impl::array_op4(const StringArray& p1, StringArray& p2,
    StringArray& p3)
{
  for (std::string& p : p2)
  {
    rotate_string(p);
  }

  StringArray sa = StringArray();

  for (StringArray::size_type i = 0; i < p1.size(); i++)
  {
    sa[i] = p1[i];
  }

  p3 = sa;
}

IDL::traits<VBfixed_union1>::ref_type Test_impl::union_op1(
    IDL::traits<VBfixed_union1>::ref_type p1,
    IDL::traits<VBfixed_union1>::ref_type& p2,
    IDL::traits<VBfixed_union1>::ref_type& p3)
{
  TAOX11_TEST_DEBUG << "Method Test_impl::union_op1." << std::endl;
  int32_t longValue;
  int16_t shortValue;
  p3 = CORBA::make_reference<VBfixed_union1>();
  IDL::traits<VBfixed_union1>::ref_type rv = 0;
  rv = CORBA::make_reference<VBfixed_union1>();

// Set output and return values
  switch (p1->_value()._d())
  {
  case 1: // Long
    longValue = p1->_value().m1() * 3;
    p3->_value().m1(longValue);
    rv->_value().m1(longValue);
    break;
  case 2: // Short
    shortValue = p1->_value().m2() * 3;
    p3->_value().m2(shortValue);
    rv->_value().m2(shortValue);
    break;
  }

// Set new_ "inout" parameter value
  switch (p2->_value()._d())
  {
  case 1: // Long
    p2->_value().m1(p2->_value().m1() * 3);
    break;
  case 2: // Short
    p2->_value().m2(p2->_value().m2() * 3);
    break;
  }

  return rv;
}

void Test_impl::union_op2(const Fixed_Union1& p1, Fixed_Union1& p2,
    Fixed_Union1& p3)

{
  TAOX11_TEST_DEBUG << "Method Test_impl::union_op2." << std::endl;
  p3 = p1;
// Set new_ "inout" parameter value
  switch (p2._d())
  {
  case 1: // Long
    p2.m1(p2.m1() * 3);
    break;
  case 2: // Short
    p2.m2(p2.m2() * 3);
    break;
  }
}

IDL::traits<VBvariable_union1>::ref_type Test_impl::union_op3(
    IDL::traits<VBvariable_union1>::ref_type p1,
    IDL::traits<VBvariable_union1>::ref_type& p2,
    IDL::traits<VBvariable_union1>::ref_type& p3)
{
  TAOX11_TEST_DEBUG << "Method Test_impl::union_op3." << std::endl;
  int32_t longValue;
  std::string stringValue;
  p3 = CORBA::make_reference<VBvariable_union1>();
  IDL::traits<VBvariable_union1>::ref_type rv = nullptr;
  rv = CORBA::make_reference<VBvariable_union1>();

// Set output and return values
  switch (p1->_value()._d())
  {
  case 1: // Long
    longValue = p1->_value().m1();
    p3->_value().m1(longValue);
    rv->_value().m1(longValue);
    break;
  case 2: // String
    stringValue = p1->_value().m2();
    p3->_value().m2(stringValue);
    rv->_value().m2(stringValue);
    break;
  }

// Set new_ "inout" parameter value
  switch (p2->_value()._d())
  {
  case 1: // Long
    p2->_value().m1(p2->_value().m1() * 3);
    break;
  case 2: // String
    stringValue = p2->_value().m2();
    rotate_string(stringValue);
    p2->_value().m2(stringValue);
    break;
  }

  return rv;
}

void Test_impl::union_op4(const Variable_Union1& p1, Variable_Union1& p2,
    Variable_Union1& p3)
{
  TAOX11_TEST_DEBUG << "Method Test_impl::union_op4." << std::endl;
  std::string stringValue;

// Return value of "in" parameter to "out" parameter
  p3 = Variable_Union1();

  switch (p1._d())
  {
  case 1: // Long
    p3.m1(p1.m1());
    break;
  case 2: // String
    p3.m2(p1.m2());
    break;
  }

// Set new_ "inout" parameter value
  switch (p2._d())
  {
  case 1: // Long
    p2.m1(p2.m1() * 3);
    break;
  case 2: // String
    stringValue = p2.m2();
    rotate_string(stringValue);
    p2.m2(stringValue);
    break;
  }
}

void Test_impl::shutdown()
{
  this->orb_->shutdown(0);
}
