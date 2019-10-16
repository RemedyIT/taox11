/**
 * @file    marshal_policy_t.h
 * @author  Martin Corino
 *
 * @brief   TAOX11 marshaling policy templates for
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_MARSHAL_POLICY_T_H
#define TAOX11_MARSHAL_POLICY_T_H

#pragma once

namespace TAOX11_NAMESPACE
{
  template <typename TRAITS>
  class NULL_Marshal_Policy
  {
  public:
    static bool demarshal_value (TAO_InputCDR&, typename TRAITS::value_type&)
    { return false; }
    static bool marshal_value (TAO_OutputCDR&, const typename TRAITS::value_type&)
    { return false; }
  };

  template <typename TRAITS>
  class CDR_Marshal_Policy
  {
  public:
    static bool demarshal_value (TAO_InputCDR& cdr, typename TRAITS::value_type& value)
    { return (cdr >> value); }
    static bool marshal_value (TAO_OutputCDR& cdr, const typename TRAITS::value_type& value)
    { return (cdr << value); }
  };

  template <typename TRAITS>
  class CDR_Marshal_byval_Policy
  {
  public:
    static bool demarshal_value (TAO_InputCDR& cdr, typename TRAITS::value_type& value)
    { return (cdr >> value); }
    static bool marshal_value (TAO_OutputCDR& cdr, typename TRAITS::value_type value)
    { return (cdr << value); }
  };

  template <typename TRAITS>
  class NULL_Bounded_Marshal_Policy
  {
  public:
    static bool demarshal_value (TAO_InputCDR&, typename TRAITS::value_type&, uint32_t)
    { return false; }
    static bool marshal_value (TAO_OutputCDR&, const typename TRAITS::value_type&, uint32_t)
    { return false; }
  };

  template <typename TRAITS>
  class CDR_Bounded_Marshal_Policy
  {
  public:
    static bool demarshal_value (TAO_InputCDR& cdr, typename TRAITS::value_type& value, uint32_t bound)
    { return (cdr >> typename TRAITS::to (value, bound)); }
    static bool marshal_value (TAO_OutputCDR& cdr, const typename TRAITS::value_type& value, uint32_t bound)
    { return (cdr << typename TRAITS::from (value, bound)); }
  };
} // namespace TAOX11_NAMESPACE

#endif /* TAOX11_MARSHAL_POLICY_T_H */
