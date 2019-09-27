/**
 * @file    basic_type_traits_t.h
 * @author  Marijke Henstmengel
 *
 * @brief   Template traits useful for programming with predefined types.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#ifndef TAOX11_BASIC_TYPE_TRAITS_T_H
#define TAOX11_BASIC_TYPE_TRAITS_T_H

#pragma once

#include "tao/x11/anytypecode/typecode.h"

namespace TAOX11_NAMESPACE
{
  // Forward declaration forces specialization for each type used,
  // necessary since each instantiated struct has values that cannot
  // be set generically, so they must be set explicitly and exported.
  template<typename T>
  class BasicTypeTraits {
  };
}

#endif /* TAOX11_BASIC_TYPE_TRAITS_T_H */
