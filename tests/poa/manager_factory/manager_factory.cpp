/**
 * @file    manager_factory.cpp
 * @author  Marcel Smit
 *
 * @brief   CORBA C++11 POA tests
 *
 * @copyright Copyright (c) Remedy IT Expertise BV
 */
#include "tao/x11/orb.h"
#include "tao/x11/corba.h"
#include "tao/x11/portable_server/portableserver_impl.h"

#include "testlib/taox11_testlog.h"

int
main (int argc, char *argv[])
{

  try
    {
      // Initialize the ORB first.
      IDL::traits<CORBA::ORB>::ref_type orb =
        CORBA::ORB_init (argc, argv, "POAManagerFactoryTest");

      // Obtain the RootPOA.
      IDL::traits<CORBA::Object>::ref_type obj =
        orb->resolve_initial_references ("RootPOA");

      // Narrow to POA.
      IDL::traits<PortableServer::POA>::ref_type root_poa =
        IDL::traits<PortableServer::POA>::narrow (obj);

      // Obtain the POAManagerFactory.
      IDL::traits<PortableServer::POAManagerFactory>::ref_type
      poa_manager_factory = root_poa->the_POAManagerFactory ();

      if (poa_manager_factory == nullptr)
        {
          TAOX11_TEST_ERROR << "Error: Manager Factory seems nil" << std::endl;
          return 1;
        }

      CORBA::PolicyList policies (0);
      policies.resize (0);

      // Explicitly create a POAManager - "POAManager1" .
      IDL::traits<PortableServer::POAManager>::ref_type poa_manager_1 =
        poa_manager_factory->create_POAManager ("POAManager1",
            policies);
      if (poa_manager_1 == nullptr)
        {
          TAOX11_TEST_ERROR << "Error: Manager Factory 1 seems nil" << std::endl;
          return 1;
        }

      // Creating a POAManager with an exiting POAManager name raises exception.
      bool got_expected_exception = false;
      try
        {
          IDL::traits<PortableServer::POAManager>::ref_type poa_manager =
            poa_manager_factory->create_POAManager ("POAManager1",
                policies);
        }
      catch (const PortableServer::POAManagerFactory::ManagerAlreadyExists& )
        {
          got_expected_exception = true;
        }
      if (!got_expected_exception)
        {
          TAOX11_TEST_ERROR << "Error: did not got the expected exception!" << std::endl;
          return 1;
        }

      {
        // Implicitly create a POAManager instance which has an automatically
        // assigned name.
        IDL::traits<PortableServer::POA>::ref_type
        child_poa = root_poa->create_POA ("childPOA2",
                                          nullptr,
                                          policies);

        IDL::traits<PortableServer::POAManager>::ref_type
        poa_manager_2 = child_poa->the_POAManager ();

        if (poa_manager_2 == nullptr)
          {
            TAOX11_TEST_ERROR << "Error: Implicitly create a POAManager failed" << std::endl;
            return 1;
          }

        std::string poa_manager_2_name = poa_manager_2->get_id ();
        TAOX11_TEST_DEBUG << "Implicitly created POAManager's ID: ["
          << poa_manager_2_name << "]" << std::endl;

        if (poa_manager_2_name.length () == 0)
          {
            TAOX11_TEST_DEBUG << "Error: ID POA Manager II seems empty."
              << std::endl;
            return 1;
          }

        PortableServer::POAManagerFactory::POAManagerSeq managers =
          poa_manager_factory->list ();

        if (managers.size () != 3)
          {
            TAOX11_TEST_ERROR << "Error: Unexpected number of managers found: "
              << "found <" << managers.size() << "> expected <3>." << std::endl;
            return 1;
          }

        IDL::traits<PortableServer::POAManager>::ref_type
        root_poa_manager = root_poa->the_POAManager();

        for (IDL::traits<PortableServer::POAManager>::ref_type tmp_pm : managers)
          {
            std::string name = tmp_pm->get_id ();
            if (name == "RootPOAManager" ||
                name == "POAManager1" ||
                name == poa_manager_2_name)
              {
                TAOX11_TEST_DEBUG << "Validate listed POAManager [" << name << "]: passed"
                  << std::endl;
                continue;
              }
            else
              {
                TAOX11_TEST_DEBUG << "Validate listed POAManager [" << name << "]: failed"
                  << std::endl;
              }
          }
      }

      {
        // Find a specific POAManager.
        IDL::traits<PortableServer::POAManager>::ref_type
          manager = poa_manager_factory->find ("POAManager1");

        std::string name = manager->get_id ();

        if (name != "POAManager1")
          {
            TAOX11_TEST_ERROR << "Error: Could not find POAManager1" << std::endl;
            return 1;
          }

        // Test that a widening to Object and narrow back works
        IDL::traits<CORBA::Object>::ref_type manager_object = manager;

        IDL::traits<PortableServer::POAManager>::ref_type manager2 =
          IDL::traits<PortableServer::POAManager>::narrow (manager);

        if (manager->get_id () != "POAManager1")
          {
            TAOX11_TEST_ERROR << "Error: Widening to object and narrowing back failed" << std::endl;
            return 1;
          }
      }

      // Create a child poa that associates with the explicitly
      // created POAManager.
      IDL::traits<PortableServer::POA>::ref_type child_poa =
        root_poa->create_POA ("childPOA", poa_manager_1, policies);

      poa_manager_1->activate ();

      root_poa->destroy (true, true);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      TAOX11_TEST_ERROR << ex;
      return 1;
    }

  return 0;
}
