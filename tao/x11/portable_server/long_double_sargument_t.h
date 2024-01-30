/**
 * @file    long_double_sargument_t.h
 * @author  Martin Corino
 *
 * @brief   Special argument template instantiation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_LONG_DOUBLE_SARGUMENT_T_H_INCLUDED
#define TAOX11_LONG_DOUBLE_SARGUMENT_T_H_INCLUDED

// In cases where sizeof(long double) == 16 the 'long double' type
// is identical to CORBA::LongDouble (old C++) and the arg traits
// implemented in TAO will be automatically reused
// In other cases though we have to define a new trait specialization.
#if (ACE_SIZEOF_LONG_DOUBLE != 16)

namespace TAOX11_NAMESPACE::PS
{
  /**
   * @class In_Basic_SArgument_T
   *
   * @brief Template class for IN skeleton argument of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class In_Basic_SArgument_T<long double, Insert_Policy>
    : public TAO_TAO::InArgument
  {
  public:
    In_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double& arg () { return this->x_; }

  private:
    long double x_;
  };

  /**
   * @class Inout_Basic_SArgument_T
   *
   * @brief Template class for INOUT skeleton argument of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class Inout_Basic_SArgument_T<long double, Insert_Policy>
    : public TAO_TAO::InoutArgument
  {
  public:
    Inout_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
    TAO_CORBA::Boolean demarshal (TAO_InputCDR &) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double & arg () { return this->x_; }

  private:
    long double x_;
  };

  /**
   * @class Out_Basic_SArgument_T
   *
   * @brief Template class for OUT skeleton argument of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class Out_Basic_SArgument_T<long double, Insert_Policy>
    : public TAO_TAO::OutArgument
  {
  public:
    Out_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double & arg () { return this->x_; }

  private:
    long double x_;
  };

  /**
   * @class Ret_Basic_SArgument_T
   *
   * @brief Template class for return skeleton value of basic IDL types.
   *
   */
  template<template <typename> class Insert_Policy>
  class Ret_Basic_SArgument_T<long double, Insert_Policy>
    : public TAO_TAO::RetArgument
  {
  public:
    Ret_Basic_SArgument_T () = default;

    TAO_CORBA::Boolean marshal (TAO_OutputCDR &cdr) override;
#if TAO_HAS_INTERCEPTORS == 1
    void interceptor_value (TAO_CORBA::Any *any) const override;
#endif /* TAO_HAS_INTERCEPTORS == 1 */
    long double & arg () { return this->x_; }

  private:
    long double x_;
  };
} // namespace TAOX11_NAMESPACE::PS

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/x11/portable_server/long_double_sargument_t.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("long_double_sargument_t.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SIZEOF_LONG_DOUBLE != 16 */

#endif /* TAOX11_LONG_DOUBLE_SARGUMENT_T_H_INCLUDED */
