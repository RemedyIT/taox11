/**
 * @file    diamond_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "diamond_i.h"

std::string
Top_i::shape ()
{
  return "top";
}

std::string
Left_i::shape ()
{
  return "left";
}

std::string
Left_i::color ()
{
  return "black";
}

std::string
Right_i::shape ()
{
  return "right";
}

int32_t
Right_i::width ()
{
  return 0;
}

std::string
Bottom_i::shape ()
{
  return "bottom";
}

std::string
Bottom_i::color ()
{
  return "translucent";
}

int32_t
Bottom_i::width ()
{
  return 100;
}

std::string
Bottom_i::name ()
{
 return "Jubilee";
}
