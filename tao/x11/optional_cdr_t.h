/**
 * @file    optional_cdr_t.h
 * @author  Johnny Willemsen
 *
 * @brief   x11 optional marshaling templates
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_OPTIONAL_CDR_T_H_INCLUDED
#define TAOX11_OPTIONAL_CDR_T_H_INCLUDED

#include "tao/x11/base/tao_corba.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

  /// Generic sequence CDR streaming helper template
  template <typename _Tp>
  struct taox11_optional_cdr
  {
    /// Unbounded insert
    template <typename _Stream>
    static bool insert (_Stream& _strm, const _Tp& _optional)
    {
      if (!(_strm << ACE_OutputCDR::from_boolean (_optional.has_value ())))
      {
        return false;
      }

      bool result { true };
      if (_optional.has_value ())
      {
        result = _strm << _optional.value ();
      }
      return result;
    }

    /// Unbounded extract
    template <typename _Stream>
    static bool extract (_Stream& _strm, _Tp& _optional)
    {
      bool _has_value{};
      if (!(_strm >> ACE_InputCDR::to_boolean (_has_value)))
      {
        return false;
      }

      if (_has_value)
      {
        // initialize associated default value
        typename _Tp::value_type temp_val{};
        // extract
        if (_strm >> temp_val)
        {
          // set union member and associated discriminant when there are multiple legal discriminant values
          _optional = std::move (temp_val);
          return true;
        }
      }
      else
      {
        _optional.reset ();
      }
      return false;
    }
  };

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_OPTIONAL_CDR_T_H_INCLUDED
