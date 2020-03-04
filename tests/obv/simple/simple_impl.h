/**
 * @file    simple_impl.h
 * @author  Martin Corino
 *
 * @brief   valuetype implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef __OBV_SIMPLE_IMPL_H_INCLUDED__
#define __OBV_SIMPLE_IMPL_H_INCLUDED__

#include "simpleC.h"

class Event_impl final
  : public IDL::traits<Test::Event>::obv_type
{
public:
  Event_impl () = default;
  ~Event_impl () = default;
  Event_impl (const Event_impl&) = default;
  Event_impl (Event_impl&&) = default;

  explicit Event_impl (int32_t value_,
                       int32_t value2_,
                       std::string id_,
                       double amount)
    : Test::obv::Event (value_,value2_, id_)
    , amount_ (amount)
  {}

public:
  IDL::traits<CORBA::ValueBase>::ref_type _copy_value () const override;

  // Operations

  // generated from c++/cli_hdr/operation.erb
  /// @copybrief Test::Event::do_print
  void do_print (const std::string& loc) override;

  // Attributes

  // generated from c++/cli_hdr/attribute.erb
  double amount() override;
  void amount(double _v) override;

private:
  double amount_ {};

  Event_impl& operator= (const Event_impl&) = delete;
  Event_impl& operator= (Event_impl&&) = delete;
};

class Event_init_impl final
  : public IDL::traits<Test::Event>::factory_type
{
public:
  Event_init_impl () = default;
  ~Event_init_impl () = default;

  IDL::traits<Test::Event>::ref_type
  create_val (int32_t value) override;

  IDL::traits<CORBA::ValueBase>::ref_type create_for_unmarshal () override;

private:
  Event_init_impl (const Event_init_impl&) = delete;
  Event_init_impl (Event_init_impl&&) = delete;
  Event_init_impl& operator= (const Event_init_impl&) = delete;
  Event_init_impl& operator= (Event_init_impl&&) = delete;
}; // Event_init

#endif
