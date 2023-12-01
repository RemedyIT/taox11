// -*- C++ -*-
/**
 * @file   optional_t.h
 * @author Johnny Willemsen
 *
 * @brief template for IDL4 optional annotaiton
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#if !defined (__IDL__OPTIONAL_T_H_INCLUDED__)
#define __IDL__OPTIONAL_T_H_INCLUDED__

#include <optional>
#include "tao/x11/versioned_x11_namespace.h"

#if defined (ACE_HAS_CPP17)

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template<typename T>
    using optional = std::optional<T>;
  } // namespace IDL
} // namespace TAOX11_NAMESPACE

#else

namespace TAOX11_NAMESPACE
{
  namespace IDL
  {
    template<typename T>
    class optional
    {
    public:
      T value_;
    };
  }
}
#endif

#endif // __IDL__OPTIONAL_T_H_INCLUDED__
