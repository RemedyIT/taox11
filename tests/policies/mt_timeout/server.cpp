/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "sleep_service.h"
#include "server_task.h"
#include "ace/Get_Opt.h"

#include "testlib/taox11_testlog.h"

#include <fstream>

std::string ior_output_file ("test.ior");

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:") );
  int c;

  while ( (c = get_opts () ) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
                     "-o <iorfile>"
                     << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}

int main (int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      IDL::traits<CORBA::Object>::ref_type poa_object =
        orb->resolve_initial_references ("RootPOA");

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

      if (!root_poa)
        {
          TAOX11_TEST_PANIC << "("
            << x11_logger::processid << "|"
            << x11_logger::threadid << ") Panic: nil RootPOA"
                       << std::endl;
          return 1;
        }

      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) == false)
        return 1;

      CORBA::servant_traits<Test::Sleep_Service>::ref_type sleep_service_impl =
        CORBA::make_reference<Sleep_Service> (orb);

      PortableServer::ObjectId id =
        root_poa->activate_object (sleep_service_impl);

      IDL::traits<CORBA::Object>::ref_type object =
        root_poa->id_to_reference (id);

      IDL::traits<Test::Sleep_Service>::ref_type process =
        IDL::traits<Test::Sleep_Service>::narrow (object);

      std::string ior = orb->object_to_string (process);

      // Output the IOR to the <ior_output_file>
      std::ofstream fos (ior_output_file);
      if (!fos)
        {
          TAOX11_TEST_ERROR << "ERROR: failed to open file "
            << ior_output_file << std::endl;
          return 1;
        }
      fos << ior;
      fos.close ();

      poa_manager->activate ();

      Server_Task server_task (ACE_Thread_Manager::instance (),
                               orb,
                               ACE_Time_Value (250));

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 8, 1) == -1)
        {
          TAOX11_TEST_ERROR << "Error activating server task" << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << "("
        << x11_logger::processid << "|"
        << x11_logger::threadid << ") Started server"
                   << std::endl;

      server_task.thr_mgr ()->wait ();

      TAOX11_TEST_DEBUG << "Server threads are ready" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();

      TAOX11_TEST_DEBUG << "Server is ready" << std::endl;
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Exception caught in server : " << ex << std::endl;
      return 1;
    }

  return 0;
}
