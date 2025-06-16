/**
 * @file    bounded_string_cdr_t.h
 * @author  Johnny Willemsen
 *
 * @brief   x11 string marshaling templates
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_STRING_CDR_T_H_INCLUDED
#define TAOX11_STRING_CDR_T_H_INCLUDED

#include "tao/x11/base/tao_corba.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// Generic string CDR streaming helper template
template <typename _Tp>
struct taox11_string_cdr
{
  /// Bounded insert
  template <const uint32_t _Bound, typename _Stream>
  static inline bool insert (_Stream& _strm, const TAOX11_IDL::bounded_string<_Bound>& _bstr)
  {
    return (_strm << ACE_OutputCDR::from_std_string (_bstr, _Bound));
  }

  template <const uint32_t _Bound, typename _Stream>
  static inline bool insert (_Stream& _strm, const TAOX11_IDL::bounded_wstring<_Bound>& _bstr)
  {
    return (_strm << ACE_OutputCDR::from_std_wstring (_bstr, _Bound));
  }

  /// Bounded extract
  template <const uint32_t _Bound, typename _Stream>
  static bool extract (_Stream& _strm, TAOX11_IDL::bounded_string<_Bound>& _bstr)
  {
    return (_strm >> ACE_InputCDR::to_std_string (_bstr, _Bound));
  }

  template <const uint32_t _Bound, typename _Stream>
  static bool extract (_Stream& _strm, TAOX11_IDL::bounded_wstring<_Bound>& _bstr)
  {
    return (_strm >> ACE_InputCDR::to_std_wstring (_bstr, _Bound));
  }
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_STRING_CDR_T_H_INCLUDED
