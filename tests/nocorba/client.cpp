/**
 * @file    client.cpp
 * @author  Johnny Willemsen
 *
 * @brief   C++11 client application which uses IDL defines types
 *          without the full TAOX11 CORBA implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "testC.h"


int
main(int, char* [])
{
  int result {};

  try
  {
  }
  catch (const std::exception& e)
  {
    std::cout << "exception caught: " << e.what() << std::endl;
    return 1;
  }
  return result;
}
