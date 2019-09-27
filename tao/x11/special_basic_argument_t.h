/**
 * @file    special_basic_argument_t.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 stub argument traits for special types
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_SPECIAL_BASIC_ARGUMENT_T_H
#define TAOX11_SPECIAL_BASIC_ARGUMENT_T_H

#pragma once

#include "tao/Argument.h"
#include "tao/x11/stddef.h"
#include "tao/x11/tao_corba.h"
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

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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
    virtual ~In_Special_Basic_Clonable_Argument_T ();

    virtual TAO_TAO::Argument* clone () override;

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

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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

    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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

    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
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
    typedef T                                               ret_type;
    typedef T                                               in_type;
    typedef T &                                             inout_type;
    typedef T &                                             out_type;

    typedef In_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>      in_arg_val;
    typedef In_Special_Basic_Clonable_Argument_T<T,to_T,from_T,Insert_Policy>
                                                                          in_clonable_arg_val;
    typedef Inout_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>   inout_arg_val;
    typedef Out_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>     out_arg_val;
    typedef Ret_Special_Basic_Argument_T<T,to_T,from_T,Insert_Policy>     ret_val;
  };
} // namespace TAOX11_NAMESPACE

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/special_basic_argument_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("special_basic_argument_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAOX11_SPECIAL_BASIC_ARGUMENT_T_H */
