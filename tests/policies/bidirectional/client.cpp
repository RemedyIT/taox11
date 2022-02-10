/**
 * @file    client.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 Bidir Policy client application
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */

#include "ace/Get_Opt.h"
#include "test_i.h"
#include "tao/x11/bidir_giop/bidir_giop.h"
#include "tao/x11/anytypecode/any.h"

#include "testlib/taox11_testlog.h"

std::string ior ("file://test.ior");

void do_nothing ()
{
}

bool
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("k:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior = get_opts.opt_arg ();
        break;

      case '?':
      default:
        TAOX11_TEST_ERROR << "usage:\n"
              << "-k <ior>" << std::endl;
        return false;
      }
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
        TAOX11_TEST_ERROR << "client - main - caught "
              << "unexpected policy error exception - "
              << ex << std::endl;
        return 1;
      }

      IDL::traits<BiDirPolicy::BidirectionalPolicy>::ref_type nil_bidir =
          IDL::traits<BiDirPolicy::BidirectionalPolicy>::narrow (nullptr);

      if (nil_bidir)
      {
        TAOX11_TEST_ERROR << "Narrow nil BiDirPolicy::BidirectionalPolicy should return nil"
            << std::endl;
        return 1;
      }

      // narrow from generic policy ref
      IDL::traits<BiDirPolicy::BidirectionalPolicy>::ref_type bidir_pol =
          IDL::traits<BiDirPolicy::BidirectionalPolicy>::narrow (policies[0]);

      if (!bidir_pol)
      {
        TAOX11_TEST_ERROR << "Unable to narrow BiDirPolicy::BidirectionalPolicy"
            << std::endl;
        return 1;
      }

      if (bidir_pol->value () != BiDirPolicy::BOTH)
      {
        TAOX11_TEST_ERROR << "Incorrect BiDirPolicy::BidirectionalPolicy set"
            << std::endl;
        return 1;
      }

      // implicitly widen to object ref
      IDL::traits<CORBA::Object>::ref_type polobj = bidir_pol;

      // (re-)narrow
      bidir_pol = IDL::traits<BiDirPolicy::BidirectionalPolicy>::narrow (polobj);

      if (!bidir_pol)
      {
        TAOX11_TEST_ERROR << "Unable to narrow BiDirPolicy::BidirectionalPolicy from CORBA::Object reference"
            << std::endl;
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

      IDL::traits<CORBA::Object>::ref_type object =
        orb->string_to_object (ior);

      IDL::traits<Simple_Server>::ref_type server =
        IDL::traits<Simple_Server>::narrow (object);

      if (!server)
      {
        TAOX11_TEST_ERROR << X11_LOG_THREAD_INFO
              << "Object reference <" << ior << "> is nil."
              << std::endl;
        return 1;
      }

      CORBA::servant_traits<Callback>::ref_type callback_impl =
        CORBA::make_reference<Callback_i> (orb);

      PortableServer::ObjectId id =
        root_poa->activate_object (callback_impl);

      IDL::traits<CORBA::Object>::ref_type object_act =
        root_poa->id_to_reference (id);

      IDL::traits<Callback>::ref_type callback =
        IDL::traits<Callback>::narrow (object_act);

      // Send the callback object to the server
      server->callback_object (callback);

      // A method to kickstart callbacks from the server
      int32_t const r = server->test_method (true);

      if (r != 0)
      {
        TAOX11_TEST_ERROR << X11_LOG_THREAD_INFO
              << "unexpected result = " << r
              << std::endl;
        return 1;
      }

      orb->run ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << "Caught exception in <client>: "
            << ex << std::endl;
      return 1;
    }

  return 0;
}
