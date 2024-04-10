/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   C++11 client application which uses IDL defines types
 *          without the full TAOX11 support
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"

// Check for ACE define

int
main(int, char* [])
{
  int result {};

  try
  {
  }
  catch (const std::exception& e)
  {
    return 1;
  }
  return result;
}
