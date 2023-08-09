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
  // Just compilation test

  Test::StringLongMap mymap;
  Test::StringLongMap mymap_2;
  Test::StringLongMap5 mymap5;

  mymap["mymap_FOO"] = 5;
  mymap5["mymap5_FOO"] = 5;
  mymap_2["mymap_FOO"] = 5;
  mymap_2["mymap_BAR"] = 4;

  TAOX11_TEST_DEBUG << "mymap: " << mymap << std::endl;
  TAOX11_TEST_DEBUG << "mymap_2: " << mymap_2 << std::endl;
  TAOX11_TEST_DEBUG << "mymap5: " << mymap5 << std::endl;

  return 0;
}
