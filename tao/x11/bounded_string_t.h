// -*- C++ -*-
/**
 * @file  bounded_string_t.h
 * @author Martin Corino
 *
 * @brief template derivative of std::basic_string for bounded strings
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (__IDL_BOUNDED_STRING_T_H_INCLUDED__)
#define __IDL_BOUNDED_STRING_T_H_INCLUDED__

#include <string>
#include "tao/x11/versioned_x11_namespace.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template<typename _CharT, const uint32_t _Bound,
             typename _Traits = std::char_traits<_CharT>,
             typename _Alloc = std::allocator<_CharT> >
    class bounded_basic_string
      : public std::basic_string<_CharT, _Traits, _Alloc>
    {
      using _String = std::basic_string<_CharT, _Traits, _Alloc>;

    public:
      using traits_type = _Traits;
      using value_type = typename _Traits::char_type;
      using allocator_type = _Alloc;
      using size_type = typename _String::size_type;
      using difference_type = typename _String::difference_type;
      using reference = typename _String::reference;
      using const_reference = typename _String::const_reference;
      using pointer = typename _String::pointer;
      using const_pointer = typename _String::const_pointer;
      using iterator = typename _String::iterator;
      using const_iterator = typename _String::const_iterator;
      using const_reverse_iterator = typename _String::const_reverse_iterator;
      using reverse_iterator = typename _String::reverse_iterator;

      using bound = std::integral_constant<uint32_t, _Bound>;

      bounded_basic_string() { }

      explicit
      bounded_basic_string(const allocator_type& __a)
        : _String(__a) { }

      bounded_basic_string(const bounded_basic_string& __x)
        : _String(__x) { }

      bounded_basic_string(const _String& __x)
        : _String(__x) { }

      bounded_basic_string(const bounded_basic_string& __x, size_type __pos,
                            size_type __n = _String::npos)
        : _String(__x, __pos, __n) { }

      bounded_basic_string(const _String& __x, size_type __pos,
                            size_type __n = _String::npos)
        : _String(__x, __pos, __n) { }

      bounded_basic_string(const bounded_basic_string& __x, size_type __pos,
                            size_type __n, const _Alloc& __a)
        : _String(__x, __pos, __n, __a) { }

      bounded_basic_string(const _String& __x, size_type __pos,
                            size_type __n, const _Alloc& __a)
        : _String(__x, __pos, __n, __a) { }

      bounded_basic_string(const _CharT* __s,
                            size_type __n,
                            const _Alloc& __a = _Alloc ())
        : _String(__s, __n, __a) { }

      bounded_basic_string(const _CharT* __s,
                            const _Alloc& __a = _Alloc ())
        : _String(__s, __a) { }

      bounded_basic_string(size_type __n, _CharT __c,
                            const _Alloc& __a = _Alloc ())
        : _String(__n, __c, __a) { }

      bounded_basic_string(bounded_basic_string&& __x)
        : _String(std::move(__x)) { }

      bounded_basic_string(_String&& __x)
        : _String(std::move(__x)) { }

      bounded_basic_string(std::initializer_list<_CharT> __l,
              const _Alloc& __a = _Alloc())
      : _String(__l, __a) { }

      template<typename _InputIterator>
        bounded_basic_string(_InputIterator __beg, _InputIterator __end,
                const _Alloc& __a = _Alloc())
          : _String(__beg, __end, __a) { }

      ~bounded_basic_string () = default;

      bounded_basic_string&
      operator=(const _String& __x)
      { this->_String::operator = (__x); return *this; }

      bounded_basic_string&
      operator=(_String&& __x)
      { this->_String::operator = (std::move (__x)); return *this; }

      bounded_basic_string&
      operator=(const bounded_basic_string& __x)
      { this->_String::operator = (__x); return *this; }

      bounded_basic_string&
      operator=(bounded_basic_string&& __x)
      { this->_String::operator = (std::move (__x)); return *this; }

      bounded_basic_string&
      operator=(std::initializer_list<_CharT> __l)
      { this->_String::operator = (__l); return *this; }

      bounded_basic_string&
      operator+=(const bounded_basic_string& __str)
      { this->_String::operator+= (__str); return *this; }

      bounded_basic_string&
      operator+=(const _String& __str)
      { this->_String::operator+= (__str); return *this; }

      bounded_basic_string&
      operator+=(const _CharT* __s)
      { this->_String::operator+= (__s); return *this; }

      bounded_basic_string&
      operator+=(_CharT __c)
      { this->_String::operator+= (__c); return *this; }

      bounded_basic_string&
      operator+=(std::initializer_list<_CharT> __l)
      { this->_String::operator+= (__l); return *this; }

      bounded_basic_string&
      append(const bounded_basic_string& __str)
      { this->_String::append (__str); return *this; }

      bounded_basic_string&
      append(const bounded_basic_string& __str, size_type __pos, size_type __n)
      { this->_String::append (__str, __pos, __n); return *this; }

      bounded_basic_string&
      append(const _String& __str)
      { this->_String::append (__str); return *this; }

      bounded_basic_string&
      append(const _String& __str, size_type __pos, size_type __n)
      { this->_String::append (__str, __pos, __n); return *this; }

      bounded_basic_string&
      append(const _CharT* __s, size_type __n)
      { this->_String::append (__s, __n); return *this; }

      bounded_basic_string&
      append(const _CharT* __s)
      { this->_String::append (__s); return *this; }

      bounded_basic_string&
      append(size_type __n, _CharT __c)
      { this->_String::append (__n, __c); return *this; }

      template<class _InputIterator>
        bounded_basic_string&
        append(_InputIterator __first, _InputIterator __last)
        { return this->_String::append(__first, __last); }

      bounded_basic_string&
      append(std::initializer_list<_CharT> __l)
      { return this->append(__l.begin(), __l.size()); }

      bounded_basic_string&
      assign(const bounded_basic_string& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(bounded_basic_string&& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(const bounded_basic_string& __str, size_type __pos, size_type __n)
      { this->_String::assign (__str, __pos, __n); return *this; }


      bounded_basic_string&
      assign(const _String& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(_String&& __str)
      { this->_String::assign (__str); return *this; }

      bounded_basic_string&
      assign(const _String& __str, size_type __pos, size_type __n)
      { this->_String::assign (__str, __pos, __n); return *this; }

      bounded_basic_string&
      assign(const _CharT* __s, size_type __n)
      { this->_String::assign (__s, __n); return *this; }

      bounded_basic_string&
      assign(const _CharT* __s)
      { this->_String::assign (__s); return *this; }

      bounded_basic_string&
      assign(size_type __n, _CharT __c)
      { this->_String::assign (__n, __c); return *this; }

      template<class _InputIterator>
        bounded_basic_string&
        assign(_InputIterator __first, _InputIterator __last)
        { this->_String::assign (__first, __last); return *this; }

      bounded_basic_string&
      assign(std::initializer_list<_CharT> __l)
      { return this->assign(__l.begin(), __l.size()); }

      bounded_basic_string&
      insert(size_type __pos1, const bounded_basic_string& __str)
      { this->_String::insert(__pos1, __str); return *this; }

      bounded_basic_string&
      insert(size_type __pos1, const bounded_basic_string& __str,
              size_type __pos2, size_type __n)
      { this->_String::insert(__pos1, __str, __pos2, __n); return *this; }

      bounded_basic_string&
      insert(size_type __pos1, const _String& __str)
      { this->_String::insert(__pos1, __str); return *this; }

      bounded_basic_string&
      insert(size_type __pos1, const _String& __str,
              size_type __pos2, size_type __n)
      { this->_String::insert(__pos1, __str, __pos2, __n); return *this; }

      bounded_basic_string&
      insert(size_type __pos, const _CharT* __s, size_type __n)
      { this->_String::insert(__pos, __s, __n); return *this; }

      bounded_basic_string&
      insert(size_type __pos, const _CharT* __s)
      { this->_String::insert(__pos, __s); return *this; }

      bounded_basic_string&
      insert(size_type __pos, size_type __n, _CharT __c)
      { this->_String::insert(__pos, __n, __c); return *this; }

      bounded_basic_string&
      erase(size_type __pos = 0, size_type __n = _String::npos)
      { this->_String::erase (__pos, __n); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n, const bounded_basic_string& __str)
      { this->_String::replace(__pos, __n, __str); return *this; }

      bounded_basic_string&
      replace(size_type __pos1, size_type __n1, const bounded_basic_string& __str,
              size_type __pos2, size_type __n2)
      { this->_String::replace(__pos1, __n1, __str, __pos2, __n2); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n, const _String& __str)
      { this->_String::replace(__pos, __n, __str); return *this; }

      bounded_basic_string&
      replace(size_type __pos1, size_type __n1, const _String& __str,
              size_type __pos2, size_type __n2)
      { this->_String::replace(__pos1, __n1, __str, __pos2, __n2); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s,
              size_type __n2)
      { this->_String::replace(__pos, __n1, __s, __n2); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n1, const _CharT* __s)
      { this->_String::replace(__pos, __n1, __s); return *this; }

      bounded_basic_string&
      replace(size_type __pos, size_type __n1, size_type __n2, _CharT __c)
      { this->_String::replace(__pos, __n1, __n2, __c); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const bounded_basic_string& __str)
      { this->_String::replace(__i1, __i2, __str); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const _String& __str)
      { this->_String::replace(__i1, __i2, __str); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s, size_type __n)
      { this->_String::replace(__i1, __i2, __s, __n); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, const _CharT* __s)
      { this->_String::replace(__i1, __i2, __s); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, size_type __n, _CharT __c)
      { this->_String::replace(__i1, __i2, __n, __c); return *this; }

      template<class _InputIterator>
        bounded_basic_string&
        replace(iterator __i1, iterator __i2,
                _InputIterator __k1, _InputIterator __k2)
        { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, _CharT* __k1, _CharT* __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2,
              const _CharT* __k1, const _CharT* __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2, iterator __k1, iterator __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string&
      replace(iterator __i1, iterator __i2,
              const_iterator __k1, const_iterator __k2)
      { this->_String::replace(__i1, __i2, __k1, __k2); return *this; }

      bounded_basic_string& replace(iterator __i1, iterator __i2,
                            std::initializer_list<_CharT> __l)
      { this->_String::replace(__i1, __i2, __l); return *this; }

      void
      swap(bounded_basic_string& __x)
      { this->_String::swap (__x); }

      void
      swap(_String& __x)
      { this->_String::swap (__x); }
    };

    template <const uint32_t _Bound>
      using bounded_string = bounded_basic_string<char, _Bound>;

    template <const uint32_t _Bound>
      using bounded_wstring = bounded_basic_string<wchar_t, _Bound>;

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(const bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __lhs,
                const std::basic_string<_CharT, _Traits, _Alloc>& __rhs)
      {
        bounded_basic_string<_CharT, _Bound, _Traits, _Alloc> __str(__lhs);
        __str.append(__rhs);
        return __str;
      }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      bounded_basic_string<_CharT, _Bound,_Traits,_Alloc>
      operator+(const _CharT* __lhs,
                const bounded_basic_string<_CharT, _Bound,_Traits,_Alloc>& __rhs)
    {
      return bounded_basic_string<_CharT, _Bound,_Traits,_Alloc> (
              __lhs + static_cast<const std::basic_string<_CharT, _Traits,_Alloc>&> (__rhs));
    }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      bounded_basic_string<_CharT, _Bound,_Traits,_Alloc>
      operator+(_CharT __lhs, const bounded_basic_string<_CharT, _Bound,_Traits,_Alloc>& __rhs)
    {
      return bounded_basic_string<_CharT, _Bound,_Traits,_Alloc> (
              __lhs + static_cast<const std::basic_string<_CharT, _Traits,_Alloc>&> (__rhs));
    }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(const bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __lhs,
                const _CharT* __rhs)
      {
        bounded_basic_string<_CharT, _Bound, _Traits, _Alloc> __str(__lhs);
        __str.append(__rhs);
        return __str;
      }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(const bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __lhs, _CharT __rhs)
      {
        using __string_type = bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>;
        using __size_type = typename __string_type::size_type;
        __string_type __str(__lhs);
        __str.append(__size_type(1), __rhs);
        return __str;
      }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>&& __lhs,
                const std::basic_string<_CharT, _Traits, _Alloc>& __rhs)
      { return std::move(__lhs.append(__rhs)); }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(const bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __lhs,
                std::basic_string<_CharT, _Traits, _Alloc>&& __rhs)
      { return std::move(__rhs.insert(0, __lhs)); }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>&& __lhs,
                std::basic_string<_CharT, _Traits, _Alloc>&& __rhs)
      {
        const auto __size = __lhs.size() + __rhs.size();
        const bool __cond = (__size > __lhs.capacity()
                             && __size <= __rhs.capacity());
        return __cond ? std::move(__rhs.insert(0, __lhs))
                      : std::move(__lhs.append(__rhs));
      }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(const _CharT* __lhs,
                bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>&& __rhs)
      { return std::move(__rhs.insert(0, __lhs)); }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(_CharT __lhs,
                bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>&& __rhs)
      { return std::move(__rhs.insert(0, 1, __lhs)); }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>&& __lhs,
                const _CharT* __rhs)
      { return std::move(__lhs.append(__rhs)); }

    template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
      inline bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>
      operator+(bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>&& __lhs,
                _CharT __rhs)
      { return std::move(__lhs.append(1, __rhs)); }

  } // namespace IDL
} // namespace TAOX11_NAMESPACE

namespace std
{
  /// See IDL::bounded_basic_string::swap().
  template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
    inline void
    swap(TAOX11_NAMESPACE::IDL::bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __x,
        TAOX11_NAMESPACE::IDL::bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
    inline void
    swap(TAOX11_NAMESPACE::IDL::bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __x,
         basic_string<_CharT, _Traits, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _CharT, const uint32_t _Bound, typename _Traits, typename _Alloc>
    inline void
    swap(basic_string<_CharT, _Traits, _Alloc>& __x,
        TAOX11_NAMESPACE::IDL::bounded_basic_string<_CharT, _Bound, _Traits, _Alloc>& __y)
    { __x.swap(__y); }
} // namespace std

#endif // __IDL_BOUNDED_STRING_T_H_INCLUDED__
