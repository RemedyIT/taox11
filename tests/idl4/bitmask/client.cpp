/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

int main (int /*argc*/, char* /*argv*/[])
{
  // Just compilation test

  MyBitMask my_bitmask;
  MyBitMaskBit my_bitmaskbit;
  X11_UNUSED_ARG(my_bitmask);
  X11_UNUSED_ARG(my_bitmaskbit);
  return 0;
}
