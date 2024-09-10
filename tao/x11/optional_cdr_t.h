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
  template <typename...>
  struct taox11_optional_cdr_in;

  template <typename _Tp>
  struct taox11_optional_cdr_in<_Tp>
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
  };

  template <typename _Tp, typename _Thelper>
  struct taox11_optional_cdr_in<_Tp, _Thelper>
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
        result = _strm << _Thelper(_optional.value ());
      }
      return result;
    }
  };

  template <typename...>
  struct taox11_optional_cdr_out;

  /// Generic sequence CDR streaming helper template
  template <typename _Tp>
  struct taox11_optional_cdr_out<_Tp>
  {
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
        // If the optional doesn't contain a value initialize it
        if (!_optional) _optional.emplace();
        // extract
        if (_strm >> _optional.value ())
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        _optional.reset ();
      }
      return true;
    }
  };

  /// Generic sequence CDR streaming helper template
  template <typename _Tp, typename _Thelper>
  struct taox11_optional_cdr_out<_Tp, _Thelper>
  {
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
        // If the optional doesn't contain a value initialize it
        if (!_optional) _optional.emplace();
        // extract
        if (_strm >> _Thelper(_optional.value ()))
        {
          return true;
        }
        else
        {
          return false;
        }
      }
      else
      {
        _optional.reset ();
      }
      return true;
    }
  };


TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_OPTIONAL_CDR_T_H_INCLUDED
