/**
 * @file    server.cpp
 * @author  Marijke Hengstmengel
 *
 * @brief   CORBA C++11x server ami test
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "test_i.h"

#include "tao/x11/log.h"

#include <fstream>

int
main(int argc, ACE_TCHAR *argv[])
{
  // by default show all messages logged through global logger
  X11_LOGGER::priority_mask (x11_logger::X11_LogMask::LP_ALL);

  int result = 0;
  try
    {
      IDL::traits<CORBA::ORB>::ref_type _orb = CORBA::ORB_init (argc, argv);

      if (_orb == nullptr)
        {
          taox11_error
            << "ERROR: CORBA::ORB_init (argc, argv) returned null ORB."
            << std::endl;
          return 1;
        }

      IDL::traits<CORBA::Object>::ref_type obj =
        _orb->resolve_initial_references ("RootPOA");

      if (!obj)
        {
          taox11_error
            << "ERROR: resolve_initial_references (\"RootPOA\") returned null reference." << std::endl;
          return 1;
        }

      taox11_info << "retrieved RootPOA object reference" << std::endl;

      IDL::traits<PortableServer::POA>::ref_type root_poa =
         IDL::traits<PortableServer::POA>::narrow (obj);

      if (!root_poa)
        {
          taox11_error
            << "ERROR: PortableServer::POA::_narrow (obj) returned null object."
            << std::endl;
          return 1;
        }

      taox11_info << "narrowed POA interface" << std::endl;

      IDL::traits<PortableServer::POAManager>::ref_type poaman =
        root_poa->the_POAManager ();

      if (!poaman)
        {
          taox11_error
            << "ERROR: root_poa->the_POAManager () returned null object."
            << std::endl;
          return 1;
        }

      CORBA::servant_traits<Test::A>::ref_type a_impl =
        CORBA::make_reference<A_i> (_orb);


      taox11_info << "created Test::A servant" << std::endl;

      PortableServer::ObjectId id = root_poa->activate_object (a_impl);

      taox11_info << "activated Test::A servant" << std::endl;

      IDL::traits<CORBA::Object>::ref_type a_obj =
        root_poa->id_to_reference (id);

      if (a_obj == nullptr)
        {
          taox11_error
            << "ERROR: root_poa->id_to_reference (id) returned null reference."
            << std::endl;
          return 1;
        }

      IDL::traits<Test::A>::ref_type a_ref =
          IDL::traits<Test::A>::narrow (a_obj);

      if (a_ref == nullptr)
        {
          taox11_error
            << "ERROR: Test::A::_narrow (a_obj) returned null reference."
            << std::endl;
          return 1;
        }

      std::string ior = _orb->object_to_string (a_ref);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos("server.ior");
      if (!fos)
        {
          taox11_error << "ERROR: failed to open file 'server.ior'" << std::endl;
          return 1;
        }
      fos << ior;
      fos.close ();

      taox11_info << "IOR for Test::A servant written to 'server.ior' : " << ior << std::endl;

      poaman->activate ();

      taox11_info << "starting event loop" << std::endl;

      _orb->run ();

      taox11_info << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      _orb->destroy ();
    }
  catch (const std::exception& e)
    {
      taox11_error << "exception caught: " << e.what() << std::endl;
      return 1;
    }

  return result;
}
