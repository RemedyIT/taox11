/**
 * @file    supports_test_impl.h
 * @author  Martin Corino
 *
 * @brief   test implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#ifndef TAOX11_SUPPORTS_TEST_IMPL_H
#define TAOX11_SUPPORTS_TEST_IMPL_H

#include "testS.h"

/**
 * \class node_impl
 *
 *
 */
class node_impl final
  : public virtual IDL::traits<Supports_Test::Node>::obv_type
{
public:
  node_impl ();

  node_impl (const std::string& name);

  void add_edge (IDL::traits<Supports_Test::Node>::ref_type neighbor) override;

  void remove_edge (IDL::traits<Supports_Test::Node>::ref_type neighbor) override;

  void change_weight (int32_t new_weight) override;

  void print () override;
private:
  node_impl (const node_impl&) = delete;
  node_impl (node_impl&&) = delete;
  node_impl& operator= (const node_impl&) = delete;
  node_impl& operator= (node_impl&&) = delete;

};

class node_init_impl final
  : public IDL::traits<Supports_Test::Node>::factory_type
{
public:
  node_init_impl () = default;
  ~node_init_impl () = default;

  Supports_Test::Node::_ref_type create () override;

  IDL::traits<CORBA::ValueBase>::ref_type create_for_unmarshal () override;
private:
  node_init_impl (const node_init_impl&) = delete;
  node_init_impl (node_init_impl&&) = delete;
  node_init_impl& operator= (const node_init_impl&) = delete;
  node_init_impl& operator= (node_init_impl&&) = delete;
};


class vt_graph_impl final
  : public virtual IDL::traits<Supports_Test::vt_graph>::obv_type
  , public virtual CORBA::servant_traits<Supports_Test::vt_graph>::base_type
{
public:
  vt_graph_impl ();

  vt_graph_impl (int num_nodes);

  int32_t size () override;

  void add_node (const std::string& name) override;

  void print () override;
private:
  vt_graph_impl (const vt_graph_impl&) = delete;
  vt_graph_impl (vt_graph_impl&&) = delete;
  vt_graph_impl& operator= (const vt_graph_impl&) = delete;
  vt_graph_impl& operator= (vt_graph_impl&&) = delete;
};

class vt_graph_init_impl final
  : public IDL::traits<Supports_Test::vt_graph>::factory_type
{
public:
  vt_graph_init_impl () = default;
  ~vt_graph_init_impl () = default;

  Supports_Test::vt_graph::_ref_type create () override;

  CORBA::ValueBase::_ref_type create_for_unmarshal () override;

private:
  vt_graph_init_impl (const vt_graph_init_impl&) = delete;
  vt_graph_init_impl (vt_graph_init_impl&&) = delete;
  vt_graph_init_impl& operator= (const vt_graph_init_impl&) = delete;
  vt_graph_init_impl& operator= (vt_graph_init_impl&&) = delete;
};

class test_impl final
  : public virtual CORBA::servant_traits<Supports_Test::test>::base_type
{
public:
  test_impl (CORBA::ORB::_ref_type orb);

  virtual ~test_impl ();

  void pass_obj_graph_in (Supports_Test::graph::_ref_type graph_param) override;

  void pass_vt_graph_in (Supports_Test::vt_graph::_ref_type vt_graph_param) override;

  void pass_obj_graph_out (Supports_Test::graph::_ref_type& graph_param) override;

  void pass_vt_graph_out (Supports_Test::vt_graph::_ref_type& vt_graph_param) override;

  void pass_obj_graph_inout (Supports_Test::graph::_ref_type& graph_param) override;

  void pass_vt_graph_inout (Supports_Test::vt_graph::_ref_type& vt_graph_param) override;

  void start () override;

  void finish () override;

private:
  CORBA::ORB::_ref_type orb_;

  test_impl (const test_impl&) = delete;
  test_impl (test_impl&&) = delete;
  test_impl& operator= (const test_impl&) = delete;
  test_impl& operator= (test_impl&&) = delete;
};

#endif /* TAOX11_SUPPORTS_TEST_IMPL_H */
