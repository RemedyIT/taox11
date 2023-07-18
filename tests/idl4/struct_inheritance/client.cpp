/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

int main (int /*argc*/, char* /*argv*/[])
{
  // Just compilation test
  EmptyStruct es;
  X11_UNUSED_ARG(es);
  Point2D p2d;
  X11_UNUSED_ARG(p2d);
  Point3D p3d;
  X11_UNUSED_ARG(p3d);
  return 0;
}
