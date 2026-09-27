/**
 * @file client.cpp
 * @author Johnny Willemsen
 * @brief IDL fixed decimal regression tests
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "tao/x11/corba.h"
#include "testC.h"
#include "testlib/taox11_testlog.h"
#include "ace/Get_Opt.h"

#include <cstdint>
#include <limits>
#include <sstream>
#include <tuple>

namespace
{
  int errors = 0;

  void check(bool ok, char const* what)
  {
    if (!ok)
    {
      TAOX11_TEST_ERROR << "fixed: " << what << std::endl;
      ++errors;
    }
  }

  template <typename Action>
  void expect_conversion_error(Action action, char const* what)
  {
    bool caught = false;
    try
    {
      action();
    }
    catch (CORBA::DATA_CONVERSION const&)
    {
      caught = true;
    }
    catch (std::exception const& ex)
    {
      TAOX11_TEST_ERROR << "fixed: unexpected exception for " << what << ": " << ex.what() << std::endl;
      ++errors;
      return;
    }
    catch (...)
    {
      TAOX11_TEST_ERROR << "fixed: unexpected exception for " << what << std::endl;
      ++errors;
      return;
    }
    check(caught, what);
  }
}

int main(int argc, char* argv[])
{
  const ACE_TCHAR* ior = ACE_TEXT("file://test.ior");
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:"));
  int c;
  while ((c = get_opts()) != -1)
  {
    if (c == 'k')
      ior = get_opts.opt_arg();
    else
    {
      TAOX11_TEST_ERROR << "usage: -k <ior>" << std::endl;
      return 1;
    }
  }
  static_assert(std::tuple_size<fixed_array>::value == 3);
  static_assert(IDL::traits<pi_type>::digits() == 7);
  static_assert(IDL::traits<pi_type>::scale() == 6);
  static_assert(IDL::traits<V::pi_type>::digits() == 7);
  static_assert(IDL::traits<V::pi_type>::scale() == 6);

  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);
    if (!orb)
    {
      TAOX11_TEST_ERROR << "CORBA::ORB_init returned a null ORB" << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type object = orb->string_to_object(ior);
    if (!object)
    {
      TAOX11_TEST_ERROR << "string_to_object returned a null object" << std::endl;
      return 1;
    }

    IDL::traits<FixedTest>::ref_type fixed_test = IDL::traits<FixedTest>::narrow(object);
    if (!fixed_test)
    {
      TAOX11_TEST_ERROR << "narrow returned a null FixedTest reference" << std::endl;
      return 1;
    }

    fixed_type const zero;
    check(!static_cast<bool>(zero), "zero is false");
    check(zero.to_string() == "0.000", "default value and scale");

    fixed_type const left("1.250");
    fixed_type const right("2.000");
    check(left.to_string() == "1.250", "preserve trailing zeros");
    check(fixed_type("1.250d") == left, "IDL fixed suffix");
    check(left.fixed_digits() == 4 && left.fixed_scale() == 3, "value digits and scale");
    check(static_cast<bool>(left), "nonzero is true");
    check(static_cast<int64_t>(left) == 1, "integer conversion truncates");
    check((left + right).to_string() == "3.250", "addition");
    check((right - left).to_string() == "0.750", "subtraction");
    check((right * left).to_string() == "2.500", "multiplication");
    check((right / fixed_type("1.600")).to_string() == "1.250", "division");
    check((fixed_type("1.000") / fixed_type("3.000")).to_string() == "0.333",
          "division truncates to declared scale");
    check(left < right && right > left && left != right && left == fixed_type("1.25"),
          "comparisons");
    check(fixed_type("-0.005").round(2).to_string() == "-0.010", "round away from zero");
    check(fixed_type("-0.005").truncate(2).to_string() == "0.000", "truncate toward zero");

    fixed_type changed("1.000");
    check((changed++).to_string() == "1.000" && changed.to_string() == "2.000", "postincrement");
    check((--changed).to_string() == "1.000", "predecrement");

    fixed_type swapped("3.000");
    using std::swap;
    swap(changed, swapped);
    check(changed.to_string() == "3.000" && swapped.to_string() == "1.000", "swap");

    std::stringstream stream;
    stream << left;
    check(stream.str().find("1.250") != std::string::npos, "stream output");
    std::istringstream input_stream("1.250");
    fixed_type read;
    input_stream >> read;
    check(read == left, "stream input");

    check(fixed_test->echo_fixed(left) == left, "fixed client/server round trip");

    fixed_array const values {left, right, fixed_type("-3.125")};
    check(fixed_test->echo_fixed_array(values) == values, "fixed array client/server round trip");

    large_type const large("3.142");
    check(fixed_test->echo_large(large) == large, "large fixed client/server round trip");

    pi_type const pi("3.142857");
    check(fixed_test->echo_pi(pi) == pi, "pi fixed client/server round trip");

    V::F::f_type const fraction("0.12345");
    check(fixed_test->echo_fraction(fraction) == fraction,
          "fractional fixed client/server round trip");

    using max_fixed = IDL::Fixed<31, 0>;
    fixed_test->shutdown();
    orb->destroy();

    check(pi_double.to_string() == "3.142857", "global fixed constant");
    check(V::pi.to_string() == "3.142857", "module fixed constant");
    check(V::F::pi.to_string() == "3.142857", "interface fixed constant");
    check(V::F::large.to_string() == "3.142", "large fixed constant");
    check(V::F::fraction.to_string() == "0.12345", "fractional fixed constant");
    check(V::F::fraction.fixed_digits() == 5 && V::F::fraction.fixed_scale() == 5,
          "fractional value digits and scale");

    expect_conversion_error([] { (void)fixed_type("1.2not-a-number"); }, "invalid text must throw DATA_CONVERSION");
    expect_conversion_error([] { (void)fixed_type("1.2345"); }, "excess scale must throw DATA_CONVERSION");
    expect_conversion_error([] { (void)fixed_type("12345678"); }, "excess digits must throw DATA_CONVERSION");
    expect_conversion_error([] { (void)V::F::f_type("12345"); }, "fractional overflow must throw DATA_CONVERSION");

    expect_conversion_error([&] { (void)(left / zero); }, "division by zero must throw DATA_CONVERSION");

    using big_fixed = IDL::Fixed<31, 0>;
    big_fixed const max_value("9999999999999999999999999999999");
    check((max_value / big_fixed(2)).to_string() == "4999999999999999999999999999999",
          "maximum fixed value divided by two");
    using small_fixed = IDL::Fixed<3, 2>;
    small_fixed const large_quotient("9.99");
    expect_conversion_error([&] { (void)(large_quotient / small_fixed("0.01")); },
                            "unrepresentable division must throw DATA_CONVERSION");
    expect_conversion_error([&] { (void)(max_value + big_fixed(1)); },
                            "addition overflow must throw DATA_CONVERSION");
    expect_conversion_error([&] { (void)(max_value * big_fixed(10)); },
                            "multiplication overflow must throw DATA_CONVERSION");
    expect_conversion_error([] { (void)static_cast<int64_t>(big_fixed("9223372036854775808")); },
                            "integer overflow must throw DATA_CONVERSION");

    expect_conversion_error([] { (void)fixed_type("1e20"); }, "exponent must throw DATA_CONVERSION");
    expect_conversion_error([] { (void)fixed_type(std::numeric_limits<double>::infinity()); },
                            "nonfinite value must throw DATA_CONVERSION");
  }
  catch (std::exception const& ex)
  {
    TAOX11_TEST_ERROR << "unexpected exception: " << ex.what() << std::endl;
    ++errors;
  }
  catch (...)
  {
    TAOX11_TEST_ERROR << "unexpected non-standard exception" << std::endl;
    ++errors;
  }
  return errors == 0 ? 0 : 1;
}
