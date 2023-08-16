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

  MyBitMask my_bitmask1 = MyBitMask::flag0;

  // static_cast is necessary when we map to an `enum class`, when we map to an `enum` the static_cast
  // is not necessary but that puts the enum members into the containing namespace. The IDL4 specification
  // does put IDL enums in the containing namespace, but doesn't do that for bitmask members
  if (static_cast<bool>(my_bitmask1 & MyBitMask::flag0))
  {
    TAOX11_TEST_INFO << "Flag my_bitmask1 correctly set: " << my_bitmask1 << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Flag my_bitmask1 NOT set" << my_bitmask1 << std::endl;
  }
  my_bitmask1 = my_bitmask1 | MyBitMask::flag1;
  if (static_cast<bool>(my_bitmask1 & MyBitMask::flag0) && static_cast<bool>(my_bitmask1 & MyBitMask::flag1))
  {
    TAOX11_TEST_INFO << "Flag my_bitmask1 correctly set: " << my_bitmask1 << std::endl;
  }
  else
  {
    TAOX11_TEST_ERROR << "Flag my_bitmask1 NOT set: " << static_cast<uint16_t>(my_bitmask1) << std::endl;
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
