/**
 * @file cdr.cpp
 * @author Johnny Willemsen
 * @brief CDR regression tests for IDL fixed decimal values
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "tao/CDR.h"
#include "tao/x11/array_cdr_t.h"
#include "testlib/taox11_testlog.h"

int test_fixed_cdr(fixed_type const& left, fixed_type const& right)
{
  int errors = 0;
  auto check = [&errors](bool ok, char const* what)
  {
    if (!ok)
    {
      TAOX11_TEST_ERROR << "fixed: " << what << std::endl;
      ++errors;
    }
  };

  TAO_OutputCDR output;
  check(static_cast<bool>(output << left), "CDR write");
  TAO_InputCDR input(output);
  fixed_type decoded;
  check(static_cast<bool>(input >> decoded) && decoded == left, "CDR round trip");

  fixed_array array_value {};
  array_value[0] = left;
  array_value[1] = right;
  array_value[2] = fixed_type("-3.125");
  TAO_OutputCDR array_output;
  check(TAO_VERSIONED_NAMESPACE_NAME::taox11_array_cdr<0U, fixed_type, 3>::insert(
            array_output, array_value), "fixed array CDR write");
  TAO_InputCDR array_input(array_output);
  fixed_array array_decoded {};
  check(TAO_VERSIONED_NAMESPACE_NAME::taox11_array_cdr<0U, fixed_type, 3>::extract(
            array_input, array_decoded) && array_decoded == array_value,
        "fixed array CDR round trip");

  TAO_OutputCDR fractional_output;
  check(static_cast<bool>(fractional_output << V::F::fraction), "CDR all-fraction write");
  TAO_InputCDR fractional_input(fractional_output);
  V::F::f_type fractional_decoded;
  check(static_cast<bool>(fractional_input >> fractional_decoded) &&
        fractional_decoded == V::F::fraction, "CDR all-fraction round trip");

  fixed_type const negative("-12.345");
  TAO_OutputCDR negative_output;
  check(static_cast<bool>(negative_output << negative), "CDR negative write");
  TAO_InputCDR negative_input(negative_output);
  fixed_type negative_decoded;
  check(static_cast<bool>(negative_input >> negative_decoded) &&
        negative_decoded == negative, "CDR negative round trip");

  return errors;
}
