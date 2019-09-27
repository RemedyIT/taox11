/**
 * @file    long_double_argument_t.h
 * @author  Martin Corino
 *
 * @brief   Special argument template instantiations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_LONG_DOUBLE_ARGUMENT_T_H_INCLUDED
#define TAOX11_LONG_DOUBLE_ARGUMENT_T_H_INCLUDED

#include "tao/x11/tao_corba.h"
#include "tao/x11/taox11_export.h"

// In cases where sizeof(long double) == 16 the 'long double' type
// is identical to CORBA::LongDouble (old C++) and the arg traits
// implemented in TAO will be automatically reused
// In other cases though we have to define a new trait specialization.
#if (ACE_SIZEOF_LONG_DOUBLE != 16)

#include "tao/x11/basic_argument_t.h"

namespace TAOX11_NAMESPACE
{

  /**
   * @class In_Basic_Argument_T
   *
   * @brief Template class for IN stub argument of basic IDL types.
   */
  template<template <typename> class Insert_Policy>
  class In_Basic_Argument_T<long double, Insert_Policy>
    : public TAO_TAO::InArgument
  {
  public:
    In_Basic_Argument_T (long double const & x)
      : x_ (x) {}

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    const long double & arg () const { return this->x_; }

  protected:
    long double const & x_;
  };

  /**
   * @class In_Basic_Clonable_Argument_T
   *
   * @brief Template class for IN stub argument of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class In_Basic_Clonable_Argument_T<long double, Insert_Policy>
    : public In_Basic_Argument_T<long double, Insert_Policy>
  {
  public:
    In_Basic_Clonable_Argument_T (long double const & x)
      : In_Basic_Argument_T<long double, Insert_Policy> (x) {}
    virtual ~In_Basic_Clonable_Argument_T ();

    virtual TAO_TAO::Argument* clone () override;

  private:
    bool is_clone_;
  };

  /**
   * @class Inout_Basic_Argument_T
   *
   * @brief Template class for INOUT stub argument of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class Inout_Basic_Argument_T<long double, Insert_Policy>
    : public TAO_TAO::InoutArgument
  {
  public:
    Inout_Basic_Argument_T (long double & x)
      : x_ (x) {}

    virtual TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double & arg () { return this->x_; }

  private:
    long double & x_;
  };

  /**
   * @class Out_Basic_Argument_T
   *
   * @brief Template class for OUT stub argument of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class Out_Basic_Argument_T<long double, Insert_Policy>
    : public TAO_TAO::OutArgument
  {
  public:
    Out_Basic_Argument_T (long double & x)
      : x_ (x) {}

    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double & arg () { return this->x_; }

  private:
    long double & x_;
  };

  /**
   * @class Ret_Basic_Argument_T
   *
   * @brief Template class for return stub value of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class Ret_Basic_Argument_T<long double, Insert_Policy>
    : public TAO_TAO::RetArgument
  {
  public:
    Ret_Basic_Argument_T () = default;

    virtual TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    virtual void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double & arg () { return this->x_; }

    long double excp () { return this->x_; }
    long double retn () { return std::move (this->x_); }

  private:
    long double x_;
  };

} // namespace TAOX11_NAMESPACE

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/long_double_argument_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("long_double_argument_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SIZEOF_LONG_DOUBLE != 16 */

#endif /* TAOX11_LONG_DOUBLE_ARGUMENT_T_H_INCLUDED */
