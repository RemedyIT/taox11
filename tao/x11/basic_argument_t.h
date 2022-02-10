/**
 * @file    basic_argument_t.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 stub argument traits for all types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_BASIC_ARGUMENT_T_H
#define TAOX11_BASIC_ARGUMENT_T_H

#pragma once

#include "tao/Argument.h"

#include "tao/x11/stddef.h"
#include "tao/x11/tao_corba.h"
#include "tao/x11/arg_traits_t.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class In_Basic_Argument_T
   *
   * @brief Template class for IN stub argument of basic IDL types.
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Basic_Argument_T : public TAO_TAO::InArgument
  {
  public:
    In_Basic_Argument_T (S const & x)
      : x_ (x) {}

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr ) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg () const { return this->x_; }

  protected:
    S const & x_;
  };

  /**
   * @class In_Basic_Clonable_Argument_T
   *
   * @brief Template class for IN stub argument of basic IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class In_Basic_Clonable_Argument_T :
                      public In_Basic_Argument_T<S, Insert_Policy>
  {
  public:
    In_Basic_Clonable_Argument_T (S const & x)
      : In_Basic_Argument_T<S, Insert_Policy> (x)
      , is_clone_ (false) {}
    virtual ~In_Basic_Clonable_Argument_T ();

    TAO_TAO::Argument* clone () override;

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_Basic_Argument_T
   *
   * @brief Template class for INOUT stub argument of basic IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Inout_Basic_Argument_T : public TAO_TAO::InoutArgument
  {
  public:
    Inout_Basic_Argument_T (S & x)
      : x_ (x) {}

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg () { return this->x_; }

  private:
    S & x_;
  };

  /**
   * @class Out_Basic_Argument_T
   *
   * @brief Template class for OUT stub argument of basic IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Out_Basic_Argument_T : public TAO_TAO::OutArgument
  {
  public:
    Out_Basic_Argument_T (S & x)
      : x_ (x) {}

    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg () { return this->x_; }

  private:
    S & x_;
  };

  /**
   * @class Ret_Basic_Argument_T
   *
   * @brief Template class for return stub value of basic IDL types.
   *
   */
  template<typename S,
           template <typename> class Insert_Policy>
  class Ret_Basic_Argument_T : public TAO_TAO::RetArgument
  {
  public:
    Ret_Basic_Argument_T () = default;

    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S & arg () { return this->x_; }

    S excp () { return this->x_; }
    S retn () { return std::move (this->x_); }

  private:
    S x_;
  };

  /**
   * @struct Basic_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of basic IDL types.
   *
   */
  template<typename T, template <typename> class Insert_Policy>
  struct Basic_Arg_Traits_T
  {
    using ret_type = T;
    using in_type = T;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Basic_Argument_T<T, Insert_Policy>;
    using in_clonable_arg_val = In_Basic_Clonable_Argument_T<T, Insert_Policy>;
    using inout_arg_val = Inout_Basic_Argument_T<T, Insert_Policy>;
    using out_arg_val = Out_Basic_Argument_T<T, Insert_Policy>;
    using ret_val = Ret_Basic_Argument_T<T, Insert_Policy>;
  };
} // namespace TAOX11_NAMESPACE

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/basic_argument_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("basic_argument_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAOX11_BASIC_ARGUMENT_T_H */
