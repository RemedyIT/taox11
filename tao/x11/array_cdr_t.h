/**
 * @file    array_cdr_t.h
 * @author  Martin Corino
 *
 * @brief   x11 array marshal/unmarshal helper templates
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_ARRAY_CDR_T_H_INCLUDED
#define TAOX11_ARRAY_CDR_T_H_INCLUDED

#include <array>
#include <iostream>
#include "tao/x11/tao_corba.h"
#include "tao/x11/taox11_export.h"
#include "tao/x11/basic_traits.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

  /// Generic nested array helper template
  template <const uint32_t _Dim, typename _Tp, const std::size_t _Nm>
  struct taox11_array_cdr
  {
    template <typename _Stream>
    static bool insert (
        _Stream& strm ,
        const std::array<_Tp, _Nm>& _v)
    {
      for (const _Tp& _ve : _v)
        {
          if (!taox11_array_cdr<_Dim-1,
                                typename _Tp::value_type,
                                std::tuple_size<_Tp>::value>::insert (strm, _ve))
          {
            return false;
          }
        }
      return true;
    }

    template <typename _Stream>
    static bool extract (
        _Stream& strm ,
        std::array<_Tp, _Nm>& _v)
    {
      for (_Tp& _ve : _v)
        {
          if (!taox11_array_cdr<_Dim-1,
                                typename _Tp::value_type,
                                std::tuple_size<_Tp>::value>::extract (strm, _ve))
          {
            return false;
          }
        }
      return true;
    }
  };

  /// Generic array helper template
  template <typename _Tp, const std::size_t _Nm>
  struct taox11_array_cdr<0U, _Tp, _Nm>
  {
    template <typename _Stream>
    static bool insert (
        _Stream& strm ,
        const std::array<_Tp, _Nm>& _v)
    {
      for (const _Tp& _ve : _v)
        {
          if (!(strm << _ve))
            return false;
        }
      return true;
    }

    template <typename _Stream>
    static bool extract (
        _Stream& strm ,
        std::array<_Tp, _Nm>& _v)
    {
      for (_Tp& _ve : _v)
        {
          if (!(strm >> _ve))
            return false;
        }
      return true;
    }
  };

  /// Boolean array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, bool, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<bool, _Nm>& _v)
    {
      return strm.write_boolean_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<bool, _Nm>& _v)
    {
      return strm.read_boolean_array (_v.data (), _Nm);
    }
  };

  /// Char array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, char, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<char, _Nm>& _v)
    {
      return strm.write_char_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<char, _Nm>& _v)
    {
      return strm.read_char_array (_v.data (), _Nm);
    }
  };

  /// WChar array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, wchar_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<wchar_t, _Nm>& _v)
    {
      return strm.write_wchar_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<wchar_t, _Nm>& _v)
    {
      return strm.read_wchar_array (_v.data (), _Nm);
    }
  };

  /// Octet array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, uint8_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<uint8_t, _Nm>& _v)
    {
      return strm.write_octet_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<uint8_t, _Nm>& _v)
    {
      return strm.read_octet_array (_v.data (), _Nm);
    }
  };

  /// TinyShort array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, int8_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<int8_t, _Nm>& _v)
    {
      return strm.write_int8_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<int8_t, _Nm>& _v)
    {
      return strm.read_int8_array (_v.data (), _Nm);
    }
  };

  /// Short array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, int16_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<int16_t, _Nm>& _v)
    {
      return strm.write_short_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<int16_t, _Nm>& _v)
    {
      return strm.read_short_array (_v.data (), _Nm);
    }
  };

  /// UShort array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, uint16_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<uint16_t, _Nm>& _v)
    {
      return strm.write_ushort_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<uint16_t, _Nm>& _v)
    {
      return strm.read_ushort_array (_v.data (), _Nm);
    }
  };

  /// Long array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, int32_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<int32_t, _Nm>& _v)
    {
      return strm.write_long_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<int32_t, _Nm>& _v)
    {
      return strm.read_long_array (_v.data (), _Nm);
    }
  };

  /// ULong array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, uint32_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<uint32_t, _Nm>& _v)
    {
      return strm.write_ulong_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<uint32_t, _Nm>& _v)
    {
      return strm.read_ulong_array (_v.data (), _Nm);
    }
  };

  /// LongLong array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, int64_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<int64_t, _Nm>& _v)
    {
      return strm.write_longlong_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<int64_t, _Nm>& _v)
    {
      return strm.read_longlong_array (_v.data (), _Nm);
    }
  };

  /// ULongLong array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, uint64_t, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<uint64_t, _Nm>& _v)
    {
      return strm.write_ulonglong_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<uint64_t, _Nm>& _v)
    {
      return strm.read_ulonglong_array (_v.data (), _Nm);
    }
  };

  /// Float array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, float, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<float, _Nm>& _v)
    {
      return strm.write_float_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<float, _Nm>& _v)
    {
      return strm.read_float_array (_v.data (), _Nm);
    }
  };

  /// Double array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, double, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<double, _Nm>& _v)
    {
      return strm.write_double_array (_v.data (), _Nm);
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<double, _Nm>& _v)
    {
      return strm.read_double_array (_v.data (), _Nm);
    }
  };

  /// LongDouble array specialization
  template <const std::size_t _Nm>
  struct taox11_array_cdr<0U, long double, _Nm>
  {
    template <typename _Stream>
    static inline bool insert (
        _Stream& strm ,
        const std::array<long double, _Nm>& _v)
    {
#if (ACE_SIZEOF_LONG_DOUBLE == 16)
      return strm.write_longdouble_array (_v.data (), _Nm);
#else
      for (const long double& _ve : _v)
        {
          if (!(strm << _ve))
            return false;
        }
      return true;
#endif
    }

    template <typename _Stream>
    static inline bool extract (
        _Stream& strm ,
        std::array<long double, _Nm>& _v)
    {
#if (ACE_SIZEOF_LONG_DOUBLE == 16)
      return strm.read_longdouble_array (_v.data (), _Nm);
#else
      for (long double& _ve : _v)
        {
          if (!(strm >> _ve))
            return false;
        }
      return true;
#endif
    }
  };

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_SEQUENCE_CDR_T_H_INCLUDED
