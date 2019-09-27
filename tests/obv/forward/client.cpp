/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "treebaseC.h"
#include "treecontrollerC.h"
#include "treenodeC.h"

#include "testlib/taox11_testlog.h"

#include "ace/Get_Opt.h"

const ACE_TCHAR *ior = ACE_TEXT("file://test.ior");

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: " << std::endl
                     << "-k <ior>" << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

void
dump_node (IDL::traits<BaseNode>::ref_type bn, int indent)
{
  if (bn == nullptr)
  {
    return;
  }

  for (int32_t i = indent; i != 0; i--)
  {
    TAOX11_TEST_DEBUG << "  " << std::flush;
  }

  IDL::traits<StringNode>::ref_type sn =
    IDL::traits<StringNode>::narrow (bn);
  if (sn)
  {
    TAOX11_TEST_DEBUG << "<StringNode> " << sn->name ()
                 << std::endl;
  }
  else
  {
    TAOX11_TEST_DEBUG << "<BaseNode>" << std::endl;
  }

  dump_node (bn->left (), indent + 1);
  dump_node (bn->right (), indent + 1);
}

void
dump_tree (IDL::traits<TreeController>::ref_type tc)
{
  TAOX11_TEST_DEBUG << "start tree dump" << std::endl;

  dump_node (tc->root (), 1);

  TAOX11_TEST_DEBUG << "end tree dump" << std::endl;
}


int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    if (!orb)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
      return 1;
    }

    if (parse_args (argc, argv) == false)
    {
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type obj =
      orb->string_to_object (ior);

    if (!obj)
    {
      TAOX11_TEST_ERROR << "string_to_object failed." << std::endl;
      return 1;
    }

    IDL::traits<Test>::ref_type objref = IDL::traits<Test>::narrow (obj);

    if (!objref)
    {
      TAOX11_TEST_ERROR << "IDL::traits<Test>::narrow failed." << std::endl;
      return 1;
    }

    // Create and register factory for BaseNode.
    IDL::traits<BaseNode>::factory_ref_type bn_factory = CORBA::make_reference<BaseNode_init> ();

    orb->register_value_factory (bn_factory->_obv_repository_id (),
                                  bn_factory);

    // Create and register factory for tree_controller_init.
    TreeController_init::_ref_type tc_factory = CORBA::make_reference<TreeController_init> ();

    orb->register_value_factory (tc_factory->_obv_repository_id (),
                                  tc_factory);

    // Create and register factory for StringNode.
    IDL::traits<StringNode>::factory_ref_type sn_factory = CORBA::make_reference<StringNode_init> ();

    orb->register_value_factory (sn_factory->_obv_repository_id (),
                                  sn_factory);

    IDL::traits<TreeController>::ref_type tc =
      CORBA::make_reference< IDL::traits<TreeController>::obv_type> ();
    // Create the root node.
    {
      IDL::traits<StringNode>::ref_type sn =
        CORBA::make_reference< IDL::traits<StringNode>::obv_type> ();
      sn->name ("RootNode");
      tc->root (sn);

      // Create the left leaf.
      {
        IDL::traits<StringNode>::ref_type sn_left =
          CORBA::make_reference< IDL::traits<StringNode>::obv_type> ();
        sn_left->name ("LeftNode");
        sn->left (sn_left);
      }

      // Create the right leaf.
      {
        IDL::traits<StringNode>::ref_type sn_right =
          CORBA::make_reference< IDL::traits<StringNode>::obv_type> ();
        sn_right->name ("RightNode");
        sn->right (sn_right);
      }
    }

    // Dump the tree.
    dump_tree (tc);

    IDL::traits<TreeController>::ref_type result_tc =
      objref->reflect (tc);

    // Dump the resulting tree.
    dump_tree (result_tc);

    objref->shutdown ();

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Client: exception caught: " << ex << std::endl;
    return 1;
  }

  return 0;
}
