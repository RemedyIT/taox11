/**
 * @file    cdr_length.h
 * @author Johnny Willemsen
 * @brief   Checked conversion of container lengths to CDR lengths
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_CDR_LENGTH_H_INCLUDED
#define TAOX11_CDR_LENGTH_H_INCLUDED

#include "tao/x11/base/tao_corba.h"

#include <cstddef>
#include <cstdint>
#include <limits>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

inline uint32_t taox11_cdr_length (std::size_t length)
{
  if (length > (std::numeric_limits<uint32_t>::max) ())
    {
      throw TAO_CORBA::BAD_PARAM ();
    }
  return static_cast<uint32_t> (length);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // TAOX11_CDR_LENGTH_H_INCLUDED
