/**
 * @file    client.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "ace/OS_NS_string.h"
#include "testC.h"
#include "testlib/taox11_testlog.h"
#include <string>

static const ACE_TCHAR *IOR = ACE_TEXT ("file://ior");
static int shutdown_server = 0;

static bool parse_args(int argc, ACE_TCHAR **argv)
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("k:x"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'k':
        IOR = get_opts.opt_arg();
        break;

      case 'x':
        shutdown_server = 1;
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:  -k <ior> -x shutdown server"
              << std::endl;
        return false;
    }

  if (IOR == 0)
  {
    TAOX11_TEST_ERROR << "Please specify the IOR"
          << std::endl;
    return false;
  }

  return true;
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init(argc, argv);
    if (parse_args(argc, argv) == false)
      return 1;
    IDL::traits<CORBA::Object>::ref_type object = orb->string_to_object(IOR);

    Hello::_ref_type test1 = IDL::traits<Hello>::narrow (object);
    test1->method();
    Hello::_ref_type test2 = test1->create_POA();
    test2->method();
    test1->destroy_POA();
    Hello::_ref_type test3 = test1->create_POA();
    test2->method();
    test3->method();
    if (shutdown_server)
    {
      test1->shutdown();
    }

    orb->destroy();
  }
  catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught: " << ex << std::endl;
    return 1;
  }

  return 0;
}
