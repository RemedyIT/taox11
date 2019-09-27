/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Multi Threaded test application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 * Chamber of commerce Rotterdam nr.276339, The Netherlands
 */
#include "ace/Get_Opt.h"
#include "receiver_i.h"
#include "senderC.h"
#include "tao/x11/bidir_giop/bidir_giop.h"
#include "tao/x11/anytypecode/any.h"
#include "client_task.h"

#include "testlib/taox11_testlog.h"

std::string ior ("file://test.ior");

bool
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.optarg;
        break;
      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n" << "-k <ior>\n" << std::endl;
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
        _pol->destroy ();

      poa_manager->activate ();

      IDL::traits<CORBA::Object>::ref_type object =
        orb->string_to_object (ior);

      Sender::_ref_type sender = IDL::traits<Sender>::narrow (object);

      if (!sender)
        {
          TAOX11_TEST_ERROR << "Object reference <" << ior << "> is nil." << std::endl;
          return 1;
        }

      CORBA::servant_reference<Receiver_i> receiver =
        CORBA::make_reference<Receiver_i> ();

      PortableServer::ObjectId id =
        root_poa->activate_object (receiver);

      IDL::traits<CORBA::Object>::ref_type object_act =
        root_poa->id_to_reference (id);

      IDL::traits<Receiver>::ref_type receiver_obj =
        IDL::traits<Receiver>::narrow (object_act);

      // Send the callback object to the server
      sender->receiver_object (receiver_obj);

      // Threads that will handle the call backs
      Client_Task client_task (orb, ACE_Thread_Manager::instance ());

      if (client_task.activate (THR_NEW_LWP | THR_JOINABLE, 4, 1) == -1)
        {
          TAOX11_TEST_ERROR << "Error activating client task" << std::endl;
          return 1;
        }

      ACE_Thread_Manager::instance ()->wait ();

      TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO << "Client threads have finished" << std::endl;

      uint32_t const count = receiver->get_event_count ();

      TAOX11_TEST_DEBUG << X11_LOG_THREAD_INFO << "Number of events received.. ["
        << count << "]" << std::endl;

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception in <client>: " << ex << std::endl;
      return 1;
    }

  return 0;
}
