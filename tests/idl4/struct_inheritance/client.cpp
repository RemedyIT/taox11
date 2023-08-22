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
  Point2D p2d;
  TAOX11_TEST_DEBUG << "Point2D: " << p2d << std::endl;
  Point3D p3d;
  TAOX11_TEST_DEBUG << "Point3D: " << p3d << std::endl;
  Point4D p4d;
  TAOX11_TEST_DEBUG << "Point4D: " << p4d << std::endl;
  EmptyStruct es;
  TAOX11_TEST_DEBUG << "EmptyStruct: " << es << std::endl;
  DeriveEmpty de;
  TAOX11_TEST_DEBUG << "DeriveEmpty: " << de << std::endl;
  DeriveEmptyWithMember dewm;
  TAOX11_TEST_DEBUG << "DeriveEmptyWithMember: " << dewm << std::endl;
  EmptyFromDerived efd;
  TAOX11_TEST_DEBUG << "EmptyFromDerived: " << efd << std::endl;
  EmptyFromDerivedDerived efdd;
  TAOX11_TEST_DEBUG << "EmptyFromDerivedDerived: " << efdd << std::endl;
  return 0;
}
