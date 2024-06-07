/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   C++11 client application which uses IDL defines types
 *          and TAOX11. This version of the test has proxy and anytypecode
 *          in one file
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

// Check for ACE define

int
main(int, char* [])
{
  int result {};

  Test::Bar mybar;

  try
  {
  }
  catch (const std::exception& e)
  {
    return 1;
  }
  return result;
}
