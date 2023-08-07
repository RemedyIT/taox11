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

  MyBitMask my_bitmask = MyBitMask::flag0;

  if (static_cast<bool>(my_bitmask & MyBitMask::flag0))
  {
    TAOX11_TEST_INFO << "Flag my_bitmask correctly set" << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Flag my_bitmask NOT set" << std::endl;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound8>, uint8_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound8 is not uint8_t" << std::endl;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound16>, uint16_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound16 is not uint16_t" << std::endl;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound32>, uint32_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound32 is not uint32_t" << std::endl;
  }
  if (!std::is_same<std::underlying_type_t<MyBitMaskBound64>, uint64_t>::value)
  {
    TAOX11_TEST_ERROR << "Type of MyBitMaskBound64 is not uint64_t" << std::endl;
  }

  return 0;
}
