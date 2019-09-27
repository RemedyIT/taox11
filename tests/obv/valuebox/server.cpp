/**
 * @file    server.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "test_impl.h"

#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "testlib/taox11_testlog.h"
#include <fstream>

std::string ior_output_file = ACE_TEXT("test.ior");

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
    case 'o':
      ior_output_file = get_opts.opt_arg();
      break;
    case '?':
    default:
      TAOX11_TEST_ERROR << "usage:  " << argv[0] << " -o <iorfile>\n";
      return false;
      break;
    }
  // Indicates successful parsing of the command line
  return true;
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);
    IDL::traits<CORBA::Object>::ref_type poa_object =
        _orb->resolve_initial_references("RootPOA");

    IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);
    if (!root_poa)
    {
      TAOX11_TEST_ERROR
          << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned nil.\n";
      return 1;
    }

    IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager();

    if (parse_args(argc, argv) == false)
      return 1;

    CORBA::servant_reference<Test_impl> test_impl = CORBA::make_reference<
        Test_impl>(_orb);
    TAOX11_TEST_INFO << "created Test_impl servant" << std::endl;

    //PortableServer::ServantBase owner_transfer(test_impl);

    PortableServer::ObjectId id = root_poa->activate_object(test_impl);

    IDL::traits<CORBA::Object>::ref_type object = root_poa->id_to_reference(
        id);

    IDL::traits<Test>::ref_type test = IDL::traits<Test>::narrow (object);

    std::string ior = _orb->object_to_string(test);

    // If the ior_output_file exists, output the ior to it
    std::ofstream fos(ior_output_file);
    if (!fos)
    {
      TAOX11_TEST_ERROR << "ERROR: failed to open file '" << ior_output_file
          << "'\n";
      return 1;
    }
    fos << ior;
    fos.close();

    poa_manager->activate();

    _orb->run();

    TAOX11_TEST_DEBUG << "(%P|%t) server - event loop finished\n";
    root_poa->destroy(1, 1);
    _orb->destroy();
  } catch (const CORBA::Exception& ex)
  {
    TAOX11_TEST_ERROR << "Exception caught:" << ex;
    return 1;
  }

  return 0;

}
