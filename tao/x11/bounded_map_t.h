// -*- C++ -*-
/**
 * @file   bounded_map_t.h
 * @author Johnny Willemsen
 *
 * @brief template derivative of std::map for bounded maps
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (__IDL__BOUNDED_MAP_T_H_INCLUDED__)
#define __IDL__BOUNDED_MAP_T_H_INCLUDED__

#include <map>
#include <initializer_list>
#include "tao/x11/versioned_x11_namespace.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template<typename _Keyp, typename _Tp, const uint32_t _Bound, typename _Comparep = std::less<_Keyp>, typename _Alloc = std::allocator<std::pair<const _Keyp, _Tp>> >
    class bounded_map
      : public std::map<_Keyp, _Tp, _Comparep, _Alloc>
    {
      // Concept requirements.
      using _Alloc_value_type = typename _Alloc::value_type;

      using _Map = std::map<_Keyp, _Tp, _Comparep, _Alloc>;

    public:
      using key_type = _Keyp;
      using mapped_type = _Tp;
      using value_type = std::pair<const _Keyp, _Tp>;
      using key_compare = _Comparep;
      using pointer = typename _Map::pointer;
      using const_pointer = typename _Map::const_pointer;
      using reference = typename _Map::reference;
      using const_reference = typename _Map::const_reference;
      using iterator = typename _Map::iterator;
      using const_iterator = typename _Map::const_iterator;
      using const_reverse_iterator = typename _Map::const_reverse_iterator;
      using reverse_iterator = typename _Map::reverse_iterator;
      using size_type = typename _Map::size_type;
      using difference_type = typename _Map::difference_type;
      using allocator_type = typename _Map::allocator_type;
#if defined (ACE_HAS_CPP17)
      using node_type = typename _Map::node_type;
      using insert_return_type = typename _Map::insert_return_type;
#endif /* ACE_HAS_CPP17 */

      using bound = std::integral_constant<uint32_t, _Bound>;

      bounded_map() = default;

      explicit
      bounded_map(const allocator_type& __a)
       : _Map(__a) { }

      explicit
      bounded_map(const _Comparep& __comp, const allocator_type& __a)
       : _Map(__comp, __a) {}

      bounded_map(const bounded_map& __x)
       : _Map(__x) { }

      bounded_map(bounded_map&& __x)
       : _Map(std::move(__x)) { }

      bounded_map(const _Map& __x)
       : _Map(__x) { }

      bounded_map(_Map&& __x)
       : _Map(std::move(__x)) { }

      bounded_map(bounded_map&& __rv, const allocator_type& __m)
       : _Map(std::move(__rv), __m) { }

      bounded_map(std::initializer_list<value_type> __l,
             const allocator_type& __a = allocator_type())
       : _Map(__l, __a) { }

      template<typename _InputIterator>
      bounded_map(_InputIterator __first, _InputIterator __last,
               const allocator_type& __a = allocator_type())
       : _Map(__first, __last, __a) { }

      ~bounded_map () = default;

      bounded_map&
      operator=(const _Map& __x)
      { this->_Map::operator = (__x); return *this; }

      bounded_map&
      operator=(_Map&& __x)
      { this->_Map::operator = (std::move (__x)); return *this; }

      bounded_map&
      operator=(const bounded_map& __x)
      { this->_Map::operator = (__x); return *this; }

      bounded_map&
      operator=(bounded_map&& __x)
      { this->_Map::operator = (std::move (__x)); return *this; }

      bounded_map&
      operator=(std::initializer_list<value_type> __l)
      { this->_Map::operator = (__l); return *this; }

      void
      swap(bounded_map& __x)
      { this->_Map::swap (__x); }

      void
      swap(_Map& __x)
      { this->_Map::swap (__x); }
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

namespace std
{
  /// See IDL::bounded_map::swap().
  template<typename _Keyp, typename _Tp, const uint32_t _Bound, typename _Comparep, typename _Alloc>
    inline void
    swap(TAOX11_NAMESPACE::IDL::bounded_map<_Keyp, _Tp, _Bound, _Comparep, _Alloc>& __x,
         TAOX11_NAMESPACE::IDL::bounded_map<_Keyp, _Tp, _Bound, _Comparep, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _Keyp, typename _Tp, const uint32_t _Bound, typename _Comparep, typename _Alloc>
    inline void
    swap(TAOX11_NAMESPACE::IDL::bounded_map<_Keyp, _Tp, _Bound, _Comparep, _Alloc>& __x,
         map<_Keyp, _Tp, _Comparep, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _Keyp, typename _Tp, const uint32_t _Bound, typename _Comparep, typename _Alloc>
    inline void
    swap(map<_Keyp, _Tp, _Comparep, _Alloc>& __x,
         TAOX11_NAMESPACE::IDL::bounded_map<_Keyp, _Tp, _Bound, _Comparep, _Alloc>& __y)
    { __x.swap(__y); }
} // namespace std

#endif // __IDL__BOUNDED_MAP_T_H_INCLUDED__
