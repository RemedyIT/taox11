/**
 * @file    basic_sarguments.h
 * @author  Martin Corino
 *
 * @brief   x11 argument traits for basic types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_BASIC_SARGUMENTS_H
#define TAOX11_BASIC_SARGUMENTS_H

#pragma once

#include "tao/x11/portable_server/taox11_portableserver_export.h"
#include "tao/x11/anyinsert_policy_t.h"
#include "tao/x11/portable_server/sarg_traits_t.h"
#include "tao/x11/portable_server/basic_sargument_t.h"
#include "tao/x11/portable_server/long_double_sargument_t.h"

namespace TAOX11_NAMESPACE::PS
{
  /**
    *
    * @brief Specialization for void return type.
    *
    */
  template<>
  class TAOX11_PortableServer_Export SArg_Traits<void>
  {
  public:
    using ret_type = void;
    using ret_val = TAO_TAO::RetArgument;
  };

  /**
    *
    * @brief Specializations for basic skeleton arg types,
    *  except (w)char/boolean/octet.
    *
    */
  template<>
  class TAOX11_PortableServer_Export SArg_Traits<int16_t>
    : public
        Basic_SArg_Traits_T<
          int16_t,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<int32_t>
    : public
        Basic_SArg_Traits_T<
          int32_t,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<uint16_t>
    : public
        Basic_SArg_Traits_T<
          uint16_t,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<uint32_t>
    : public
        Basic_SArg_Traits_T<
          uint32_t,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<float>
    : public
        Basic_SArg_Traits_T<
          float,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<double>
    : public
        Basic_SArg_Traits_T<
          double,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<int64_t>
    : public
        Basic_SArg_Traits_T<
          int64_t,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<uint64_t>
    : public
        Basic_SArg_Traits_T<
          uint64_t,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<long double>
    : public
        Basic_SArg_Traits_T<
          long double,
          Any_Insert_Policy_Stream>
  {
  };

  template<>
  class TAOX11_PortableServer_Export SArg_Traits<std::string>
    : public Basic_SArg_Traits_T<
        std::string,
        Any_Insert_Policy_Stream>
  {
  };

#if !defined(ACE_LACKS_STD_WSTRING)
  template<>
  class TAOX11_PortableServer_Export SArg_Traits<std::wstring>
    : public Basic_SArg_Traits_T<
        std::wstring,
        Any_Insert_Policy_Stream>
  {
  };
#endif
} // namespace TAOX11_NAMESPACE::PS

#endif /* TAOX11_BASIC_SARGUMENTS_H */
