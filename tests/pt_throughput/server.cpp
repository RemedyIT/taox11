/**
 * @file    server.cpp
 * @author  Mark Drijver
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "testlib/taox11_testlog.h"
#include <fstream>

#include "receiver_factory.h"
#include "ace/Get_Opt.h"

bool parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts(argc, argv, ACE_TEXT("o:"));
  int c;

  while ((c = get_opts()) != -1)
    switch (c)
    {
      case 'o':
        //ior_output_file = get_opts.opt_arg();
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage: -o <iorfile>"
              << std::endl;
        return false;
    }
  // Indicates successful parsing of the command line
  return true;
}

int ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int result = 0;
  try
  {
    IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init(argc, argv);

    if (_orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type poa_object = _orb->resolve_initial_references("RootPOA");

    if (poa_object == nullptr)
    {
      TAOX11_TEST_ERROR
            << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference."
            << std::endl;
      return 1;
    }

      TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (poa_object);

    if (!root_poa)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object." << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed POA interface" << std::endl;

    IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

    if (!poaman)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
      return 1;
    }

    if (parse_args(argc, argv) == false)
      return 1;

    CORBA::servant_traits<Test::Receiver_Factory>::ref_type
      receiver_factory_impl = CORBA::make_reference<Receiver_Factory> (_orb, root_poa);
    TAOX11_TEST_DEBUG << "created Receiver_Factory" << std::endl;

    PortableServer::ObjectId id = root_poa->activate_object(
        receiver_factory_impl);
    TAOX11_TEST_DEBUG << "activated Receiver_Factory servant" << std::endl;
    IDL::traits<CORBA::Object>::ref_type object = root_poa->id_to_reference(id);
    if (object == nullptr)
    {
      TAOX11_TEST_ERROR
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
      return 1;
    }
    Test::Receiver_Factory::_ref_type factory = IDL::traits<Test::Receiver_Factory>::narrow ( object);

    if (factory == nullptr)
    {
      TAOX11_TEST_ERROR
            << "ERROR: IDL::traits<Test::Hello>::narrow (hello_obj) returned null reference."
            << std::endl;
      return 1;
    }

    std::string ior = _orb->object_to_string(factory);

    // Output the IOR to the <ior_output_file>
      std::ofstream fos("test.ior");
    if (!fos)
    {
      TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
      return 1;
    }
    fos << ior;
    fos.close();

    TAOX11_TEST_DEBUG << "IOR for Hello servant written to 'test.ior' : "
          << ior << std::endl;

    poaman->activate ();

    TAOX11_TEST_DEBUG << "starting event loop" << std::endl;

    _orb->run();

    TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

    root_poa->destroy (true, true);

    _orb->destroy();
  }
  catch (const std::exception& e)
  {
      TAOX11_TEST_ERROR << "ERROR: exception caught: " << e.what () << std::endl;
      return 1;
  }

  return result;
}
