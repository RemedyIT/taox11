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
  bar_struct bs1;
  bar_class bc1;

  // Just compilation test
  TAOX11_TEST_INFO << "bs1: " << bs1 << std::endl;

  return 0;
}
