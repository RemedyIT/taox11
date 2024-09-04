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
  Shape shape;
  shape.color_seq().resize(5);

  TAOX11_TEST_INFO << "shape: " << shape << std::endl;

  if (shape.color_red() != Color::RED)
  {
    TAOX11_TEST_ERROR << "shape.color_red() not RED but: " << shape.color_red() << std::endl;
    ++retval;
  }
  for (const auto& arrayi : shape.color_red_array())
  {
    if (arrayi != Color::RED)
    {
      TAOX11_TEST_ERROR << "shape.color_red_array() not RED but: " << arrayi << std::endl;
      ++retval;
    }
  }
  for (size_t i = 0; i < shape.color_seq().size(); ++i)
  {
    if (shape.color_seq()[i] != Color::RED)
    {
      TAOX11_TEST_ERROR << "shape.color_seq()[" << i << "] not RED but: " << shape.color_seq()[i] << std::endl;
      ++retval;
    }
  }

  return retval;
}
