/**
 * @file    tc_i.cpp
 * @author  Martin Corino
 *
 * @brief   test valuetype(factory) implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tc_i.h"

std::string
tree_controller_i::base_op (const std::string& inarg)
{
  return inarg;
}

int32_t
tree_controller_i::rw_foo ()
{
  return rw_foo_;
}

void
tree_controller_i::rw_foo (int32_t a)
{
  rw_foo_ = a;
}

int32_t
tree_controller_i::r_foo ()
{
  return 1;
}

IDL::traits<CORBA::ValueBase>::ref_type
tree_controller_init::create_for_unmarshal ()
{
  return CORBA::make_reference<tree_controller_i> ();
}

