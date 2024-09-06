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
  int retval {};
  bar mybar;
  Foo f;
  TestUnion_Octet to;

  TAOX11_TEST_INFO << "mybar: " << mybar << std::endl;
  TAOX11_TEST_INFO << "f: " << f << std::endl;
  TAOX11_TEST_INFO << "to: " << to << std::endl;
  mybar.z(6);
  TAOX11_TEST_INFO << "mybar: " << mybar << std::endl;

  return retval;
}
