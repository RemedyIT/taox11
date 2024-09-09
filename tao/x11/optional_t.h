// -*- C++ -*-
/**
 * @file   optional_t.h
 * @author Johnny Willemsen
 *
 * @brief template for IDL4 optional annotation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (__IDL__OPTIONAL_T_H_INCLUDED__)
#define __IDL__OPTIONAL_T_H_INCLUDED__

#include <optional>
#include "tao/x11/base/versioned_x11_namespace.h"

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template<typename T>
    using optional = std::optional<T>;

    template<typename T>
    std::ostream& operator <<(std::ostream& stream, const optional<T>& optional)
    {
      if (optional.has_value())
      {
        stream << IDL::traits<T>::write(optional.value ());
      }
      else
      {
        stream << "std::nullopt";
      }
      return stream;
    }
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#endif // __IDL__OPTIONAL_T_H_INCLUDED__
