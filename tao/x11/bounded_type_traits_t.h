/**
 * @file    bounded_type_traits_t.h
 * @author  Martin Corino
 *
 * @brief   Helper traits templates for IDL typedefs of possibly bounded types (string, wstring, sequence, or map)
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#ifndef TAOX11_BOUNDED_TYPE_TRAITS_T_H_INCLUDED
#define TAOX11_BOUNDED_TYPE_TRAITS_T_H_INCLUDED

#include "tao/x11/idl_traits_t.h"
#include "tao/x11/object_traits_t.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template <typename T>           // actual valuetype
    struct bounded_traits
    {
      using bound = std::integral_constant<uint32_t, T::bound::value>;
    };
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // TAOX11_BOUNDED_TYPE_TRAITS_T_H_INCLUDED
