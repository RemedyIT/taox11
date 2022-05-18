/**
 * @file    server.cpp
 * @author  Johnny Willemsen
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "foo.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  uint16_t retval = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (!_orb)
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

      CORBA::servant_reference<Foo> hello_impl = CORBA::make_reference<Foo> (_orb);

      TAOX11_TEST_DEBUG << "created Foo servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (hello_impl);

      TAOX11_TEST_DEBUG << "activated Foo servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type hello_obj = root_poa->id_to_reference (id);

      if (hello_obj == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: root_poa->id_to_reference (id) returned null reference." << std::endl;
        return 1;
      }

      IDL::traits<Test::Foo>::ref_type hello = IDL::traits<Test::Foo>::narrow (hello_obj);

      if (hello == nullptr)
      {
        TAOX11_TEST_ERROR << "ERROR: IDL::traits<Test::Foo>::narrow (hello_obj) returned null reference." << std::endl;
        return 1;
      }

      std::string const ior = _orb->object_to_string (hello);

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

      _orb->run ();

      TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

      // Testing the TestUnion_U1 type
      Test::TestUnion_U1 u1_1;
      Test::TestUnion_U1 u1_2 (u1_1); // make a copy
      // Validate that u1_1 and u1_2 are the same
      if (u1_1._d() != u1_2._d())
      {
        TAOX11_TEST_ERROR << "u1_1._d() != u1_2._d()" << std::endl;
        return 1;
      }
      Test::TestUnion_U1 u1_3 (std::move(u1_1)); // do a move, u1_1 is now nil
      // Validate that u1_2 and u1_3 are the same
      if (u1_2._d() != u1_3._d())
      {
        TAOX11_TEST_ERROR << "u1_2._d() != u1_3._d()" << std::endl;
        return 1;
      }
      // Swap the content of u1_2 and u1_3, some gcc compilers give an invalid warning
      // here
      u1_2.swap (u1_3);
      // Validate that u1_2 and u1_3 are the same
      if (u1_2._d() != u1_3._d())
      {
        TAOX11_TEST_ERROR << "u1_2._d() != u1_3._d()" << std::endl;
        return 1;
      }

      // Testing the TestUnion_U2 type
      Test::TestUnion_U2 u2_1;
      Test::TestUnion_U2 u2_2 (u2_1); // make a copy
      // Swap the content of u1_1 and u2_2, commenting this swap operation
      // will trigger the warnings given by some gcc versions above. Also
      // commenting out the call `CORBA::ORB_init (argc, argv)` will remove
      // the warning given by some gcc compilers on the swap above
      u2_1.swap (u2_2);
      // Validate that u1_2 and u2_2 are the same
      if (u2_1._d() != u2_2._d())
      {
        TAOX11_TEST_ERROR << "u2_1._d() != u2_2._d()" << std::endl;
        return 1;
      }
      root_poa->destroy (true, true);

      _orb->destroy ();

      retval += hello_impl->get_error_count ();
    }
  catch (const std::exception& e)
    {
      TAOX11_TEST_ERROR << "exception caught: " << e << std::endl;
      return 1;
    }

  return retval;
}
