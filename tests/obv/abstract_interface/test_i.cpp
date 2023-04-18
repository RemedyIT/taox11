/**
 * @file    test_i.cpp
 * @author  Martin Corino
 *
 * @brief   test servant implementations
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_i.h"

std::string
foo_i::foo_op (const std::string& inarg)
{
  std::string retval ("bad");

  if (inarg == "foo_op")
    {
      retval = "good";
    }
  else
    {
      throw BadInput ("expected \"foo_op\"\n");
    }

  return retval;
}

std::string
foo_i::base_op (const std::string& inarg)
{
  std::string retval ("bad");

  if (inarg == "base_op")
    {
      retval = "good";
    }
  else
    {
      throw BadInput ("expected \"base_op\"\n");
    }

  return retval;
}

int32_t
foo_i::rw_foo ()
{
  return rw_foo_;
}

void
foo_i::rw_foo (int32_t a)
{
  rw_foo_ = a;
}

int32_t
foo_i::r_foo ()
{
  return 1;
}

passer_i::passer_i (IDL::traits<CORBA::ORB>::ref_type orb, IDL::traits<PortableServer::POA>::ref_type poa)
 : orb_ (std::move (orb))
 , poa_ (std::move (poa))
{
}

void
passer_i::shutdown ()
{
  this->orb_->shutdown (false);
}

void
passer_i::pass_ops (IDL::traits<base>::ref_type& outarg)
{
  foo_i::_ref_type servant = CORBA::make_reference<foo_i> ();

  PortableServer::ObjectId id =
    this->poa_->activate_object (servant);

  IDL::traits<CORBA::Object>::ref_type object = this->poa_->id_to_reference (id);

  outarg = IDL::traits<foo>::narrow (object);
}

void
passer_i::pass_state (IDL::traits<base>::ref_type& outarg)
{
  IDL::traits<TreeController>::ref_type tc =
      CORBA::make_reference<tree_controller_i> ();

  // Create the root node.
  IDL::traits<StringNode>::ref_type sn =
      CORBA::make_reference<IDL::traits<StringNode>::obv_type> ();
  sn->name ("RootNode");
  tc->root (sn);

  // Create the left leaf.
  IDL::traits<StringNode>::ref_type l_dummy =
      CORBA::make_reference<IDL::traits<StringNode>::obv_type> ();
  l_dummy->name ("LeftNode");
  sn->left (l_dummy);

  // Create the right leaf.
  IDL::traits<StringNode>::ref_type r_dummy =
      CORBA::make_reference<IDL::traits<StringNode>::obv_type> ();
  r_dummy->name ("RightNode");
  sn->right (r_dummy);

  outarg = tc;
}

void
passer_i::pass_nil (IDL::traits<base>::ref_type& outarg)
{
  outarg = nullptr;
}
