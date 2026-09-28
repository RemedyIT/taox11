/**
 * @file    test_cdr_length.cpp
 * @author  Johnny Willemsen
 *
 * @brief   Internal CDR length boundary test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/cdr_length.h"

#include <limits>

bool
test_cdr_length ()
{
  // Exercise the CDR limit without allocating a container larger than 4 GiB.
  const auto max_length = (std::numeric_limits<uint32_t>::max) ();
  if (TAO_VERSIONED_NAMESPACE_NAME::taox11_cdr_length (max_length) != max_length)
    return false;

  if (sizeof (std::size_t) > sizeof (uint32_t))
    {
      try
        {
          TAO_VERSIONED_NAMESPACE_NAME::taox11_cdr_length (
            static_cast<std::size_t> (max_length) + 1);
          return false;
        }
      catch (const TAO_CORBA::BAD_PARAM&)
        {
        }
      catch (...)
        {
          return false;
        }
    }

  return true;
}
