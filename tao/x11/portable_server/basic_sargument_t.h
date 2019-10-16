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

    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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
    typedef T                           ret_type;
    typedef const T &                   in_type;
    typedef T &                         inout_type;
    typedef T &                         out_type;

    typedef In_Basic_SArgument_T<T, Insert_Policy>     in_arg_val;
    typedef Inout_Basic_SArgument_T<T, Insert_Policy>  inout_arg_val;
    typedef Out_Basic_SArgument_T<T, Insert_Policy>    out_arg_val;
    typedef Ret_Basic_SArgument_T<T, Insert_Policy>    ret_val;

    // Typedefs corresponding to return value of arg() method in both
    // the client and server side argument class templates.
    typedef in_type                     in_arg_type;
    typedef inout_type                  inout_arg_type;
    typedef out_type                    out_arg_type;
    typedef out_type                    ret_arg_type;
  };

  } // namespace PS
} // namespace TAOX11_NAMESPACE

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/portable_server/basic_sargument_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("basic_sargument_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAOX11_BASIC_SARGUMENT_T_H */
