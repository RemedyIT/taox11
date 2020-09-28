/**
 * @file    basic_arguments.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 stub argument traits for basic types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_BASIC_ARGUMENTS_H
#define TAOX11_BASIC_ARGUMENTS_H

#pragma once

#include "tao/x11/basic_argument_t.h"
#include "tao/x11/long_double_argument_t.h"
#include "tao/x11/arg_traits_t.h"
#include "tao/x11/anyinsert_policy_t.h"

namespace TAOX11_NAMESPACE
{

  /**
   *
   * @brief Specialization for void return type.
   *
   */
  template<>
  class TAOX11_Export Arg_Traits<void>
  {
    public:
    using ret_type = void;
    using ret_val = TAO_TAO::RetArgument ;
    using clonable_ret_val = TAO_TAO::Void_Return_Argument;
  };

  /**
   *
   * @brief Specializations for basic stub arg types,
   *  except (w)char/boolean/octet.
   *
   */

  template<>
  class TAOX11_Export Arg_Traits<int16_t>
    : public
        Basic_Arg_Traits_T<
          int16_t,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<int32_t>
    : public
        Basic_Arg_Traits_T<
          int32_t,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<uint16_t>
    : public
        Basic_Arg_Traits_T<
          uint16_t,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<uint32_t>
    : public
        Basic_Arg_Traits_T<
          uint32_t,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<float>
    : public
        Basic_Arg_Traits_T<
          float,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<double>
    : public
        Basic_Arg_Traits_T<
          double,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<int64_t>
    : public
        Basic_Arg_Traits_T<
          int64_t,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<uint64_t>
    : public
        Basic_Arg_Traits_T<
          uint64_t,
          Any_Insert_Policy_AnyInsert_TAO_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<long double>
    : public
        Basic_Arg_Traits_T<
          long double,
          Any_Insert_Policy_AnyInsert_Adapter>
  {
  };

  template<>
  class TAOX11_Export Arg_Traits<std::string>
    : public
        Basic_Arg_Traits_T <
          std::string,
          Any_Insert_Policy_AnyInsert_Adapter>
  {
  };

#if !defined(ACE_LACKS_STD_WSTRING)
  template<>
  class TAOX11_Export Arg_Traits<std::wstring>
    : public
        Basic_Arg_Traits_T <
          std::wstring,
          Any_Insert_Policy_AnyInsert_Adapter>
  {
  };
#endif /* ACE_LACKS_STD_WSTRING */

} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_BASIC_ARGUMENTS_H */
