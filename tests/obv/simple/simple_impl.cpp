/**
 * @file    simple_impl.cpp
 * @author  Martin Corino
 *
 * @brief   valuetype implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "simple_impl.h"
#include "testlib/taox11_testlog.h"

IDL::traits<CORBA::ValueBase>::ref_type Event_impl::_copy_value () const
{
  return CORBA::make_reference< Event_impl> (*this);
}

void Event_impl::do_print (const std::string& loc)
{
  TAOX11_TEST_DEBUG << "Event_impl::do_print (@" << loc << ") : "
            << "[id=" << this->id_ () << ", value=" << this->value_ () << ", value2=" << this->value2_ ()<< ", amount=" << this->amount () << "]"
            << std::endl;
}

// Attributes

// generated from c++/cli_hdr/attribute.erb
double Event_impl::amount()
{
  return this->amount_;
}

void Event_impl::amount(double _v)
{
  this->amount_ = _v;
}

IDL::traits<Test::Event>::ref_type
Event_init_impl::create_val (int32_t value)
{
  return CORBA::make_reference< Event_impl> (value, value, std::string(), 0.0);
}

IDL::traits<CORBA::ValueBase>::ref_type Event_init_impl::create_for_unmarshal ()
{
  TAOX11_TEST_DEBUG << "Event_init_impl::create_for_unmarshal() called" << std::endl;

  return CORBA::make_reference< Event_impl> ();
}
