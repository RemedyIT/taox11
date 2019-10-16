/**
 * @file    diamond_i.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Collocation Test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "diamond_i.h"

Top_i::Top_i ()
{
  // no-op.
}

Top_i::~Top_i ()
{
  // No-op.
}

std::string
Top_i::shape ()
{
  return "a top point";
}

Left_i::Left_i ()
{
  // No-op.
}

Left_i::~Left_i ()
{
  // No-op.
}

std::string
Left_i::shape ()
{
  return "the left line";
}

std::string
Left_i::color ()
{
  return "black";
}

Right_i::Right_i ()
{
  // no-op.
}

Right_i::~Right_i ()
{
  // no-op.
}

std::string
Right_i::shape ()
{
  return "the right line";
}

std::string
Right_i::color ()
{
  return "red";
  //
}

int32_t
Right_i::width ()
{
  return 0;
}

Bottom_i::Bottom_i ()
{
  // no-op.
}

Bottom_i::~Bottom_i ()
{
  // no-op.
}

std::string
Bottom_i::shape ()
{
  return "a bottom diamond";
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
