/**
 * @file    basic_sargument_t.h
 * @author  Martin Corino
 *
 * @brief   x11 argument traits for all types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_BASIC_SARGUMENT_T_H
#define TAOX11_BASIC_SARGUMENT_T_H

#pragma once

#include "tao/Argument.h"
#include "tao/x11/stddef.h"
#include "tao/x11/tao_corba.h"

namespace TAOX11_NAMESPACE
{
  namespace CORBA
  {
    class Any; // forward
  }

  namespace PS
  {
  /**
   * @class In_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class In_Basic_SArgument_T : public TAO_TAO::InArgument
  {
  public:
    In_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S& arg () { return this->x_; }

  private:
    S x_;
  };

  /**
   * @class Inout_Basic_SArgument_T
   *
   * @brief Template class for INOUT skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Inout_Basic_SArgument_T : public TAO_TAO::InoutArgument
  {
  public:
    Inout_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg () { return this->x_; }

  private:
    S x_;
  };

  /**
   * @class Out_Basic_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Out_Basic_SArgument_T : public TAO_TAO::OutArgument
  {
  public:
    Out_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg () { return this-> x_; }

  private:
    S x_;
  };

  /**
   * @class Ret_Basic_SArgument_T
   *
   * @brief Template class for return skeleton value of basic IDL types.
   *
   */
  template<typename S, template <typename> class Insert_Policy>
  class Ret_Basic_SArgument_T : public TAO_TAO::RetArgument
  {
  public:
    Ret_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg () { return this->x_; }

  private:
    S x_;
  };

  /**
   * @struct Basic_SArg_Traits_T
   *
   * @brief Template class for skeleton argument traits of basic IDL types.
   *
   */
  template<typename T, template <typename> class Insert_Policy>
  struct Basic_SArg_Traits_T
  {
    using ret_type = T;
    using in_type = const T &;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Basic_SArgument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Basic_SArgument_T<T, Insert_Policy>;
    using out_arg_val = Out_Basic_SArgument_T<T, Insert_Policy>;
    using ret_val = Ret_Basic_SArgument_T<T, Insert_Policy>;

    // Types corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    using in_arg_type = in_type;
    using inout_arg_type = inout_type;
    using out_arg_type = out_type;
    using ret_arg_type = out_type;
  };

  } // namespace PS
} // namespace TAOX11_NAMESPACE

#include "tao/x11/portable_server/basic_sargument_t.cpp"

#endif /* TAOX11_BASIC_SARGUMENT_T_H */
