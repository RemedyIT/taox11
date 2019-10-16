/**
 * @file    supports_test_impl.cpp
 * @author  Martin Corino
 *
 * @brief   test implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "supports_test_impl.h"
#include "testlib/taox11_testlog.h"
#include "testlib/taox11_testlog.h"

/* vt_graph_impl */

vt_graph_impl::vt_graph_impl ()
{
}

// Creates a vt_graph_impl with the given number of nodes. There will be one
// root node and the rest will be children of it.
vt_graph_impl::vt_graph_impl (int num_nodes)
{
  add_node ("ROOT");
  for (int i = 1; i < num_nodes; i++)
    {
      add_node ("CHILD");
      nodes_ ().front ()->add_edge (nodes_ ().back ());
    }
}

// Get the number of nodes in the vt_graph.
int32_t vt_graph_impl::size ()
{
  return ACE_Utils::truncate_cast<int32_t> (nodes_ ().size ());
}

// Add a node to the graph with no edges.
void
vt_graph_impl::add_node (const std::string& name)
{
  Supports_Test::Node::_ref_type new_node = CORBA::make_reference<node_impl> (name);
  nodes_ ().push_back (new_node);

  TAOX11_TEST_DEBUG << "Number of nodes: [" << nodes_ ().size () << "]" << std::endl;
  TAOX11_TEST_DEBUG << "ostream test seq of valuetypes: " << nodes_ () << std::endl;

}

// Print out information about each node.
void
vt_graph_impl::print ()
{
  TAOX11_TEST_DEBUG << "Printing graph data..." << std::endl;

  TAOX11_TEST_DEBUG << "Number of nodes: [" << nodes_ ().size () << "]\n" << std::endl;

  for (Supports_Test::Node::_ref_type n : nodes_ ())
    n->print ();
}


/* vt_graph_init_impl - factory operations */

Supports_Test::vt_graph::_ref_type
vt_graph_init_impl::create ()
{
  return CORBA::make_reference<vt_graph_impl> ();
}

CORBA::ValueBase::_ref_type
vt_graph_init_impl::create_for_unmarshal ()
{
  return CORBA::make_reference<vt_graph_impl> ();
}


/* test_impl */

test_impl::test_impl (CORBA::ORB::_ref_type orb) : orb_ (orb)
{
}

test_impl::~test_impl ()
{
}

void
test_impl::pass_obj_graph_in (
  Supports_Test::graph::_ref_type graph_param)
{
  ACE_ASSERT (graph_param->size () == 3);
  graph_param->add_node ("NEW1");
  ACE_ASSERT (graph_param->size () == 4);
}

void
test_impl::pass_vt_graph_in (
  Supports_Test::vt_graph::_ref_type vt_graph_param)
{
  ACE_ASSERT (vt_graph_param->size () == 3);
  vt_graph_param->add_node ("NEW1");
  ACE_ASSERT (vt_graph_param->size () == 4);
}

void
test_impl::pass_obj_graph_out (
  Supports_Test::graph::_ref_type& graph_param)
{
  vt_graph_impl::_ref_type vtg_srv = CORBA::make_reference<vt_graph_impl> (4);
  graph_param = vtg_srv->_this ();

  ACE_ASSERT (graph_param->size () == 4);
  graph_param->add_node ("NEW1");
  ACE_ASSERT (graph_param->size () == 5);
}

void
test_impl::pass_vt_graph_out (
    Supports_Test::vt_graph::_ref_type& vt_graph_param)
{
  vt_graph_param = CORBA::make_reference<vt_graph_impl> (4);

  ACE_ASSERT (vt_graph_param->size () == 4);
  vt_graph_param->add_node ("NEW1");
  ACE_ASSERT (vt_graph_param->size () == 5);
}

void
test_impl::pass_obj_graph_inout (
    Supports_Test::graph::_ref_type& graph_param)
{
  ACE_ASSERT (graph_param->size () == 6);
  graph_param->add_node ("NEW3");
  ACE_ASSERT (graph_param->size () == 7);
}

void
test_impl::pass_vt_graph_inout (
    Supports_Test::vt_graph::_ref_type& vt_graph_param)
{
  ACE_ASSERT (vt_graph_param->size () == 6);
  vt_graph_param->add_node ("NEW3");
  ACE_ASSERT (vt_graph_param->size () == 7);
}

void
test_impl::start ()
{
}

void
test_impl::finish ()
{
  this->orb_->shutdown (false);
}


/* node_impl */

node_impl::node_impl ()
{
}

// Initialize state.
node_impl::node_impl (const std::string& name)
{
  name_ (name);
  weight_ (0);
  degree_ (0);
}

// Add an edge from this node to neighbor.
void
node_impl::add_edge (Supports_Test::Node::_ref_type neighbor)
{
  degree_ (degree_ () + 1);
  neighbors_ ().push_back (neighbor);
}

// Remove the edge from this node to neighbor.
void
node_impl::remove_edge (Supports_Test::Node::_ref_type neighbor)
{
  for (Supports_Test::Node_List::iterator it = neighbors_ ().begin () ;
       it != neighbors_ ().end () ; ++it)
    if ((*it) == neighbor)
      {
        it = neighbors_ ().erase (it);
      }
}

void
node_impl::change_weight (int32_t new_weight)
{
  weight_ (new_weight);
}

void
node_impl::print ()
{
  TAOX11_TEST_DEBUG << "  Name: " << name_ () << std::endl;
  TAOX11_TEST_DEBUG << "    Weight: " << weight_ () << std::endl;
  TAOX11_TEST_DEBUG << "    Degree: " << degree_ () << std::endl;
  TAOX11_TEST_DEBUG << "    Neighbors: " << std::endl;
  for (Supports_Test::Node::_ref_type n : neighbors_ ())
    TAOX11_TEST_DEBUG << "      " << n->name_ () << std::endl;
}

/* node_init_impl - factory operations */

Supports_Test::Node::_ref_type
node_init_impl::create ()
{
  return CORBA::make_reference<node_impl> ();
}

CORBA::ValueBase::_ref_type
node_init_impl::create_for_unmarshal ()
{
  return CORBA::make_reference<node_impl> ();
}
