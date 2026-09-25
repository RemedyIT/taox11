/**
 * @file client.cpp
 * @brief IDL fixed decimal regression tests
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"

#include <cstdint>
#include <limits>
#include <sstream>

namespace
{
  int errors = 0;

  void check(bool ok, const char* what)
  {
    if (!ok)
    {
      TAOX11_TEST_ERROR << "fixed: " << what << std::endl;
      ++errors;
    }
  }

  template <typename F>
  void expect_conversion_error(const char* input)
  {
    try
    {
      F value(input);
      (void)value;
      check(false, "expected DATA_CONVERSION");
    }
    catch (const CORBA::DATA_CONVERSION&) {}
  }
}

int main(int, char*[])
{
  static_assert(IDL::traits<pi_type>::digits() == 7);
  static_assert(IDL::traits<pi_type>::scale() == 6);
  static_assert(IDL::traits<V::pi_type>::digits() == 7);
  static_assert(IDL::traits<V::pi_type>::scale() == 6);

  try
  {
    const fixed_type zero;
    check(!static_cast<bool>(zero), "zero is false");
    check(zero.to_string() == "0.000", "default value and scale");

    const fixed_type left("1.250");
    const fixed_type right("2.000");
    check(left.to_string() == "1.250", "preserve trailing zeros");
    check(left.fixed_digits() == 4 && left.fixed_scale() == 3, "value digits and scale");
    check(static_cast<bool>(left), "nonzero is true");
    check(static_cast<int64_t>(left) == 1, "integer conversion truncates");
    check((left + right).to_string() == "3.250", "addition");
    check((right - left).to_string() == "0.750", "subtraction");
    check((right * left).to_string() == "2.500", "multiplication");
    check((right / fixed_type("1.600")).to_string() == "1.250", "division");
    check(left < right && right > left && left != right && left == fixed_type("1.25"),
          "comparisons");
    check(fixed_type("-0.005").round(2).to_string() == "-0.010", "round away from zero");
    check(fixed_type("-0.005").truncate(2).to_string() == "0.000", "truncate toward zero");

    fixed_type changed("1.000");
    check((changed++).to_string() == "1.000" && changed.to_string() == "2.000", "postincrement");
    check((--changed).to_string() == "1.000", "predecrement");

    std::stringstream stream;
    stream << left;
    check(stream.str() == "1.250", "stream output");
    fixed_type read;
    stream >> read;
    check(read == left, "stream input");

    TAO_OutputCDR output;
    check(static_cast<bool>(output << left), "CDR write");
    TAO_InputCDR input(output);
    fixed_type decoded;
    check(static_cast<bool>(input >> decoded) && decoded == left, "CDR round trip");

    TAO_OutputCDR fractional_output;
    check(static_cast<bool>(fractional_output << V::F::fraction), "CDR all-fraction write");
    TAO_InputCDR fractional_input(fractional_output);
    V::F::f_type fractional_decoded;
    check(static_cast<bool>(fractional_input >> fractional_decoded) &&
          fractional_decoded == V::F::fraction, "CDR all-fraction round trip");

    check(pi_double.to_string() == "3.142857", "global fixed constant");
    check(V::pi.to_string() == "3.142857", "module fixed constant");
    check(V::F::pi.to_string() == "3.142857", "interface fixed constant");
    check(V::F::large.to_string() == "3.142", "large fixed constant");
    check(V::F::fraction.to_string() == "0.12345", "fractional fixed constant");

    expect_conversion_error<fixed_type>("1.2not-a-number");
    expect_conversion_error<fixed_type>("1.2345");
    expect_conversion_error<fixed_type>("12345678");
    expect_conversion_error<V::F::f_type>("12345");

    try
    {
      (void)(left / zero);
      check(false, "division by zero must throw");
    }
    catch (const CORBA::DATA_CONVERSION&) {}

    using big_fixed = IDL::Fixed<31, 0>;
    const big_fixed max_value("9999999999999999999999999999999");
    try
    {
      (void)(max_value + big_fixed(1));
      check(false, "addition overflow must throw");
    }
    catch (const CORBA::DATA_CONVERSION&) {}
    try
    {
      (void)(max_value * big_fixed(10));
      check(false, "multiplication overflow must throw");
    }
    catch (const CORBA::DATA_CONVERSION&) {}

    try
    {
      (void)static_cast<int64_t>(big_fixed("9223372036854775808"));
      check(false, "integer conversion overflow must throw");
    }
    catch (const CORBA::DATA_CONVERSION&) {}

    expect_conversion_error<fixed_type>("1e20");
    try
    {
      (void)fixed_type(std::numeric_limits<double>::infinity());
      check(false, "nonfinite value must throw");
    }
    catch (const CORBA::DATA_CONVERSION&) {}
  }
  catch (const std::exception& ex)
  {
    TAOX11_TEST_ERROR << "unexpected exception: " << ex << std::endl;
    ++errors;
  }
  return errors == 0 ? 0 : 1;
}
