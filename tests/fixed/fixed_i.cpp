/**
 * @file fixed_i.cpp
 * @author Johnny Willemsen
 * @brief Servant operations for the IDL fixed decimal regression test
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "fixed_i.h"
#include "testlib/taox11_testlog.h"

FixedTest_i::FixedTest_i(IDL::traits<CORBA::ORB>::ref_type orb)
  : orb_(std::move(orb))
{
}

void FixedTest_i::check(bool ok, const char* what)
{
  if (!ok)
  {
    TAOX11_TEST_ERROR << "fixed servant: " << what << std::endl;
    ++this->errors_;
  }
}

fixed_type FixedTest_i::echo_fixed(const fixed_type& value)
{
  this->check(value == fixed_type("1.250"), "fixed value");
  return value;
}

fixed_array FixedTest_i::echo_fixed_array(const fixed_array& value)
{
  const fixed_array expected {fixed_type("1.250"), fixed_type("2.000"), fixed_type("-3.125")};
  this->check(value == expected, "fixed array");
  return value;
}

large_type FixedTest_i::echo_large(const large_type& value)
{
  this->check(value == large_type("3.142"), "large fixed value");
  return value;
}

pi_type FixedTest_i::echo_pi(const pi_type& value)
{
  this->check(value == pi_type("3.142857"), "pi fixed value");
  return value;
}

V::F::f_type FixedTest_i::echo_fraction(const V::F::f_type& value)
{
  this->check(value == V::F::f_type("0.12345"), "all-fraction fixed value");
  return value;
}

void FixedTest_i::shutdown()
{
  this->orb_->shutdown(false);
}
