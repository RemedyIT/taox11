/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   C++11 client application which uses IDL defines types
 *          and TAOX11
 *
 * This setup doesn't work on Windows with DLLs. The TAO CORBA typecodes do
 * require the TAOX11 Any >>= and <== support for the union, but the TAOX11
 * typecodes do require the CORBA supports which causes a cyclic dependency
 * between the shared libraries created by this test.
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

int
main(int, char* [])
{
  int result {};

  Test::Bar mybar;
  CORBA::Any a;
  a <<= mybar;

  try
  {
  }
  catch (const std::exception&)
  {
    return 1;
  }
  return result;
}
