/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "ami_coll_i.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
  {

    // COMMON
    IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

    if (orb == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
      return 1;
    }

    IDL::traits<CORBA::Object>::ref_type root_poa_obj = orb->resolve_initial_references ("RootPOA");

    if (!root_poa_obj)
    {
      TAOX11_TEST_ERROR << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;

    IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (root_poa_obj);

    if (!root_poa)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<PortableServer::POA>::narrow (root_poa_obj) returned null object." << std::endl;
      return 1;
    }

    TAOX11_TEST_DEBUG << "narrowed POA interface" << std::endl;

    IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

    if (!poaman)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
      return 1;
    }

    // BAR SERVER
    CORBA::servant_traits<Test::Bar>::ref_type bar_impl = CORBA::make_reference<Bar> ();

    PortableServer::ObjectId bar_id = root_poa->activate_object (bar_impl);

    TAOX11_TEST_DEBUG << "activated Bar servant" << std::endl;

    IDL::traits<CORBA::Object>::ref_type bar_obj = root_poa->id_to_reference (bar_id);

    if (bar_obj == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->id_to_reference (bar_id) returned null reference." << std::endl;
      return 1;
    }

    IDL::traits<Test::Bar>::ref_type bar = IDL::traits<Test::Bar>::narrow (bar_obj);

    if (bar == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Bar>::narrow (bar_obj) returned null reference." << std::endl;
      return 1;
    }

    // AMI BAR CLIENT
    CORBA::amic_traits<Test::Bar>::ref_type ami_bar_client = CORBA::amic_traits<Test::Bar>::narrow (bar_obj);
    if (!ami_bar_client)
    {
      TAOX11_TEST_ERROR << "ERROR: CORBA::amic_traits<Test::Bar>::narrow (bar_obj) returned null object."
                   << std::endl;
      return 1;
    }

    // BAR AMI HANDLER
    CORBA::amic_traits<Test::Bar>::replyhandler_servant_ref_type bar_handler_impl =
         CORBA::make_reference<BarHandler> ();

    TAOX11_TEST_INFO << "created BarHandler servant" << std::endl;

    PortableServer::ObjectId bar_handler_id = root_poa->activate_object (bar_handler_impl);

    TAOX11_TEST_INFO << "activated BarHandler servant" << std::endl;

    IDL::traits<CORBA::Object>::ref_type bar_handler_obj =
         root_poa->id_to_reference (bar_handler_id);

    if (bar_handler_obj == nullptr)
    {
      TAOX11_TEST_ERROR
         << "ERROR: root_poa->id_to_reference (bar_handler_id) returned null reference."
         << std::endl;
       return 1;
    }

    CORBA::amic_traits<Test::Bar>::replyhandler_ref_type bar_handler =
        CORBA::amic_traits<Test::Bar>::replyhandler_traits::narrow (bar_handler_obj);

    if (bar_handler == nullptr)
    {
      TAOX11_TEST_ERROR
         << "ERROR: CORBA::amic_traits<A::AMI_Test>::replyhandler_traits::narrow "
         << "(bar_handler_obj) returned null reference."
         << std::endl;
      return 1;
    }

    // FOO
    CORBA::servant_traits<Test::Foo>::ref_type foo_impl =
      CORBA::make_reference<Foo> (orb, bar, bar_handler, ami_bar_client);

    TAOX11_TEST_DEBUG << "created Foo servant" << std::endl;

    PortableServer::ObjectId foo_id = root_poa->activate_object (foo_impl);

    TAOX11_TEST_DEBUG << "activated Foo servant" << std::endl;

    IDL::traits<CORBA::Object>::ref_type foo_obj = root_poa->id_to_reference (foo_id);

    if (foo_obj == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: root_poa->id_to_reference (id) returned null reference." << std::endl;
      return 1;
    }

    IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (foo_obj);

    if (foo == nullptr)
    {
      TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (foo_obj) returned null reference." << std::endl;
      return 1;
    }

    // WRITING OF IOR FILE
    std::string ior = orb->object_to_string (foo);

    // Output the IOR to the <ior_output_file>
    std::ofstream fos("test.ior");
    if (!fos)
    {
      TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
      return 1;
    }
    fos << ior;
    fos.close ();

    // RUN AND CLOSE.
    TAOX11_TEST_DEBUG << "IOR for Foo servant written to 'test.ior' : " << ior << std::endl;

    poaman->activate ();

    TAOX11_TEST_DEBUG << "starting event loop" << std::endl;

    orb->run ();

    TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

    root_poa->destroy (true, true);

    orb->destroy ();
  }
  catch (const std::exception& e)
  {
    TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
    return 1;
  }

  return 0;
}
