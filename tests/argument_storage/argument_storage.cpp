/**
 * @file    argument_storage.cpp
 * @author  Johnny Willemsen
 *
 * @brief   Regression test for default initialized argument storage.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/CDR_Stream.h"

#include "tao/x11/basic_arguments.h"
#include "tao/x11/portable_server/basic_sarguments.h"
#include "tao/x11/portable_server/special_basic_sarguments.h"
#include "tao/x11/special_basic_arguments.h"
#include "testlib/taox11_testlog.h"

#include <cstdint>

int
main (int, char *[])
{
  int retval {};

  using boolean_sarg_traits = taox11::PS::SArg_Traits<ACE_InputCDR::to_boolean>;
  using boolean_arg_traits = taox11::Arg_Traits<ACE_InputCDR::to_boolean>;
  using basic_sarg_traits = taox11::PS::SArg_Traits<int32_t>;
  using basic_arg_traits = taox11::Arg_Traits<int32_t>;

  boolean_sarg_traits::ret_val boolean_ret;
  boolean_sarg_traits::out_arg_val boolean_out;
  boolean_sarg_traits::in_arg_val boolean_in;
  boolean_sarg_traits::inout_arg_val boolean_inout;
  if (boolean_ret.arg () || boolean_out.arg () || boolean_in.arg () || boolean_inout.arg ())
    {
      TAOX11_TEST_ERROR << "Boolean skeleton argument storage is not false" << std::endl;
      ++retval;
    }

  TAO_OutputCDR cdr;
  if (!boolean_ret.marshal (cdr))
    {
      TAOX11_TEST_ERROR << "Could not marshal default Boolean return storage" << std::endl;
      ++retval;
    }
  else if (cdr.total_length () != 1 || cdr.begin () == nullptr ||
           static_cast<uint8_t> (*cdr.begin ()->rd_ptr ()) != 0)
    {
      TAOX11_TEST_ERROR << "Default Boolean return storage did not marshal as CDR octet 0" << std::endl;
      ++retval;
    }

  TAO_InputCDR input (cdr);
  boolean_sarg_traits::in_arg_val boolean_round_trip;
  if (!boolean_round_trip.demarshal (input) || boolean_round_trip.arg ())
    {
      TAOX11_TEST_ERROR << "Could not demarshal default Boolean return storage" << std::endl;
      ++retval;
    }

  basic_sarg_traits::ret_val basic_ret;
  basic_sarg_traits::out_arg_val basic_out;
  basic_sarg_traits::in_arg_val basic_in;
  basic_sarg_traits::inout_arg_val basic_inout;
  if (basic_ret.arg () != 0 || basic_out.arg () != 0 || basic_in.arg () != 0 || basic_inout.arg () != 0)
    {
      TAOX11_TEST_ERROR << "Basic skeleton argument storage is not zero" << std::endl;
      ++retval;
    }

  boolean_arg_traits::ret_val client_boolean_ret;
  basic_arg_traits::ret_val client_basic_ret;
  if (client_boolean_ret.arg () || client_basic_ret.arg () != 0)
    {
      TAOX11_TEST_ERROR << "Client return argument storage is not value initialized" << std::endl;
      ++retval;
    }

#if (ACE_SIZEOF_LONG_DOUBLE != 16)
  using long_double_sarg_traits = taox11::PS::SArg_Traits<long double>;
  using long_double_arg_traits = taox11::Arg_Traits<long double>;
  long_double_sarg_traits::ret_val long_double_ret;
  long_double_sarg_traits::out_arg_val long_double_out;
  long_double_arg_traits::ret_val client_long_double_ret;
  if (long_double_ret.arg () != 0.0L || long_double_out.arg () != 0.0L ||
      client_long_double_ret.arg () != 0.0L)
    {
      TAOX11_TEST_ERROR << "Long double argument storage is not zero" << std::endl;
      ++retval;
    }
#endif

  return retval;
}
