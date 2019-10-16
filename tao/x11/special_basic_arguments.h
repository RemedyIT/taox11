/**
 * @file    special_basic_arguments.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 stub argument traits for special types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SPECIAL_BASIC_ARGUMENTS_H
#define TAOX11_SPECIAL_BASIC_ARGUMENTS_H

#pragma once

#include "tao/x11/special_basic_argument_t.h"
#include "tao/x11/arg_traits_t.h"
#include "tao/x11/anyinsert_policy_t.h"

namespace TAOX11_NAMESPACE
{

  /**
   *
   * @brief Specializations for (w)char, octet and boolean.
   *
   */

  template<>
  class TAOX11_Export Arg_Traits<ACE_InputCDR::to_char>
    : public Special_Basic_Arg_Traits_T<char,
                                        ACE_InputCDR::to_char,
                                        ACE_OutputCDR::from_char,
                                        Any_Insert_Policy_AnyInsert_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<ACE_InputCDR::to_wchar>
    : public Special_Basic_Arg_Traits_T<wchar_t,
                                        ACE_InputCDR::to_wchar,
                                        ACE_OutputCDR::from_wchar,
                                        Any_Insert_Policy_AnyInsert_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<ACE_InputCDR::to_octet>
    : public Special_Basic_Arg_Traits_T<uint8_t,
                                        ACE_InputCDR::to_octet,
                                        ACE_OutputCDR::from_octet,
                                        Any_Insert_Policy_AnyInsert_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<ACE_InputCDR::to_boolean>
    : public Special_Basic_Arg_Traits_T<bool,
                                        ACE_InputCDR::to_boolean,
                                        ACE_OutputCDR::from_boolean,
                                        Any_Insert_Policy_AnyInsert_Adapter>
  {
  };
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_SPECIAL_BASIC_ARGUMENTS_H */
