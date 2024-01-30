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

int main (int /*argc*/, char* /*argv*/[])
{
  MyBitset mbs1;

  // Just compilation test
  TAOX11_TEST_INFO << "mbs1: " << mbs1 << std::endl;

  return 0;
}
