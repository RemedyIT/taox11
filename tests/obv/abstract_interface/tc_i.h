/**
 * @file    tc_i.h
 * @author  Martin Corino
 *
 * @brief   test servant implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#ifndef TAOX11_TC_I_H
#define TAOX11_TC_I_H

#include "testC.h"

class tree_controller_i : public virtual IDL::traits<TreeController>::obv_type
{
public:
  tree_controller_i () = default;

  virtual std::string base_op (const std::string& inarg) override;

  virtual int32_t rw_foo () override;

  virtual void rw_foo (int32_t a) override;

  virtual int32_t r_foo () override;
private:
  int32_t rw_foo_ {};
};

class tree_controller_init
  : public IDL::traits<TreeController>::factory_type
{
public:
  virtual IDL::traits<CORBA::ValueBase>::ref_type
  create_for_unmarshal () override;

  tree_controller_init () = default;
  virtual ~tree_controller_init () = default;
  tree_controller_init (const tree_controller_init&) = default;
  tree_controller_init (tree_controller_init&&) = default;
}; // tree_controller_init

#endif /* TAOX11_TC_I_H */
