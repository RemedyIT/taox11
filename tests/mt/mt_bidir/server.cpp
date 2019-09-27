/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "ace/Get_Opt.h"
#include "sender_i.h"
#include "server_task.h"

#include "tao/x11/bidir_giop/bidir_giop.h"
#include "tao/x11/anytypecode/any.h"
#include "testlib/taox11_testlog.h"

#include "ace/Manual_Event.h"

#include <fstream>

std::string ior_output_file ("");
static int no_iterations = 10;
static int no_clients = 2;

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:i:c:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.optarg;
        break;
      case 'c':
        no_clients = std::atoi (get_opts.optarg);
        break;
      case 'i':
        no_iterations = std::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
              << "-o <iorfile>\n"
              << "-c <no_clients>\n"
              << "-i <no_iterations>\n"
              << std::endl;
        return false;
      }
  // Indicates successful parsing of the command line
  return true;
}


int
main (int argc, char *argv[])
{
  try
    {
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv);

      if (!parse_args (argc, argv))
        return 1;

      IDL::traits<CORBA::Object>::ref_type poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (!poa_object)
      {
        TAOX11_TEST_ERROR << X11_LOG_THREAD_INFO
              << "Unable to initialize the POA" << std::endl;
        return 1;
      }

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);

      CORBA::Any pol;
      pol <<= BiDirPolicy::BOTH;
      policies[0] =
        orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                            pol);

      // Create POA as child of RootPOA with the above policies.  This POA
      // will receive request in the same connection in which it sent
      // the request
      IDL::traits<PortableServer::POA>::ref_type child_poa =
        root_poa->create_POA ("childPOA",
                              poa_manager,
                              policies);

      // Creation of childPOA is over. Destroy the Policy objects.
      for (IDL::traits<CORBA::Policy>::ref_type _pol : policies)
      {
        _pol->destroy ();
      }

      poa_manager->activate ();

      ACE_Manual_Event manual_event;

      CORBA::servant_reference<Sender_i> sender =
        CORBA::make_reference<Sender_i> (no_clients, manual_event);

      PortableServer::ObjectId const id =
        child_poa->activate_object (sender);

      IDL::traits<CORBA::Object>::ref_type obj =
        child_poa->id_to_reference (id);

      std::string const ior = orb->object_to_string (obj);

      TAOX11_TEST_DEBUG << "Activated as <"
            << ior << ">" << std::endl;

      // If the ior_output_file exists, output the ior to it
      if (!ior_output_file.empty())
      {
        std::ofstream fos (ior_output_file);
        if (!fos)
        {
          TAOX11_TEST_ERROR << "ERROR: failed to open file "
                << ior_output_file << std::endl;
          return 1;
        }
        fos << ior;
        fos.close ();
      }

      Server_Task server_task (sender,
                               manual_event,
                               no_iterations,
                               orb,
                               ACE_Thread_Manager::instance ());

      if (server_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          TAOX11_TEST_ERROR << "Error activating server task" << std::endl;
          return 1;
        }

      TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
          << "Server threads are activated" << std::endl;

      // run the ORB for at most 60 seconds...
      std::chrono::seconds tv (60);

      // Call the ORB run from the main thread
      orb->run (tv);

      TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
          << "ORB event loop finished" << std::endl;

      ACE_Thread_Manager::instance ()->wait ();

      TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
          << "Server threads are ready" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception <server::main> :"
            << ex << std::endl;
      return 1;
    }

  return 0;
}
