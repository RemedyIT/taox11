/**
 * @file    client.cpp
 * @author  Martin Corino
 *
 * @brief   CORBA C++11 client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "tc_i.h"
#include "ace/Get_Opt.h"
#include "testlib/taox11_testlog.h"

const ACE_TCHAR *ior_input_file = ACE_TEXT ("file://test.ior");
int debug = 0;

enum class test_select
{
  TEST_STATE,
  TEST_OPERATION,
  TEST_EXCEPTION,
  TEST_WEAK,
  TEST_ALL
};

test_select which_test = test_select::TEST_ALL;

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:dsoew"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
              ior_input_file = get_opts.optarg;
              break;
      case 'd':
              debug = 1;
              break;
      case 's':
              which_test = test_select::TEST_STATE;
              break;
      case 'o':
              which_test = test_select::TEST_OPERATION;
              break;
      case 'e':
              which_test = test_select::TEST_EXCEPTION;
              break;
      case 'w':
              which_test = test_select::TEST_WEAK;
              break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: " << argv[0] << "-k <iorfile>" << std::endl;
        return false;
      }

  // Indicates successful parsing of the command line
  return true;
}

void
dump_node (IDL::traits<BaseNode>::ref_type bn, int indent)
{
  if (!bn)
  {
    return;
  }

  IDL::traits<StringNode>::ref_type sn = IDL::traits<StringNode>::narrow (bn);

  if (debug)
  {
    // This is ugly I know
    for (int i = indent; i != 0; --i) TAOX11_TEST_DEBUG << "  ";

    if (sn)
    {
      TAOX11_TEST_DEBUG << "<StringNode> " << sn->name () << std::endl;
    }
    else
    {
      TAOX11_TEST_DEBUG << "<BaseNode>" << std::endl;
    }
  }

  dump_node (bn->left (),
             indent + 1);
  dump_node (bn->right (),
             indent + 1);
}

void
dump_tree (IDL::traits<TreeController>::ref_type tc)
{
  if (debug)
  {
    TAOX11_TEST_DEBUG << "start tree dump" << std::endl;
  }

  dump_node (tc->root (), 1);

  if (debug)
  {
    TAOX11_TEST_DEBUG << "end tree dump" << std::endl;
  }
}

void
test_state (IDL::traits<base>::ref_type abs)
{
  IDL::traits<CORBA::ValueBase>::ref_type vbase = abs->_to_value ();
  IDL::traits<TreeController>::ref_type tc = IDL::traits<TreeController>::narrow (vbase);
  dump_tree (tc);
}

int
test_operation (base::_ref_type abs)
{
  int errors = 0;

  std::string retval = abs->base_op ("base_op");

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (abstract base_op) : " << retval << std::endl;
  }

  if (retval != "good")
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (base_op): wrong return value "
      << "received: expected <good> - received <" << retval << ">"
      << std::endl;
    ++errors;
  }

  int32_t r_foo = abs->r_foo ();

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (abstract r_foo) : " << r_foo << std::endl;
  }

  if (r_foo != 1)
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (base_op r_foo): wrong return value "
      << "received: expected <1> - received <" << r_foo << ">"
      << std::endl;
    ++errors;
  }

  abs->rw_foo (5);
  int32_t rw_foo = abs->rw_foo ();

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (abstract rw_foo) : " << rw_foo << std::endl;
  }

  if (rw_foo != 5)
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (abstract rw_foo): wrong return value "
      << "received: expected <5> - received <" << rw_foo << ">"
      << std::endl;
    ++errors;
  }

  IDL::traits<CORBA::Object>::ref_type obj = abs->_to_object ();

  IDL::traits<foo>::ref_type concrete = IDL::traits<foo>::narrow (obj);

  retval = concrete->foo_op ("foo_op");

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (concrete foo_op) : " << retval << std::endl;
  }

  if (retval != "good")
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (foo_op): wrong return value "
      << "received: expected <good> - received <" << retval << ">"
      << std::endl;
    ++errors;
  }

  r_foo = concrete->r_foo ();

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (concrete r_foo) : " << r_foo << std::endl;
  }

  if (r_foo != 1)
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (concrete r_foo): wrong return value "
      << "received: expected <1> - received <" << r_foo << ">"
      << std::endl;
    ++errors;
  }

  concrete->rw_foo (7);
  rw_foo = concrete->rw_foo ();

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (concrete rw_foo) : " << rw_foo << std::endl;
  }

  if (rw_foo != 7)
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (concrete rw_foo): wrong return value "
      << "received: expected <7> - received <" << rw_foo << ">"
      << std::endl;
    ++errors;
  }

  retval = concrete->base_op ("base_op");

  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_operation (concrete base_op) : " << retval << std::endl;
  }

  if (retval != "good")
  {
    TAOX11_TEST_ERROR << "ERROR - test_operation (concrete->base_op): wrong return value "
      << "received: expected <good> - received <" << retval << ">"
      << std::endl;
    ++errors;
  }
  return errors;
}

int
test_exception (IDL::traits<base>::ref_type abs)
{
  try
  {
    std::string retval = abs->base_op ("bad_name");
    if (debug)
    {
      TAOX11_TEST_DEBUG << "test_exception (abstract base_op)" << retval << std::endl;
    }

    TAOX11_TEST_ERROR << "ERROR - test_exception: No exception caught"
      << std::endl;
    return 1;
  }
  catch (const BadInput& ex)
  {
    if (debug)
    {
      TAOX11_TEST_DEBUG << ex << std::endl;
    }
    if (ex.message () != "expected \"base_op\"\n")
    {
      TAOX11_TEST_ERROR << "ERROR - test_exception: Wrong exception message received : "
        << "expected <expected \"base_op\"> - received <" << ex.message () << ">"
        << std::endl;
      return 1;
    }
  }
  return 0;
}

int
test_abstract ()
{
  int errors = 0;
  // Check the is_abstract traits of an abstract interface
  // Check the core first
  if (!IDL::traits<CORBA::LocalObject>::is_abstract ())
  {
    if (debug)
    {
      TAOX11_TEST_DEBUG << "test_abstract - OK, is_abstract trait correctly set for "
        << "CORBA::LocalObject." << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "test_abstract - ERROR, bad is_abstract trait for "
      << "CORBA::LocalObject found." << std::endl;
    ++errors;
  }
  if (!IDL::traits<CORBA::Object>::is_abstract ())
  {
    if (debug)
    {
      TAOX11_TEST_DEBUG << "test_abstract - OK, is_abstract trait correctly set for "
        << "CORBA::Object." << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "test_abstract - ERROR, bad is_abstract trait for "
      << "CORBA::Object found." << std::endl;
    ++errors;
  }

  if (IDL::traits<base>::is_abstract ())
  {
    if (debug)
    {
      TAOX11_TEST_DEBUG << "test_abstract - OK, is_abstract trait correctly set for "
        << "<base>." << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "test_abstract - ERROR, bad is_abstract trait for "
      << "<base> found." << std::endl;
    ++errors;
  }

  // Check the is_abstract traits of an interface
  if (!IDL::traits<foo>::is_abstract ())
  {
    if (debug)
    {
      TAOX11_TEST_DEBUG << "test_abstract - OK, is_abstract trait correctly set for "
        << "<foo>." << std::endl;
    }
  }
  else
  {
    TAOX11_TEST_ERROR << "test_abstract - ERROR, bad is_abstract trait for "
      << "<foo> found." << std::endl;
    ++errors;
  }
  return errors;
}

int
test_weak (IDL::traits<base>::ref_type abs)
{
  int errors = 0;

  IDL::traits<base>::weak_ref_type weak_abs =
    abs.weak_reference ();

  IDL::traits<base>::ref_type test_abs = weak_abs.lock();
  if (test_abs == nullptr)
  {
    TAOX11_TEST_ERROR << "test_weak - ERROR, weak_abs.lock() returned null reference."
      << std::endl;
    ++errors;
  }
  if (weak_abs.expired())
  {
     TAOX11_TEST_ERROR << "ERROR: weak_abs is expired."<< std::endl;
     ++errors;
  }
  else if (debug)
  {
    TAOX11_TEST_DEBUG << "OK: weak_abs isn't expired."<< std::endl;
  }
  std::string retval = test_abs->base_op ("base_op");
  if (debug)
  {
    TAOX11_TEST_DEBUG << "test_weak (base_op) : " << retval << std::endl;
  }

  return errors;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  uint16_t errors = 0;

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
      orb->string_to_object (ior_input_file);

    if (!obj)
    {
      TAOX11_TEST_ERROR << "string_to_object failed." << std::endl;
      return 1;
    }

    IDL::traits<passer>::ref_type objref = IDL::traits<passer>::narrow (obj);

    if (!objref)
    {
      TAOX11_TEST_ERROR << "IDL::traits<passer>::narrow failed." << std::endl;
      return 1;
    }

    IDL::traits<base>::ref_type package;

    if (which_test == test_select::TEST_STATE || which_test == test_select::TEST_ALL)
    {
      // Create and register factory for BaseNode.
      IDL::traits<BaseNode>::factory_ref_type bn_factory = CORBA::make_reference<BaseNode_init> ();

      orb->register_value_factory (bn_factory->_obv_repository_id (),
                                   bn_factory);

      // Create and register factory for tree_controller_init.
      tree_controller_init::_ref_type tc_factory = CORBA::make_reference<tree_controller_init> ();

      orb->register_value_factory (tc_factory->_obv_repository_id (),
                                   tc_factory);

      // Create and register factory for StringNode.
      IDL::traits<StringNode>::factory_ref_type sn_factory = CORBA::make_reference<StringNode_init> ();

      orb->register_value_factory (sn_factory->_obv_repository_id (),
                                   sn_factory);

      objref->pass_state (package);

      if (!package)
      {
        TAOX11_TEST_ERROR << "pass_state() returned null OUT arg" << std::endl;
        return 1;
      }

      test_state (package);
    }

    if (which_test != test_select::TEST_STATE)
    {
      objref->pass_ops (package);

      if (!package)
      {
        TAOX11_TEST_ERROR << "pass_ops() returned null OUT arg" << std::endl;
        ++errors;
      }
    }

    if (which_test == test_select::TEST_OPERATION || which_test == test_select::TEST_ALL)
    {
      errors += test_operation (package);
    }

    if (which_test == test_select::TEST_EXCEPTION || which_test == test_select::TEST_ALL)
    {
      errors += test_exception (package);
    }

    if (which_test == test_select::TEST_WEAK || which_test == test_select::TEST_ALL)
    {
      errors += test_weak (package);
    }

    if (which_test == test_select::TEST_ALL)
    {
      objref->pass_nil (package);

      if (package)
      {
        TAOX11_TEST_ERROR << "pass_nil() did NOT return null OUT arg" << std::endl;
        ++errors;
      }

      //check whehter the is_abstract returns the correct value
      errors += test_abstract ();
    }

    objref->shutdown ();

    orb->destroy ();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Client: exception caught: " << ex << std::endl;
    return 1;
  }

  if (errors == 0)
  {
    TAOX11_TEST_DEBUG << "No issues found in test" << std::endl;
  }
  else
  {
     TAOX11_TEST_DEBUG << "<" << errors << "> errors found in test" << std::endl;
  }

  return errors;
}
