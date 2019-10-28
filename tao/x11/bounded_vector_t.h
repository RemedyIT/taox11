// -*- C++ -*-
/**
 * @file   bounded_vector_t.h
 * @author Martin Corino
 *
 * @brief template derivative of std::vector for bounded sequences
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (__IDL__BOUNDED_VECTOR_T_H_INCLUDED__)
#define __IDL__BOUNDED_VECTOR_T_H_INCLUDED__

#include <vector>
#include <initializer_list>
#include "tao/x11/versioned_x11_namespace.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template<typename _Tp, const uint32_t _Bound, typename _Alloc = std::allocator<_Tp> >
    class bounded_vector
      : public std::vector<_Tp, _Alloc>
    {
      // Concept requirements.
      typedef typename _Alloc::value_type                _Alloc_value_type;

      typedef std::vector<_Tp, _Alloc>                   _Vector;

    public:
      typedef _Tp                                        value_type;
      typedef typename _Vector::pointer                  pointer;
      typedef typename _Vector::const_pointer            const_pointer;
      typedef typename _Vector::reference                reference;
      typedef typename _Vector::const_reference          const_reference;
      typedef typename _Vector::iterator                 iterator;
      typedef typename _Vector::const_iterator           const_iterator;
      typedef std::reverse_iterator<const_iterator>      const_reverse_iterator;
      typedef std::reverse_iterator<iterator>            reverse_iterator;
      typedef size_t                                     size_type;
      typedef std::ptrdiff_t                             difference_type;
      typedef _Alloc                                     allocator_type;

      typedef std::integral_constant<uint32_t, _Bound>   bound;

      bounded_vector() { }

      explicit
      bounded_vector(const allocator_type& __a)
       : _Vector(__a) { }

      explicit
      bounded_vector(size_type __n)
       : _Vector(__n) {}

      bounded_vector(size_type __n, const value_type& _value,
             const allocator_type& __a = allocator_type())
       : _Vector(__n, _value, __a) { }

      bounded_vector(const bounded_vector& __x)
       : _Vector(__x) { }

      bounded_vector(bounded_vector&& __x)
       : _Vector(std::move(__x)) { }

      bounded_vector(const _Vector& __x)
       : _Vector(__x) { }

      bounded_vector(_Vector&& __x)
       : _Vector(std::move(__x)) { }

      bounded_vector(const bounded_vector& __x, const allocator_type& __a)
       : _Vector(__x, __a) { }

      bounded_vector(bounded_vector&& __rv, const allocator_type& __m)
       : _Vector(std::move(__rv), __m) { }

      bounded_vector(std::initializer_list<value_type> __l,
             const allocator_type& __a = allocator_type())
       : _Vector(__l, __a) { }

      template<typename _InputIterator>
      bounded_vector(_InputIterator __first, _InputIterator __last,
               const allocator_type& __a = allocator_type())
       : _Vector(__first, __last, __a) { }

      ~bounded_vector () = default;

      bounded_vector&
      operator=(const _Vector& __x)
      { this->_Vector::operator = (__x); return *this; }

      bounded_vector&
      operator=(_Vector&& __x)
      { this->_Vector::operator = (std::move (__x)); return *this; }

      bounded_vector&
      operator=(const bounded_vector& __x)
      { this->_Vector::operator = (__x); return *this; }

      bounded_vector&
      operator=(bounded_vector&& __x)
      { this->_Vector::operator = (std::move (__x)); return *this; }

      bounded_vector&
      operator=(std::initializer_list<value_type> __l)
      { this->_Vector::operator = (__l); return *this; }

      void
      swap(bounded_vector& __x)
      { this->_Vector::swap (__x); }

      void
      swap(_Vector& __x)
      { this->_Vector::swap (__x); }
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

namespace std
{
  /// See IDL::bounded_vector::swap().
  template<typename _Tp, const uint32_t _Bound, typename _Alloc>
    inline void
    swap(TAOX11_NAMESPACE::IDL::bounded_vector<_Tp, _Bound, _Alloc>& __x,
         TAOX11_NAMESPACE::IDL::bounded_vector<_Tp, _Bound, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _Tp, const uint32_t _Bound, typename _Alloc>
    inline void
    swap(TAOX11_NAMESPACE::IDL::bounded_vector<_Tp, _Bound, _Alloc>& __x,
         vector<_Tp, _Alloc>& __y)
    { __x.swap(__y); }

  template<typename _Tp, const uint32_t _Bound, typename _Alloc>
    inline void
    swap(vector<_Tp, _Alloc>& __x,
         TAOX11_NAMESPACE::IDL::bounded_vector<_Tp, _Bound, _Alloc>& __y)
    { __x.swap(__y); }
} // namespace std

#endif // __IDL__BOUNDED_VECTOR_T_H_INCLUDED__
