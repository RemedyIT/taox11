/**
 * @file    server.cpp
 * @author  Martin Corino
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb = CORBA::ORB_init (argc, argv);

      if (!orb)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::Object>::ref_type obj = orb->resolve_initial_references ("RootPOA");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_DEBUG << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (obj);

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

      CORBA::servant_traits<Test::Bar>::ref_type bar_impl = CORBA::make_reference<Bar> ();

      PortableServer::ObjectId bar_id = root_poa->activate_object (bar_impl);

      TAOX11_TEST_DEBUG << "activated Bar servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type bar_obj = root_poa->id_to_reference (bar_id);

      if (!bar_obj)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->id_to_reference (bar_id) returned null reference." << std::endl;
        return 1;
      }

      IDL::traits<Test::Bar>::ref_type bar = IDL::traits<Test::Bar>::narrow (bar_obj);

      if (!bar)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Bar>::narrow (bar_obj) returned null reference." << std::endl;
        return 1;
      }

      CORBA::servant_traits<Test::Foo>::ref_type foo_impl = CORBA::make_reference<Foo> (orb, bar);

      TAOX11_TEST_DEBUG << "created Foo servant" << std::endl;

      PortableServer::ObjectId foo_id = root_poa->activate_object (foo_impl);

      TAOX11_TEST_DEBUG << "activated Foo servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type foo_obj = root_poa->id_to_reference (foo_id);

      if (!foo_obj)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->id_to_reference (id) returned null reference." << std::endl;
        return 1;
      }

      IDL::traits<Test::Foo>::ref_type foo = IDL::traits<Test::Foo>::narrow (foo_obj);

      if (!foo)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (foo_obj) returned null reference." << std::endl;
        return 1;
      }

      std::string const ior = orb->object_to_string (foo);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos("test.ior");
      if (!fos)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
        return 1;
      }
      fos << ior;
      fos.close ();

      TAOX11_TEST_DEBUG << "IOR for Foo servant written to 'test.ior' : " << ior << std::endl;

      poaman->activate ();

      TAOX11_TEST_DEBUG << "starting event loop" << std::endl;

      orb->run ();

      TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();

      TAOX11_TEST_DEBUG << "server finished" << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "SERVER - exception caught: " << e.what () << std::endl;
      return 1;
    }

  return 0;
}
