/**
 * @file    special_basic_argument_t.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 stub argument traits for special types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SPECIAL_BASIC_ARGUMENT_T_H
#define TAOX11_SPECIAL_BASIC_ARGUMENT_T_H

#pragma once

#include "tao/Argument.h"
#include "tao/x11/base/stddef.h"
#include "tao/x11/base/tao_corba.h"
#include "tao/x11/arg_traits_t.h"

namespace TAOX11_NAMESPACE
{
  /**
   * @class In_Special_Basic_Argument_T
   *
   * @brief Template class for IN stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class In_Special_Basic_Argument_T : public TAO_TAO::InArgument
  {
  public:
    In_Special_Basic_Argument_T (S const &x)
      : x_ (x) {}

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    S const & arg () const { return this->x_; }

  protected:
    S const & x_;
  };

  /**
   * @class In_Special_Basic_Clonable_Argument_T
   *
   * @brief Template class for IN stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class In_Special_Basic_Clonable_Argument_T :
        public In_Special_Basic_Argument_T<S, to_S, from_S, Insert_Policy>
  {
  public:
    In_Special_Basic_Clonable_Argument_T (S const &x)
      : In_Special_Basic_Argument_T<S, to_S, from_S, Insert_Policy> (x)
      , is_clone_ (false) {}
    ~In_Special_Basic_Clonable_Argument_T () override;

    TAO_TAO::Argument* clone () override;

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_Special_Basic_Argument_T
   *
   * @brief Template class for INOUT stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class Inout_Special_Basic_Argument_T : public TAO_TAO::InoutArgument
  {
  public:
    Inout_Special_Basic_Argument_T (S & x)
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
   * @class Out_Special_Basic_Argument_T
   *
   * @brief Template class for OUT stub argument of (w)char/boolean/octet.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class Out_Special_Basic_Argument_T : public TAO_TAO::OutArgument
  {
  public:
    Out_Special_Basic_Argument_T (S & x)
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
   * @brief Template class for return stub value of (w)char/boolean/octet.
   *
   */
  template<typename S,
           typename to_S,
           typename from_S,
           template <typename> class Insert_Policy>
  class Ret_Special_Basic_Argument_T : public TAO_TAO::RetArgument
  {
  public:
    Ret_Special_Basic_Argument_T () = default;

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
   * @struct Special_Basic_Tag
   *
   * @brief Struct for basic IDL type arguments id tag.
   *
   */
  struct TAO_Export Special_Basic_Tag {};

  /**
   * @struct Special_Basic_Arg_Traits_T
   *
   * @brief Template class for stub argument traits of (w)char/boolean/octet.
   *
   */
  template<typename T,
           typename to_T,
           typename from_T,
           template <typename> class Insert_Policy>
  struct Special_Basic_Arg_Traits_T
  {
    using ret_type =  T;
    using in_type = T;
    using inout_type = T &;
    using out_type = T &;

    using in_arg_val = In_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>;
    using in_clonable_arg_val = In_Special_Basic_Clonable_Argument_T<T,to_T,from_T,Insert_Policy>;
    using inout_arg_val = Inout_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>;
    using out_arg_val = Out_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>;
    using ret_val = Ret_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>;
  };
} // namespace TAOX11_NAMESPACE

#include "tao/x11/special_basic_argument_t.cpp"

#endif /* TAOX11_SPECIAL_BASIC_ARGUMENT_T_H */
