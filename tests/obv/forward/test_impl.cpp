/**
 * @file    test_impl.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 servant implementation
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "test_impl.h"
#include "treebaseC.h"

Test_impl::Test_impl (IDL::traits<CORBA::ORB>::ref_type orb)
    : orb_ (orb)
{
}

void
reflect_node (IDL::traits<BaseNode>::ref_type bn)
{
  if (bn == nullptr)
  {
    return;
  }

  reflect_node (bn->left ());
  reflect_node (bn->right ());

  CORBA::valuetype_traits<BaseNode>::ref_type old_right = bn->right ();
  CORBA::valuetype_traits<BaseNode>::ref_type old_left = bn->left ();

  bn->right (old_left);
  bn->left (old_right);
}

IDL::traits<TreeController>::ref_type
Test_impl::reflect (IDL::traits<TreeController>::ref_type tc)
{
  if (tc)
  {
    reflect_node (tc-> root());
  }
  return tc;
}

void
Test_impl::shutdown ()
{
  this->orb_->shutdown (false);
}
