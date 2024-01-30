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
      using _Alloc_value_type = typename _Alloc::value_type;

      using _Vector = std::vector<_Tp, _Alloc>;

    public:
      using value_type = _Tp;
      using pointer = typename _Vector::pointer;
      using const_pointer = typename _Vector::const_pointer;
      using reference = typename _Vector::reference;
      using const_reference = typename _Vector::const_reference;
      using iterator = typename _Vector::iterator;
      using const_iterator = typename _Vector::const_iterator;
      using const_reverse_iterator = typename _Vector::const_reverse_iterator;
      using reverse_iterator = typename _Vector::reverse_iterator;
      using size_type = typename _Vector::size_type;
      using difference_type = typename _Vector::difference_type;
      using allocator_type = typename _Vector::allocator_type;

      using bound = std::integral_constant<uint32_t, _Bound>;

      bounded_vector() = default;

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
