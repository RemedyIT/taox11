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

  MyEnum my_bitenum;
  X11_UNUSED_ARG(my_bitenum);

  // if (!std::is_base_of<uint8_t, MyEnumBound8>::value)
  // {
  //   TAOX11_TEST_ERROR << "Type of MyEnumBound8 is not uint8_t" << std::endl;
  // }
  // if (!std::is_base_of<uint16_t, MyEnumBound16>::value)
  // {
  //   TAOX11_TEST_ERROR << "Type of MyEnumBound16 is not uint16_t" << std::endl;
  // }

  return 0;
}
