/**
 * @file    special_basic_sarguments.h
 * @author  Martin Corino
 *
 * @brief   x11 argument traits for special types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SPECIAL_BASIC_SARGUMENTS_H
#define TAOX11_SPECIAL_BASIC_SARGUMENTS_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"
#include "tao/x11/anyinsert_policy_t.h"
#include "tao/x11/portable_server/sarg_traits_t.h"
#include "tao/x11/portable_server/special_basic_sargument_t.h"

#include "ace/CDR_Stream.h"

namespace TAOX11_NAMESPACE::PS
{
  /**
   *
   * @brief Specializations for (w)char, octet and boolean.
   *
   */

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<ACE_InputCDR::to_char>
    : public Special_Basic_SArg_Traits_T<TAO_CORBA::Char,
                                         ACE_InputCDR::to_char,
                                         ACE_OutputCDR::from_char,
                                         Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<ACE_InputCDR::to_wchar>
    : public Special_Basic_SArg_Traits_T<TAO_CORBA::WChar,
                                         ACE_InputCDR::to_wchar,
                                         ACE_OutputCDR::from_wchar,
                                         Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<ACE_InputCDR::to_octet>
    : public Special_Basic_SArg_Traits_T<TAO_CORBA::Octet,
                                         ACE_InputCDR::to_octet,
                                         ACE_OutputCDR::from_octet,
                                         Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<ACE_InputCDR::to_boolean>
    : public Special_Basic_SArg_Traits_T<TAO_CORBA::Boolean,
                                         ACE_InputCDR::to_boolean,
                                         ACE_OutputCDR::from_boolean,
                                         Any_Insert_Policy_Stream>

  {
  };
} // namespace TAOX11_NAMESPACE::PS

#endif /* TAOX11_SPECIAL_BASIC_SARGUMENTS_H */
