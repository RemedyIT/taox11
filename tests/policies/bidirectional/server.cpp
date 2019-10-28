/**
 * @file    server.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Bidir Policy server application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "tao/x11/bidir_giop/bidir_giop.h"
#include "tao/x11/anytypecode/any.h"
#include "tao/x11/portable_server/portableserver_functions.h"

#include "testlib/taox11_testlog.h"
#include <fstream>

std::string ior_output_file ("");
int no_iterations = 10;

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:i:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'i':
        no_iterations = std::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
              << "-o <iorfile>\n"
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

      IDL::traits<CORBA::Object>::ref_type poa_object =
        orb->resolve_initial_references ("RootPOA");

      if (!poa_object)
      {
        TAOX11_TEST_ERROR << X11_LOG_THREAD_INFO
              << "Unable to initialize the POA."
              << std::endl;
        return 1;
      }

      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (poa_object);

      IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
        root_poa->the_POAManager ();

      // Policies for the childPOA to be created.
      CORBA::PolicyList policies (1);

      try
      {
        CORBA::Any pol;
        pol <<= BiDirPolicy::BOTH;
        policies[0] =
          orb->create_policy (BiDirPolicy::BIDIRECTIONAL_POLICY_TYPE,
                              pol);
      }
      catch (const CORBA::PolicyError &ex)
      {
        TAOX11_TEST_ERROR << "server - main - caught "
              << "unexpected policy error exception - "
              << ex.reason () << std::endl;
        return 1;
      }

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

      if (!parse_args (argc, argv))
        return 1;

      CORBA::servant_reference<Simple_Server_i> server_impl =
        CORBA::make_reference<Simple_Server_i> (orb, no_iterations);

      PortableServer::ObjectId id =
        PortableServer::string_to_ObjectId ("simple_server");

      child_poa->activate_object_with_id (id, server_impl);

      IDL::traits<CORBA::Object>::ref_type obj =
        child_poa->id_to_reference (id);

      std::string ior = orb->object_to_string (obj);

      TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO
            << "Simple Server activated as <" << ior << ">" << std::endl;

      // If the ior_output_file exists, output the ior to it
      if (!ior_output_file.empty())
        {
          // Output the IOR to the <ior_output_file>
          std::ofstream fos(ior_output_file);
          if (!fos)
          {
            TAOX11_TEST_ERROR << "ERROR: failed to open file '"
                  << ior_output_file << "'" << std::endl;
            return 1;
          }
          fos << ior;
          fos.close ();
        }

      bool retval = false;
      while (!retval)
        {
          // Just process one upcall. We know that we would get the
          // clients IOR in that call.
          bool const pending = orb->work_pending();

          if (pending)
          {
            orb->perform_work();
          }

          // Now that hopefully we have the clients IOR, just start
          // making remote calls to the client.
          retval = server_impl->call_client();
        }
      TAOX11_TEST_DEBUG << "event loop finished" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception in <server>: "
            << ex << std::endl;
      return 1;
    }

  return 0;
}
