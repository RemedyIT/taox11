/**
 * @file    sequence_cdr_t.h
 * @author  Martin Corino
 *
 * @brief   x11 sequence marshaling templates
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SEQUENCE_CDR_T_H_INCLUDED
#define TAOX11_SEQUENCE_CDR_T_H_INCLUDED

#include "tao/x11/tao_corba.h"
#include "tao/x11/taox11_export.h"
#include "tao/x11/basic_traits.h"
#include "tao/x11/bounded_vector_t.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

  /// Generic sequence CDR streaming helper template
  template <typename _Tp>
  struct taox11_sequence_cdr
  {
    /// Unbounded insert
    template <typename _Stream>
    static bool insert (
        _Stream& _strm,
        const std::vector<_Tp>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      for (const _Tp& _el : _seq)
      {
        if (!(_strm << _el))
        {
          return false;
        }
      }

      return true;
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<_Tp, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static bool extract (
        _Stream& _strm,
        std::vector<_Tp>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<_Tp> _new_seq (length);
      for (_Tp& _el : _new_seq)
      {
        if (!(_strm >> _el))
        {
          return false;
        }
      }

      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<_Tp, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<_Tp> _new_seq (length);
      for (_Tp& _el : _new_seq)
      {
        if (!(_strm >> _el))
        {
          return false;
        }
      }

      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Char sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<char>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<char>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_char_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<char, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<char>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<char> _new_seq (length);
      if (!_strm.read_char_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<char, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<char> _new_seq (length);
      if (!_strm.read_char_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Boolean sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<bool>
  {
    /// Unbounded insert
    template <typename _Stream>
    static bool insert (
        _Stream& _strm,
        const std::vector<bool>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      for (const bool& _el : _seq)
      {
        if (!(_strm.write_boolean (_el)))
        {
          return false;
        }
      }
      return true;
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<bool, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static bool extract (
        _Stream& _strm,
        std::vector<bool>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<bool> _new_seq (length);
      for ( std::vector <bool>::size_type i = 0; i < length; ++i)
      {
        TAO_CORBA::Boolean v;
        if (!(_strm.read_boolean (v)))
        {
          return false;
        }
        _new_seq[i] = v;
      }

      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<bool, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<bool> _new_seq (length);
      for ( std::vector <bool>::size_type i = 0; i < length; ++i)
      {
        TAO_CORBA::Boolean v;
        if (!(_strm.read_boolean (v)))
        {
          return false;
        }
        _new_seq[i] = v;
      }

      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Octet sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<uint8_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<uint8_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_octet_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<uint8_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<uint8_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<uint8_t> _new_seq (length);
      if (!_strm.read_octet_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<uint8_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<uint8_t> _new_seq (length);
      if (!_strm.read_octet_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// WChar sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<wchar_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<wchar_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_wchar_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<wchar_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<wchar_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<wchar_t> _new_seq (length);
      if (!_strm.read_wchar_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<wchar_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<wchar_t> _new_seq (length);
      if (!_strm.read_wchar_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// TinyShort sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<int8_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<int8_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_int8_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<int8_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<int8_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<int8_t> _new_seq (length);
      if (!_strm.read_int8_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<int8_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<int8_t> _new_seq (length);
      if (!_strm.read_int8_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Short sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<int16_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<int16_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_short_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<int16_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<int16_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<int16_t> _new_seq (length);
      if (!_strm.read_short_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<int16_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<int16_t> _new_seq (length);
      if (!_strm.read_short_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// UShort sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<uint16_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<uint16_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_ushort_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<uint16_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<uint16_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<uint16_t> _new_seq (length);
      if (!_strm.read_ushort_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<uint16_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<uint16_t> _new_seq (length);
      if (!_strm.read_ushort_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Long sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<int32_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<int32_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_long_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<int32_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<int32_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<int32_t> _new_seq (length);
      if (!_strm.read_long_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<int32_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<int32_t> _new_seq (length);
      if (!_strm.read_long_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// ULong sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<uint32_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<uint32_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_ulong_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<uint32_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<uint32_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<uint32_t> _new_seq (length);
      if (!_strm.read_ulong_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<uint32_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<uint32_t> _new_seq (length);
      if (!_strm.read_ulong_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// LongLong sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<int64_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<int64_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_longlong_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<int64_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<int64_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<int64_t> _new_seq (length);
      if (!_strm.read_longlong_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<int64_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<int64_t> _new_seq (length);
      if (!_strm.read_longlong_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// ULongLong sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<uint64_t>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<uint64_t>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_ulonglong_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<uint64_t, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<uint64_t>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<uint64_t> _new_seq (length);
      if (!_strm.read_ulonglong_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<uint64_t, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<uint64_t> _new_seq (length);
      if (!_strm.read_ulonglong_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Float sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<float>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<float>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_float_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<float, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<float>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<float> _new_seq (length);
      if (!_strm.read_float_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<float, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<float> _new_seq (length);
      if (!_strm.read_float_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// Double sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<double>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<double>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

      return _strm.write_double_array (_seq.data (), length);
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<double, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<double>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<double> _new_seq (length);
      if (!_strm.read_double_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<double, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<double> _new_seq (length);
      if (!_strm.read_double_array (_new_seq.data (), length))
        return false;
      _seq.swap (_new_seq);
      return true;
    }
  };

  /// LongDouble sequence CDR streaming helper template
  template <>
  struct taox11_sequence_cdr<long double>
  {
    /// Unbounded insert
    template <typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const std::vector<long double>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (!(_strm << length))
      {
        return false;
      }

#if (ACE_SIZEOF_LONG_DOUBLE == 16)
      return _strm.write_longdouble_array (_seq.data (), length);
#else
      for (const long double& _el : _seq)
      {
        if (!(_strm << _el))
        {
          return false;
        }
      }
      return true;
#endif
    }

    /// Bounded insert
    template <const uint32_t _Bound, typename _Stream>
    static inline bool insert (
        _Stream& _strm,
        const TAOX11_IDL::bounded_vector<long double, _Bound>& _seq)
    {
      uint32_t const length = ACE_Utils::truncate_cast<uint32_t> (_seq.size ());

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      return insert<_Stream> (_strm, _seq);
    }

    /// Unbounded extract
    template <typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        std::vector<long double>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      std::vector<long double> _new_seq (length);
#if (ACE_SIZEOF_LONG_DOUBLE == 16)
      if (!_strm.read_longdouble_array (_new_seq.data (), length))
        return false;
#else
      for (long double& _el : _new_seq)
      {
        if (!(_strm >> _el))
        {
          return false;
        }
      }
#endif
      _seq.swap (_new_seq);
      return true;
    }

    /// Bounded extract
    template <const uint32_t _Bound, typename _Stream>
    static inline bool extract (
        _Stream& _strm,
        TAOX11_IDL::bounded_vector<long double, _Bound>& _seq)
    {
      uint32_t length {};

      if (!(_strm >> length))
      {
        return false;
      }

      if (_Bound && length > _Bound)
      {
        throw TAO_CORBA::BAD_PARAM ();
      }

      std::vector<long double> _new_seq (length);
#if (ACE_SIZEOF_LONG_DOUBLE == 16)
      if (!_strm.read_longdouble_array (_new_seq.data (), length))
        return false;
#else
      for (long double& _el : _new_seq)
      {
        if (!(_strm >> _el))
        {
          return false;
        }
      }
#endif
      _seq.swap (_new_seq);
      return true;
    }
  };

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_SEQUENCE_CDR_T_H_INCLUDED
