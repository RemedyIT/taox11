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
  int retval {};
  MyEnum my_bitenum;
  X11_UNUSED_ARG(my_bitenum);

  if (sizeof(uint8_t) !=  sizeof(MyEnumBound8))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnumBound8 is not uint8_t" << std::endl;
  }
  if (sizeof(uint16_t) != sizeof (MyEnumBound16))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnumBound16 is not uint16_t" << std::endl;
  }
  if (sizeof(uint32_t) != sizeof (MyEnumBound32))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnumBound32 is not uint32_t" << std::endl;
  }
  if (sizeof(uint32_t) != sizeof (MyEnum))
  {
    ++retval;
    TAOX11_TEST_ERROR << "Type of MyEnum is not uint32_t" << std::endl;
  }

  if (IDL::traits<MyEnumBound8>::bit_bound::value != 8)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnumBound8 is 8" << std::endl;
  }
  if (IDL::traits<MyEnumBound16>::bit_bound::value != 16)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnumBound16 is 16" << std::endl;
  }
  if (IDL::traits<MyEnumBound32>::bit_bound::value != 32)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnumBound32 is 32" << std::endl;
  }
  if (IDL::traits<MyEnum>::bit_bound::value != 32)
  {
    ++retval;
    TAOX11_TEST_ERROR << "bit_bound traits MyEnum is 32" << std::endl;
  }

  return retval;
}
