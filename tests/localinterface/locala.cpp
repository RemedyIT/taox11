/**
 * @file    locala.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "locala.h"
#include "testlib/taox11_testlog.h"

// Implementation interface iLocal_A. Method to test if instance is OK.
std::string iLocal_A_impl::do_local ()
{
  TAOX11_TEST_DEBUG << "Called iLocal_A_impl::do_local" << std::endl;
  return "iLocal_A_impl::do_local";
}

IDL::traits<Test::iLocal>::ref_type lia;
IDL::traits<Test::iLocal>::ref_type iLocal_A_impl::loc ()
{
  return lia;
}

void iLocal_A_impl::loc (IDL::traits<Test::iLocal>::ref_type _v)
{
  lia = _v;
}

std::string iLocal_A_impl::do_A (int32_t& result)
{
  TAOX11_TEST_DEBUG << "Called iLocal_A_impl::do_A with value " << result << std::endl;
  return "iLocal_A_impl::do_A";
}

// Implementation interface iLocal_B.
std::string iLocal_B_impl::do_localB ()
{
  TAOX11_TEST_DEBUG << "Called iLocal_B_impl::do_localB" << std::endl;
  return "iLocal_B_impl::do_local";
}

std::string iLocal_B_impl::do_B (int32_t& result)
{
  TAOX11_TEST_DEBUG << "Called iLocal_B_impl::do_B with value " << result << std::endl;
  return "iLocal_B_impl::do_B";
}


Foo_impl::Foo_impl (IDL::traits<Test::FooFactory>::ref_type factory) :
  factory_ (std::move(factory))
{
}

std::string
Foo_impl::get_foo ()
{
  return "foo";
}

IDL::traits<Test::FooFactory>::ref_type
Foo_impl::get_factory ()
{
  return factory_;
}

std::string
Foo_Factory::factory_name ()
{
  return "foofactory";
}

IDL::traits<Test::Foo>::ref_type
Foo_Factory::get_foo ()
{
  return IDL::traits<Test::Foo>::make_reference <Foo_impl> (this->_this ());
}

