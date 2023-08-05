/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"
#include "testlib/taox11_testlog.h"
#include <type_traits>

int main (int /*argc*/, char* /*argv*/[])
{
  // Just compilation test

  MyBitMask my_bitmask;
  MyBitMaskBit my_bitmaskbit;
  X11_UNUSED_ARG(my_bitmask);
  X11_UNUSED_ARG(my_bitmaskbit);

  if (!std::is_same<MyBitMaskBound8, uint8_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound8 is not uint8_t" << std::endl;
  }
  if (!std::is_same<MyBitMaskBound16, uint16_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound16 is not uint16_t" << std::endl;
  }

  return 0;
}
