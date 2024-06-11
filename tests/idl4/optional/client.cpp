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

  TAOX11_TEST_INFO << "mybar: " << mybar << std::endl;
  mybar.z(6);
  TAOX11_TEST_INFO << "mybar: " << mybar << std::endl;

  return retval;
}
