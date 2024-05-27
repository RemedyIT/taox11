/**
 * @file    map_cdr_t.h
 * @author  Johnny Willemsen
 *
 * @brief   x11 map marshaling templates
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_MAP_CDR_T_H_INCLUDED
#define TAOX11_MAP_CDR_T_H_INCLUDED

#include "tao/x11/base/tao_corba.h"
#include "tao/x11/base/taox11_export.h"
#include "tao/x11/base/basic_traits.h"
#include "tao/x11/base/bounded_map_t.h"

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
          _optional= std::move (temp_val);
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


  /// Generic sequence CDR streaming helper template
  template <typename _Tp, typename _Tx>
  struct taox11_map_cdr
  {
    /// Unbounded insert
    template <typename _Stream>
    static bool insert (_Stream& _strm, const std::map<_Tp,_Tx>& _map)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_map.size ());

      if (!(_strm << length))
      {
        return false;
      }

      for (auto& _el : _map)
      {
        if (!(_strm << _el.first))
        {
          return false;
        }
        if (!(_strm << _el.second))
        {
          return false;
        }
      }

      return true;
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (_Stream& _strm, const TAOX11_IDL::bounded_map<_Tp, _Tx, _Bound>& _map)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_map.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _map);
    }

    /// Unbounded extract
    template <typename _Stream>
    static bool extract (_Stream& _strm, std::map<_Tp,_Tx>& _map)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::map<_Tp, _Tx> _new_map;
      for (uint32_t i = 0; i < length; ++i)
      {
        _Tp key {};
        _Tx mapped_type {};
        if (!(_strm >> key))
        {
          return false;
        }
        if (!(_strm >> mapped_type))
        {
          return false;
        }
        _new_map.insert({std::move(key), std::move(mapped_type)});
      }

      _map.swap (_new_map);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static bool extract (_Stream& _strm, TAOX11_IDL::bounded_map<_Tp, _Tx, _Bound>& _map)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::map<_Tp, _Tx> _new_map;
      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      for (uint32_t i = 0; i < length; ++i)
      {
        _Tp key {};
        _Tx mapped_type {};
        if (!(_strm >> key))
        {
          return false;
        }
        if (!(_strm >> mapped_type))
        {
          return false;
        }
        _new_map.insert({std::move(key), std::move(mapped_type)});
      }

      _map.swap (_new_map);
      return true;
    }
  };

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_MAP_CDR_T_H_INCLUDED
