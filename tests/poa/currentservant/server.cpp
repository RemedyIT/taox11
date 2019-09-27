/**
 * @file    server.cpp
 * @author  Mark Drijver
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */

#include "hello.h"
#include "tao/x11/portable_server/portableserver_functions.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB." << std::endl;
        return 1;
      }

      IDL::traits<CORBA::Object>::ref_type obj = _orb->resolve_initial_references ("RootPOA");

      if (!obj)
      {
        TAOX11_TEST_ERROR << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
        return 1;
      }

      TAOX11_TEST_INFO << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa = IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<PortableServer::POA>::narrow (obj) returned null object." << std::endl;
        return 1;
      }

      TAOX11_TEST_INFO << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman = root_poa->the_POAManager ();

      if (!poaman)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->the_POAManager () returned null object." << std::endl;
        return 1;
      }

      CORBA::servant_traits<Test::Hello>::ref_type hello_impl =
        CORBA::make_reference<Hello> (_orb);
      TAOX11_TEST_INFO << "created Hello servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (hello_impl);
      TAOX11_TEST_INFO << "activated Hello servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type hello_obj = root_poa->id_to_reference (id);
      if (hello_obj == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: currentPoa->id_to_reference (id) returned null reference." << std::endl;
        return 1;
      }

      // Invoke reference_to_id(). Should retrieve default servant id.
      TAOX11_TEST_INFO << "Invoke reference_to_id." << std::endl;
      PortableServer::ObjectId id2 = root_poa->reference_to_id (hello_obj);
      if (!id2.empty())
      {
        TAOX11_TEST_INFO << "Invoke reference_to_id successful." << std::endl;
      }
      else
      {
        TAOX11_TEST_ERROR << "ERROR2 : Return value of reference_to_id wrong." << id2.empty()  << "  " << id2.size() << "  " << id2
            << std::endl;
        return 1;
      }

      // Invoke reference_to_id() with a null reference should result in an exception
      TAOX11_TEST_INFO << "Invoke reference_to_id with null reference." << std::endl;
      try
      {
        PortableServer::ObjectId id3 = root_poa->reference_to_id (nullptr);
        TAOX11_TEST_ERROR << "ERROR: root_poa->reference_to_id with a null reference should throw exception."
              << std::endl;
        return 1;
      }
      catch (const CORBA::INV_OBJREF&)
      {
        TAOX11_TEST_DEBUG << "root_poa->reference_to_id(nullptr): caught CORBA::INV_OBJREF as expected" << std::endl;
      }

      IDL::traits<Test::Hello>::ref_type hello = IDL::traits<Test::Hello>::narrow (hello_obj);
      if (hello == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Hello>::narrow (hello_obj) returned null reference." << std::endl;
        return 1;
      }

      std::string ior = _orb->object_to_string (hello);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos("test.ior");
      if (!fos)
      {
        TAOX11_TEST_ERROR << "ERROR: failed to open file 'test.ior'" << std::endl;
        return 1;
      }
      fos << ior;
      fos.close ();

      TAOX11_TEST_INFO << "IOR for Hello servant written to 'test.ior' : " << ior << std::endl;

      poaman->activate ();

      TAOX11_TEST_INFO << "starting event loop" << std::endl;

      _orb->run ();

      TAOX11_TEST_INFO << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      _orb->destroy ();

      TAOX11_TEST_INFO << "orb destroyed" << std::endl;
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e.what () << std::endl;
      return 1;
    }

  return 0;
}
